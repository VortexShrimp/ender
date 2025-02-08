# ender
<img src="data/logo.png" align="left" width="350px"/>

### About
A 2D rendering framework for Windows, written in modern C++ and Lua.
### Requirements
- Visual Studio 2022 (C++ 20)
- Directx11 Windows
### Building
1. Clone this repository.
2. Get dependencies from [`ender/lib`](https://github.com/VortexShrimp/ender/tree/master/lib).
3. Build in `Release\Debug` -> `x64`
<br clear="left"/>

Making intuitive and efficient GUI applications with C++ has been
a terrible experience for me so, here is my attempt at an elegant solution.

### Roadmap
- [ ] Complete console debug logging system with a compile-time switch.
- [ ] Consistent error or exception handling system.
- [ ] File handling system for configurations, scripting and more.
- [ ] Independant 2D batch sprite, primitive & text renderer.
## Example
The example below spawns a 64-bit Win32, Directx11 window running ImGui.
This is the [mess](https://github.com/ocornut/imgui/blob/master/examples/example_win32_directx11/main.cpp)
that *ender* is cleaning up.
```cpp
// include <ender/platform/window.hpp>
// include <imgui/imgui.h>

void on_render_frame(ender::window*) {
    // Run any ImGui code here.
    ImGui::ShowDemoWindow();
}

INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR cmd_line, INT cmd_show) {
    auto app = std::make_unique<ender::window>();
    if (app->create(nullptr, {.title = L"imgui demo window",
                                        .width = 1280,
                                        .height = 720,
                                        .on_message_create = nullptr,
                                        .on_message_destroy = nullptr,
                                        .on_message_close = nullptr,
                                        .instance = instance,
                                        .cmd_show = cmd_show}) == true) {
        while (app->handle_events(nullptr) == true) {
            app->render_frame(on_render_frame);
        }

        app->destroy(nullptr);

        return 0;
    }

    return 1;
}
```
<img src="data/menu_app_example.PNG" align="right" width="450px"></img>

Find more examples at [`ender/examples`](https://github.com/VortexShrimp/ender/tree/master/examples).

Any class inheritting from <code>ender\::window</code> can be infinitely created.
For example, you could have <code>std::vector<ender\::window*> windows</code>
to manage many windows, just make sure they run in their own threads.

All window messages will be routed through their own callbacks, if they've been set
during initialization. Use them to change what your windows do.

Each *ender* window also has a Lua context attached to it. Use the handlers and
callbacks to create event structures in Lua that can communicate with C++.

<br clear="right"/>

## Support
Only supports 64-bit Windows at the moment.