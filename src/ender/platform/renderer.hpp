#pragma once
#include <vector>

#include <d3d11.h>
#include <directxmath.h>

namespace ender {
    struct vertex {
        DirectX::XMFLOAT2 position;
        DirectX::XMFLOAT4 color;
    };

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
              m_is_swap_chain_occluded(false) {
        }
        ~d3d11_renderer();

        bool create(HWND hwnd);
        bool destroy();

        bool is_swapchain_occluded();

        void render_frame();

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
}  // namespace ender
