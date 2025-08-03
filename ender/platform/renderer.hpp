#pragma once
#include <vector>
#include <array>

#include <d2d1.h>
#include <d3d11.h>
#include <directxmath.h>

#include <directxtk\GeometricPrimitive.h>

namespace ender {
    /**
     * @brief Currently a d3d11 renderer that depends
     * heavily on ImGui's architecture.
     *
     * The goal is to add many more renderers in the future.
     */
    class d3d11_imgui_renderer {
    public:
        d3d11_imgui_renderer()
            : m_device(nullptr),
              m_device_context(nullptr),
              m_swap_chain(nullptr),
              m_render_target_view(nullptr),
              m_dxgi_factory(nullptr),
              m_is_swap_chain_occluded(false) {
        }
        ~d3d11_imgui_renderer();

        bool create(HWND hwnd);
        [[maybe_unused]] bool destroy();

        void render_frame(std::array<float, 4> clear_color = m_default_clear_color);

        bool is_swapchain_occluded();
        void set_swap_chain_occluded(bool is_occluded);

        void handle_resize(HWND hwnd);

        ID3D11Device* device() const noexcept;
        ID3D11DeviceContext* device_context() const noexcept;

    private:
        constexpr static std::array<float, 4> m_default_clear_color = {0.2f, 0.2f, 0.2f, 1.0f};

        bool create_render_target();

        ID3D11Device* m_device;
        ID3D11DeviceContext* m_device_context;
        IDXGISwapChain* m_swap_chain;
        ID3D11RenderTargetView* m_render_target_view;

        IDXGIFactory* m_dxgi_factory;

        bool m_is_swap_chain_occluded;
    };
}  // namespace ender
