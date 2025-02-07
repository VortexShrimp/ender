# ender
<img src="data/logo.png" align="left" width="350px"/>

### About
A 2D rendering framework for Windows, written in modern C++ and Lua.
### Requirements
- Visual Studio 2022 (C++ 20)
- Directx11 Windows
### Building
1. Clone this repository.
2. Get dependencies from `ender/lib`.
3. Build in `Release\Debug` -> `x64`
<br clear="left"/>

Making intuitive and efficient GUI applications with C++ has been
a terrible experience for me so, here is my attempt at an elegant solution.

## Example
```cpp
// include <platform/window.hpp>

// Inheret from ender::platform_window to easily spawn a window.
class menu_app : public ender::platform_window {
public:
    menu_app() : platform_window() {
    }

    // Render imgui here.
    void on_render_frame_imgui() noexcept {
        ImGui::ShowDemoWindow();
    }
};

// These are the callbacks given to the window.
// Use them to call custom events in the class.
void on_render_frame_handler(ender::platform_window* ctx) {
    auto app = static_cast<menu_app*>(ctx);

    // Render ImGui event.
    app->on_render_frame_imgui();

    // Call some other custom event in your class.
    // app->on_render_frame()
}

// Example Windows entry point.
INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR cmd_line, INT cmd_show) {
    // Create and run the window.
    // Put more windows in seperate threads...
    auto app = ender::make_unique_pointer<menu_app>();
    if (app->create(nullptr, {.title = L"menu app",
                                        .width = 1280,
                                        .height = 720,
                                        .on_message_create = nullptr,
                                        .on_message_destroy = nullptr,
                                        .on_message_close = nullptr,
                                        .instance = instance,
                                        .cmd_show = cmd_show}) == true) {
        while (app->handle_events(nullptr) == true) {
            app->render_frame(on_render_frame_handler);
        }

        app->destroy(nullptr);
    }

    return 0;
}
```
<img src="data/menu_app_example.PNG" align="right" width="350px"></img>

The example above spawns a <i>64-bit win32, directx11 window</i> with ImGui
showing the demo window.

Any class inheretting from <code>ender\::platform_window</code> can infinitely
create windows. For example, you could have <code>std::vector<menu_app*> windows</code>
to manage many windows, just make sure they run in their own threads.

All window messages will be routed to their own callbacks, if they've been set
during initialization.

<br clear="right"/>

## Support
Only supports 64-bit Windows at the moment.