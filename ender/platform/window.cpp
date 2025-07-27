#include "window.hpp"

#include <unordered_map>
#include <string>
#include <filesystem>

#include "../ender.hpp"
#include "../utils/console.hpp"

#ifdef ENDER_IMGUI
// Include all the imgui stuff.
#include <imgui\imgui.h>
#include <imgui\imgui_impl_dx11.h>
#include <imgui\imgui_impl_win32.h>
#endif  // ENDER_IMGUI

/**
 * @brief Stores data that is shared between wndproc and windows.
 *
 * Uses HWND as a key and stores data in a structure for a each window.
 *
 */
static std::unordered_map<HWND, ender::internal_window_data> s_window_data;

auto ender::get_internal_window_data(HWND hwnd) -> internal_window_data& {
    return s_window_data[hwnd];
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message,
                                                             WPARAM wparam, LPARAM lparam);

/**
 * @brief Dispatches window messages.
 *
 * Every ender::game_window created uses this function as its wndproc.
 * This then dispatches the correct messages to the correct windows
 * based on HWND. This allows ender::game_window's to work like standalone
 * objects.
 *
 * Not sure how good it scales but it should work just fine for a few
 * windows.
 *
 * @param hwnd
 * @param msg
 * @param wparam
 * @param lparam
 * @return
 */
static LRESULT WINAPI ender_wndproc_dispatch(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
#ifdef ENDER_IMGUI
    // Pass messages to ImGui.
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
        return true;
    }
#endif  // ENDER_IMGUI

    auto imgui_wants_focus = []() -> bool {
#ifdef ENDER_IMGUI
        // TODO: Add an option for window movement...
        //
        // Check if ImGui wants to capture the mouse.
        // This is used to determine whether to move the ImGui window, or OS
        // window when borderless window style is used.
        // return ImGui::GetIO().WantCaptureMouseUnlessPopupClose == true;
        return false;
#else
        return false;
#endif  // ENDER_IMGUI
    };

    // Get the window data for this window from the map.
    ender::internal_window_data& window_data = s_window_data[hwnd];

    switch (msg) {
        // Sent on CreateWindow(Ex).
        // lParam is CREATESTRUCT pointer.
        case WM_CREATE: {
            if (window_data.on_message_create != nullptr) {
                window_data.on_message_create();
            }
            break;
        }

        // Sent after the size of the window has changed.
        // https://learn.microsoft.com/en-us/windows/win32/winmsg/wm-size
        case WM_SIZE: {
            if (wparam == SIZE_MINIMIZED) {
                break;
            }

            window_data.resize_width = static_cast<UINT>(LOWORD(lparam));
            window_data.resize_height = static_cast<UINT>(HIWORD(lparam));
            break;
        }

        // Sent when a window should be terminated.
        // Use it to allow a user to confirm their exit.
        case WM_CLOSE: {
            if (window_data.on_message_close != nullptr) {
                const bool should_confirm_exit = window_data.on_message_close();
                if (should_confirm_exit == true) {
                    // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebox
                    const int choice = MessageBox(hwnd, L"Would you like to exit this window?",
                                                  L"ender", MB_YESNO | MB_ICONEXCLAMATION);

                    // User clicked yes.
                    if (choice == IDYES) {
                        DestroyWindow(hwnd);
                        break;
                    } else {  // Keep running because user selected no.
                        break;
                    }
                }
            }

            DestroyWindow(hwnd);
            break;
        }

        // Sent on DestroyWindow.
        case WM_DESTROY: {
            if (window_data.on_message_destroy != nullptr) {
                window_data.on_message_destroy();
            }

            PostQuitMessage(EXIT_SUCCESS);
            break;
        }

        // Sent by PostQuitMessage.
        // wParam is the exit code passed to PostQuitMessage.
        case WM_QUIT: {
            break;
        }

        case WM_LBUTTONDOWN: {
            if (window_data.style == ender::window_style::borderless &&
                imgui_wants_focus() == false) {
                POINT mouse_position = {LOWORD(lparam), HIWORD(lparam)};

                // if (mouse_position.y < 100) {
                // Start dragging the window.
                window_data.is_dragging = true;
                window_data.drag_start.x = mouse_position.x;
                window_data.drag_start.y = mouse_position.y;
                SetCapture(hwnd);
                //}
            }

            break;
        }

        case WM_MOUSEMOVE: {
            if (window_data.style == ender::window_style::borderless &&
                imgui_wants_focus() == false) {
                if (window_data.is_dragging == true) {
                    // Move the window.
                    POINT current_pos = {LOWORD(lparam), HIWORD(lparam)};
                    POINT delta = {current_pos.x - window_data.drag_start.x,
                                   current_pos.y - window_data.drag_start.y};
                    RECT rc;
                    GetWindowRect(hwnd, &rc);
                    SetWindowPos(hwnd, nullptr, rc.left + delta.x, rc.top + delta.y, 0, 0,
                                 SWP_SHOWWINDOW | SWP_NOZORDER | SWP_NOSIZE);
                }
            }

            break;
        }

        case WM_LBUTTONUP: {
            if (window_data.style == ender::window_style::borderless &&
                imgui_wants_focus() == false) {
                // Stop dragging the window.
                window_data.is_dragging = false;
                ReleaseCapture();
            }

            break;
        }

        case WM_DPICHANGED: {
            break;
        }

        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return 0;
}

