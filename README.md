# ender
<img src="data/logo.png" align="left" width="350px"/>

### About
A 2D rendering framework for Windows, written in modern C++.
### Requirements
- Visual Studio 2022 (C++ 20)
- Directx11 Windows
### Building
1. Clone this repository.
2. Get dependencies from `ender/lib`.
3. Build in `Release\Debug` -> `x64`
<br clear="left"/>

## Example
Making intuitive and efficient GUI applications with C++ has been
a terrible experience for me so here is my attempt at an elegant solution.

```cpp
// include <platform/window.hpp>

// Inheret from ender::platform_window to easily spawn a window.
class menu_app : public ender::platform_window {
public:
    menu_app() : m_current_page(pages::login), platform_window() {
    }

    // Called when the window has been created.
    bool on_create() noexcept {
        set_window_title(L"menu app example");
        return true;
    }

    // Render imgui here.
    void on_render_frame_imgui() noexcept {
        ImGui::ShowDemoWindow();
    }
};

bool on_create_handler(ender::platform_window* ctx) {
    auto app = static_cast<menu_app*>(ctx);
    return app->on_create();
}

// These are the callbacks given to the window.
// Use them to call custom events in the class.
void on_render_frame_handler(ender::platform_window* ctx) {
    auto app = static_cast<menu_app*>(ctx);

    // Render ImGui event.
    app->on_render_frame_imgui();

    // Call some other custom event in your class.
    // app->on_render_frame()
}

int main() {
    // Create and run the window.
    // Put more windows in seperate threads...
    menu_app app = {};
    if (app.create(on_create_handler, {...}) == true) {
        while (app.handle_events(nullptr) == true) {
            app.render_frame(on_render_frame);
        }

        app.destroy(nullptr);
    }

    return 0;
}
```
The example above spawns a Win32, DirectX 11 window with ImGui
showing the demo window.
## Support
Only supports 64-bit Windows at the moment.