#pragma once
#include <vector>

#include <d2d1.h>
#include <d3d11.h>
#include <directxmath.h>

namespace ender {
    /**
     * @brief Currently a d3d11 renderer.
     */
    class d3d11_renderer {
    public:
        d3d11_renderer()
            : m_device(nullptr),
              m_device_context(nullptr),
              m_swap_chain(nullptr),
              m_render_target_view(nullptr),
              m_dxgi_factory(nullptr),
              m_is_swap_chain_occluded(false) {
        }
        ~d3d11_renderer();

        bool create(HWND hwnd);
        bool destroy();

        void render_frame();

        bool is_swapchain_occluded();
        void set_swap_chain_occluded(bool is_occluded);

        void handle_resize(HWND hwnd);

        ID3D11Device* device() const noexcept;
        ID3D11DeviceContext* device_context() const noexcept;

    private:
        bool create_render_target();

        ID3D11Device* m_device;
        ID3D11DeviceContext* m_device_context;
        IDXGISwapChain* m_swap_chain;
        ID3D11RenderTargetView* m_render_target_view;

        IDXGIFactory* m_dxgi_factory;

        bool m_is_swap_chain_occluded;
    };

    class direct2d_renderer {
    public:
        direct2d_renderer() : m_factory(nullptr), m_render_target(nullptr), m_brush(nullptr) {
        }
        ~direct2d_renderer();

        bool create(HWND hwnd);
        void destroy();

        void render_frame();

    private:
        ID2D1Factory* m_factory;
        ID2D1HwndRenderTarget* m_render_target;
        ID2D1SolidColorBrush* m_brush;
    };
}  // namespace ender
