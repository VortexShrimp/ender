#include "window.hpp"

#include <unordered_map>
#include <vector>
#include <string>

#include "../ender.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

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
    if constexpr (ender::use_imgui == true) {
        // Pass messages to ImGui.
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
            return true;
        }
    }

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
                const bool should_confirm_exit = window_data.on_message_close(window_data.window);
                if (should_confirm_exit == true) {
                    // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebox
                    const int choice = MessageBox(hwnd, L"Would you like to exit this window?",
                                                  L"ender", MB_YESNO | MB_ICONEXCLAMATION);

                    // User clicked yes.
                    if (choice == IDYES) {
                        DestroyWindow(hwnd);
                        break;
                    } else {
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

        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
    }

    return 0;
}

bool ender::platform_window::create(create_function on_create, window_details details) {
    m_instance = details.instance;

    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_CLASSDC;
    wcex.lpfnWndProc = ender_wndproc_dispatch;
    wcex.cbClsExtra = 0L;
    wcex.cbWndExtra = 0L;
    wcex.hInstance = m_instance;
    wcex.hIcon = nullptr;
    wcex.hCursor = nullptr;
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"ender_window";
    wcex.hIconSm = nullptr;

    m_wcex = RegisterClassExW(&wcex);
    if (m_wcex == ATOM{}) {
        return false;
    }

    m_hwnd =
        CreateWindowW(wcex.lpszClassName, details.title.data(), WS_OVERLAPPEDWINDOW, 100, 100,
                      details.width, details.height, nullptr, nullptr, details.instance, nullptr);
    if (m_hwnd == nullptr) {
        UnregisterClassW(MAKEINTATOM(m_wcex), m_instance);
        return false;
    }

    // Create and initialize the renderer.
    m_renderer = make_unique_pointer<d3d11_renderer>();
    if (m_renderer->create(m_hwnd) == false) {
        DestroyWindow(m_hwnd);
        UnregisterClassW(MAKEINTATOM(m_wcex), m_instance);
        return false;
    }

    ShowWindow(m_hwnd, details.cmd_show);
    UpdateWindow(m_hwnd);

    if constexpr (use_imgui == true) {
        // Initialize ImGui.
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable keyboard.
        io.IniFilename = NULL;                                 // Disable .ini config saving.

        ImGui::StyleColorsDark();

        ImGui_ImplWin32_Init(m_hwnd);
        ImGui_ImplDX11_Init(m_renderer->get_device(), m_renderer->get_device_context());
    }

    // Add this window to the global lookup table.
    s_window_data[m_hwnd] = internal_window_data{.window = this,
                                                 .resize_width = 0,
                                                 .resize_height = 0,
                                                 .on_message_create = details.on_message_create,
                                                 .on_message_destroy = details.on_message_destroy,
                                                 .on_message_close = details.on_message_close};

    m_is_running = true;

    // If callback exists, return based on it.
    if (on_create != nullptr) {
        return on_create(this);
    }

    return m_is_running;
}

bool ender::platform_window::destroy(destroy_function on_destroy) {
    if (on_destroy != nullptr) {
        on_destroy(this);
    }

    if constexpr (use_imgui == true) {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

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

bool ender::platform_window::handle_events(handle_events_function on_handle_events) {
    MSG message;
    while (PeekMessage(&message, nullptr, 0U, 0U, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);

        if (message.message == WM_QUIT) {
            return m_is_running = false;
        }
    }

    if (on_handle_events != nullptr) {
        return on_handle_events(this);
    }

    return m_is_running;
}

void ender::platform_window::render_frame(render_frame_function on_render_frame) {
    if (m_is_running == false) {
        return;
    }

    if (m_renderer->is_swapchain_occluded() == false) {
        return;
    }
    m_renderer->set_swap_chain_occluded(false);
    m_renderer->handle_resize(m_hwnd);

    if (use_imgui == true) {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    if (on_render_frame != nullptr) {
        on_render_frame(this);
    }

    m_renderer->render_frame();
}

bool ender::platform_window::create_lua_state() {
    m_lua_state.open_libraries();
    m_lua_state.script_file("scripts\\ender_context.lua");

    auto lua_ender_context = m_lua_state["ender_context"];
    if (lua_ender_context.valid() == true) {
        lua_ender_context["engine_name"] = engine_name;
        lua_ender_context["version_string"] = version_string;
    }

    return true;
}

bool ender::platform_window::set_title(std::wstring_view new_title) {
    return SetWindowText(m_hwnd, new_title.data());
}

std::wstring_view ender::platform_window::get_title() const {
    wchar_t buffer[256];
    GetWindowText(m_hwnd, buffer, 256);
    return buffer;
}

bool ender::platform_window::is_running() const noexcept {
    return m_is_running;
}

auto ender::platform_window::get_client_size() const noexcept -> vec2i {
    RECT rect;
    GetClientRect(m_hwnd, &rect);
    return {rect.right - rect.left, rect.bottom - rect.top};
}

auto ender::platform_window::get_window_size() const noexcept -> vec2i {
    RECT rect;
    GetWindowRect(m_hwnd, &rect);
    return {rect.right - rect.left, rect.bottom - rect.top};
}

float ender::platform_window::get_delta_time() {
    return m_timer.get_delta_time();
}
