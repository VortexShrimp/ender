# ender
<img src="data/logo.png" align="left" width="350px"/>

A 2D rendering framework for Windows, written in modern C++.
## Requirements
- Visual Studio 2022 (C++ 20)
- Directx11 Windows
## Building
1. Clone this repository.
2. Get dependencies from `ender/lib`.
3. Build in `Release\Debug` -> `x64`
<br clear="left"/>

## Example
```cpp
// Inheret from ender::game_window to spawn a window.
class menu_app : public ender::game_window {
public:
    menu_app() : m_current_page(pages::login), game_window() {
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
```
## Support
Only supports 64-bit Windows at the moment.