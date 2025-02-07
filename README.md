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
## Todo
- Complete console debug logging system with a compile-time switch to easily exclude it from compilation in `Release` builds.
- Decent and consistent error/exception handling system to work with the logger.
- File handling system to create/manage files and folders on the system for configurations, scripting and more.
- Independant 2D batch sprite, primitive & text renderer built with DirectX11.

## Example
```cpp
// include <ender/platform/window.hpp>
// include <imgui/imgui.h>

void on_render_frame_handler(ender::platform_window*) {
    // Run any ImGui code here.
    ImGui::ShowDemoWindow();
}

INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR cmd_line, INT cmd_show) {
    auto app = std::make_unique<menu_app>();
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

        return 0;
    }

    return 1;
}
```
<img src="data/menu_app_example.PNG" align="right" width="450px"></img>

The example above spawns a 64-bit Win32, Directx11 window with ImGui,
showing the demo window.

Click [here](https://github.com/ocornut/imgui/blob/master/examples/example_win32_directx11/main.cpp)
to see why *ender* is "elegant".

Any class inheretting from <code>ender\::platform_window</code> can be infinitely created.
For example, you could have <code>std::vector<menu_app*> windows</code>
to manage many windows, just make sure they run in their own threads.

All window messages will be routed through their own callbacks, if they've been set
during initialization. Use them to change what your windows do.

<br clear="right"/>

## Support
Only supports 64-bit Windows at the moment.