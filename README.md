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
```cpp
// Inheret from ender::platform_window to easily spawn a window.
class menu_app : public ender::platform_window {
public:
    menu_app() : m_current_page(pages::login), platform_window() {
    }

    // Basic events to listen for.
    bool on_create() noexcept;
    void on_destroy() noexcept;
    bool on_handle_events() noexcept;
    void on_render_frame() noexcept;

    // Custom code for game/application.
    enum class pages { login, home };

private:
    pages m_current_page;
};

int main() {
    // Create and run the window.
    menu_app app = {};
    if (app.create(...) == true) {
        while (app.handle_events(...) == true) {
            app.render_frame(...);
        }

        app.destroy(...);
    }

    return 0;
}
```
## Support
Only supports 64-bit Windows at the moment.