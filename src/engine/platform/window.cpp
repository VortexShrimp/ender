#include "window.hpp"

#include <unordered_map>
#include <vector>

#include "../ender.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

namespace ender {
    /**
     * @brief Currently a d3d11 renderer.
     */
    class game_renderer {
    public:
        game_renderer()
            : m_device(nullptr),
              m_device_context(nullptr),
              m_swap_chain(nullptr),
              m_render_target_view(nullptr),
              m_is_swap_chain_occluded(false) {
        }
        ~game_renderer();

        bool create(HWND hwnd);
        bool destroy();

        bool is_swapchain_occluded();

        void render_frame();
        void post_render_frame();

        void set_swap_chain_occluded(bool is_occluded);
        void handle_resize(HWND hwnd);

        ID3D11Device* get_device() const noexcept;
        ID3D11DeviceContext* get_device_context() const noexcept;

    private:
        bool create_render_target();
        void destroy_render_target();

        ID3D11Device* m_device;
        ID3D11DeviceContext* m_device_context;
        IDXGISwapChain* m_swap_chain;
        ID3D11RenderTargetView* m_render_target_view;

        bool m_is_swap_chain_occluded;
    };

    /**
     * @brief Data that needs to be shared between Window and Message Loop.
     */
    struct engine_window_data {
        UINT resize_width;  // Resizing is handled in render loop and wndproc.
        UINT resize_height;

        game_window::message_create_function on_message_create;
        game_window::message_destroy_function on_message_destroy;
    };
}  // namespace ender

static std::unordered_map<HWND, ender::engine_window_data> s_window_data;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message,
                                                             WPARAM wparam, LPARAM lparam);

static LRESULT WINAPI ender_wndproc_dispatch(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    if constexpr (ender::use_imgui == true) {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
            return true;
        }
    }

    ender::engine_window_data& window_data = s_window_data[hwnd];

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
            // https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebox
            const int choice = MessageBox(hwnd, L"Would you like to exit this window?", L"ender",
                                          MB_YESNO | MB_ICONEXCLAMATION);
            if (choice == IDYES) {
                DestroyWindow(hwnd);
            }
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

ender::game_renderer::~game_renderer() {
    destroy();
}

bool ender::game_renderer::create(HWND hwnd) {
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hwnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT create_device_flags = 0;
    D3D_FEATURE_LEVEL feature_level;
    constexpr D3D_FEATURE_LEVEL feature_level_arr[2] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_0,
    };

    HRESULT res = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, create_device_flags, feature_level_arr, 2,
        D3D11_SDK_VERSION, &sd, &m_swap_chain, &m_device, &feature_level, &m_device_context);
    if (res == DXGI_ERROR_UNSUPPORTED) {
        res = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_WARP, nullptr, create_device_flags, feature_level_arr, 2,
            D3D11_SDK_VERSION, &sd, &m_swap_chain, &m_device, &feature_level, &m_device_context);
    }

    if (res != S_OK) {
        return false;
    }

    if (create_render_target() == false) {
        return false;
    }

    return true;
}

bool ender::game_renderer::destroy() {
    destroy_render_target();

    if (m_swap_chain != nullptr) {
        m_swap_chain->Release();
        m_swap_chain = nullptr;
    }

    if (m_device_context != nullptr) {
        m_device_context->Release();
        m_device_context = nullptr;
    }

    if (m_device != nullptr) {
        m_device->Release();
        m_device = nullptr;
    }

    return true;
}

bool ender::game_renderer::is_swapchain_occluded() {
    return m_is_swap_chain_occluded == false &&
           m_swap_chain->Present(0, DXGI_PRESENT_TEST) != DXGI_STATUS_OCCLUDED;
}

void ender::game_renderer::render_frame() {
    ImVec4 clear_color = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
    const float clear_color_with_alpha[4] = {clear_color.x * clear_color.w,
                                             clear_color.y * clear_color.w,
                                             clear_color.z * clear_color.w, clear_color.w};
    m_device_context->OMSetRenderTargets(1, &m_render_target_view, nullptr);
    m_device_context->ClearRenderTargetView(m_render_target_view, clear_color_with_alpha);
}

void ender::game_renderer::post_render_frame() {
    HRESULT hr = m_swap_chain->Present(0, 0);
    m_is_swap_chain_occluded = (hr == DXGI_STATUS_OCCLUDED);
}

void ender::game_renderer::set_swap_chain_occluded(bool is_occluded) {
    m_is_swap_chain_occluded = is_occluded;
}

void ender::game_renderer::handle_resize(HWND hwnd) {
    engine_window_data& window_data = s_window_data[hwnd];
    if (window_data.resize_width != 0 && window_data.resize_height != 0) {
        destroy_render_target();
        m_swap_chain->ResizeBuffers(0, window_data.resize_width, window_data.resize_height,
                                    DXGI_FORMAT_UNKNOWN, 0);
        create_render_target();

        window_data.resize_width = 0;
        window_data.resize_height = 0;
    }
}

ID3D11Device* ender::game_renderer::get_device() const noexcept {
    return m_device;
}

ID3D11DeviceContext* ender::game_renderer::get_device_context() const noexcept {
    return m_device_context;
}

bool ender::game_renderer::create_render_target() {
    ID3D11Texture2D* back_buffer;
    if (m_swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer)) != S_OK) {
        return false;
    }

    if (m_device->CreateRenderTargetView(back_buffer, nullptr, &m_render_target_view) != S_OK) {
        return false;
    }

    back_buffer->Release();

    return true;
}

void ender::game_renderer::destroy_render_target() {
    if (m_render_target_view != nullptr) {
        m_render_target_view->Release();
        m_render_target_view = nullptr;
    }
}

bool ender::game_window::create(create_function on_create, window_details details) {
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
    m_renderer = make_unique_pointer<game_renderer>();
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
    s_window_data[m_hwnd] = engine_window_data{.resize_width = 0,
                                               .resize_height = 0,
                                               .on_message_create = details.on_message_create,
                                               .on_message_destroy = details.on_message_destroy};

    m_is_running = true;

    // If callback exists, return based on it.
    if (on_create != nullptr) {
        return on_create(this);
    }

    return m_is_running;
}

bool ender::game_window::destroy(destroy_function on_destroy) {
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

bool ender::game_window::handle_events(handle_events_function on_handle_events) {
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

void ender::game_window::render_frame(render_frame_function on_render_frame) {
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

    if constexpr (use_imgui == true) {
        ImGui::Render();
    }

    m_renderer->render_frame();

    if constexpr (use_imgui == true) {
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    m_renderer->post_render_frame();
}

bool ender::game_window::set_title(std::wstring_view new_title) {
    return SetWindowText(m_hwnd, new_title.data());
}

bool ender::game_window::is_running() const noexcept {
    return m_is_running;
}

void ender::game_window::stop_running() noexcept {
    m_is_running = false;
}

void ender::game_window::get_client_size(int& width, int& height) {
    RECT rect;
    GetClientRect(m_hwnd, &rect);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
}

void ender::game_window::get_window_size(int& width, int& height) {
    RECT rect;
    GetWindowRect(m_hwnd, &rect);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;
}

float ender::game_window::get_delta_time() {
    return m_timer.get_delta_time();
}
