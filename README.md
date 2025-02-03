# ender
<img src="data/logo.png" align="center" width="300px"/>
A 2D rendering framework for Windows,
written in modern C++.
<br clear="center"/>

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
<img src="data/menu_app.png" align="right" width="300px"/>
The above code is an example of a simple window created with ender
and ImGui.
<br clear="left"/>


## Geting Started
### Requirements
- Visual Studio 2022 (C++ 20)
- Directx11 Windows
### Building
1. Clone this repository or download a release version.
2. Download/build required binaries from `ender/lib`.
3. Open the `.sln` file. (Development done in VS2022)
4. Build in `Release\Debug x64`

## support
Only supports 64-bit Windows at the moment.