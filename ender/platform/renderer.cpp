#include "renderer.hpp"

#include "../ender.hpp"
#include "window.hpp"

#include "../external/imgui/imgui.h"
#include "../external/imgui/imgui_impl_dx11.h"
#include "../external/imgui/imgui_impl_win32.h"

/**
 * @brief Safely release Directx resources.
 * @tparam T
 * @param ptr
 */
template <class T>
inline void safe_release(T*& ptr) noexcept {
    if (ptr) {
        ptr->Release();
        ptr = nullptr;
    }
}

ender::d3d11_imgui_renderer::~d3d11_imgui_renderer() {
    destroy();
}

bool ender::d3d11_imgui_renderer::create(HWND hwnd) {
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

bool ender::d3d11_imgui_renderer::destroy() {
    safe_release(m_render_target_view);
    safe_release(m_dxgi_factory);
    safe_release(m_device_context);
    safe_release(m_device);
    safe_release(m_swap_chain);

    return true;
}

bool ender::d3d11_imgui_renderer::is_swapchain_occluded() {
    return m_is_swap_chain_occluded == false &&
           m_swap_chain->Present(0, DXGI_PRESENT_TEST) != DXGI_STATUS_OCCLUDED;
}

void ender::d3d11_imgui_renderer::render_frame(std::array<float, 4> clear_color) {
#ifdef ENDER_IMGUI
    ImGui::Render();
#endif  // ENDER_IMGUI

    m_device_context->OMSetRenderTargets(1, &m_render_target_view, nullptr);
    m_device_context->ClearRenderTargetView(m_render_target_view, clear_color.data());

#ifdef ENDER_IMGUI
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif  // ENDER_IMGUI

    HRESULT hr = m_swap_chain->Present(1, 0);
    m_is_swap_chain_occluded = (hr == DXGI_STATUS_OCCLUDED);
}

void ender::d3d11_imgui_renderer::set_swap_chain_occluded(bool is_occluded) {
    m_is_swap_chain_occluded = is_occluded;
}

void ender::d3d11_imgui_renderer::handle_resize(HWND hwnd) {
    internal_window_data& window_data = get_internal_window_data(hwnd);
    if (window_data.resize_width != 0 && window_data.resize_height != 0) {
        safe_release(m_render_target_view);
        m_swap_chain->ResizeBuffers(0, window_data.resize_width, window_data.resize_height,
                                    DXGI_FORMAT_UNKNOWN, 0);
        create_render_target();

        window_data.resize_width = 0;
        window_data.resize_height = 0;
    }
}

ID3D11Device* ender::d3d11_imgui_renderer::device() const noexcept {
    return m_device;
}

ID3D11DeviceContext* ender::d3d11_imgui_renderer::device_context() const noexcept {
    return m_device_context;
}

bool ender::d3d11_imgui_renderer::create_render_target() {
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