bool ender::imgui_window::create(window_details details) {
    m_instance = nullptr;

    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = ender_wndproc_dispatch;
    wcex.cbClsExtra = 0L;
    wcex.cbWndExtra = 0L;
    wcex.hInstance = m_instance;
    wcex.hIcon = nullptr;
    wcex.hCursor = nullptr;
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = details.class_name.data();
    wcex.hIconSm = nullptr;

    m_wcex = RegisterClassExW(&wcex);
    if (m_wcex == ATOM{}) {
        debug_print("[error] Failed to register window class. Last error -> {}\n", GetLastError());
        return false;
    }

    DWORD style = {};
    switch (details.style) {
        case window_style::resizable:
            style = WS_OVERLAPPEDWINDOW;
            break;
        case window_style::borderless:
            style = WS_POPUP;
            break;
    }

    SetProcessDPIAware();

    // To make transparent windows, set exteneded style to WS_EX_LAYERED.

    m_hwnd = CreateWindowW(wcex.lpszClassName, details.title.data(), style, 100, 100, details.width,
                           details.height, nullptr, nullptr, m_instance, nullptr);
    if (m_hwnd == nullptr) {
        debug_print("[error] Failed to create window class. Last error -> {}\n", GetLastError());
        UnregisterClassW(MAKEINTATOM(m_wcex), m_instance);
        return false;
    }

    // SetLayeredWindowAttributes(m_hwnd, RGB(0, 0, 0), 255, LWA_COLORKEY);

    // Create and initialize the renderer.
    m_renderer = std::make_unique<d3d11_imgui_renderer>();
    if (m_renderer->create(m_hwnd) == false) {
        debug_print("[error] Failed to create renderer. Last error -> {}\n", GetLastError());
        DestroyWindow(m_hwnd);
        UnregisterClassW(MAKEINTATOM(m_wcex), m_instance);
        return false;
    }

    ShowWindow(m_hwnd, SW_SHOWDEFAULT);
    UpdateWindow(m_hwnd);

#ifdef ENDER_IMGUI
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.IniFilename = NULL;  // Disable .ini config saving.

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(m_hwnd);
    ImGui_ImplDX11_Init(m_renderer->device(), m_renderer->device_context());
#endif  // ENDER_IMGUI

    // Add this window to the global lookup table.
    s_window_data[m_hwnd] = internal_window_data{.window = this,
                                                 .resize_width = 0,
                                                 .resize_height = 0,
                                                 .style = details.style,
                                                 .is_dragging = false,
                                                 .drag_start = {0, 0},
                                                 .on_message_create = details.on_message_create,
                                                 .on_message_destroy = details.on_message_destroy,
                                                 .on_message_close = details.on_message_close};
    m_is_running = true;

    return m_is_running;
}

bool ender::imgui_window::destroy() {
#ifdef ENDER_IMGUI
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
#endif  // ENDER_IMGUI

    m_renderer->destroy();
    m_renderer.reset(nullptr);

    DestroyWindow(m_hwnd);
    UnregisterClassW(MAKEINTATOM(m_wcex), m_instance);

    // Remove the window from the global lookup table.
    if (s_window_data.find(m_hwnd) != s_window_data.end()) {
        s_window_data.erase(m_hwnd);
    }

    return true;
}

bool ender::imgui_window::process_events() {
    MSG message;
    while (PeekMessage(&message, nullptr, 0U, 0U, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);

        if (message.message == WM_QUIT) {
            return m_is_running = false;
        }
    }

    return m_is_running;
}

void ender::imgui_window::pre_render_frame() {
    if (m_is_running == false) {
        return;
    }

    if (m_renderer->is_swapchain_occluded() == false) {
        return;
    }
    m_renderer->set_swap_chain_occluded(false);

    m_renderer->handle_resize(m_hwnd);

#ifdef ENDER_IMGUI
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
#endif  // ENDER_IMGUI
}

void ender::imgui_window::post_render_frame() {
    if (m_is_running == false) {
        return;
    }

    if (m_renderer->is_swapchain_occluded() == false) {
        return;
    }

    m_renderer->render_frame();
}

bool ender::imgui_window::set_title(std::wstring_view new_title) {
    return SetWindowText(m_hwnd, new_title.data());
}

std::wstring_view ender::imgui_window::get_title() const {
    wchar_t buffer[256];
    GetWindowText(m_hwnd, buffer, 256);
    return buffer;
}

bool ender::imgui_window::is_running() const noexcept {
    return m_is_running;
}

auto ender::imgui_window::get_client_size() const noexcept -> DirectX::XMINT2 {
    RECT rect;
    GetClientRect(m_hwnd, &rect);
    return {rect.right - rect.left, rect.bottom - rect.top};
}

auto ender::imgui_window::get_window_size() const noexcept -> DirectX::XMINT2 {
    RECT rect;
    GetWindowRect(m_hwnd, &rect);
    return {rect.right - rect.left, rect.bottom - rect.top};
}

float ender::imgui_window::get_delta_time_seconds() {
    return m_timer.delta_time_seconds();
}
