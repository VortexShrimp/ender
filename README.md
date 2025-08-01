# ender
A Windows GUI framework for modern C++ with a focus on simplicity,
elegance and efficiency. Various usage examples can be found in
[`ender/examples`](https://github.com/VortexShrimp/ender/tree/master/examples).

## About
Anyone who regularly uses C++ will eventually run into the problem of
Desktop Application GUI development. The options are limited and the
available libraries are often bloated, outdated or just plain difficult to
use.

<img src="data/logo.png" align="left" width="350px"/>

### Requirements

- 64-Bit Windows & DirectX 11
- C++ Build Tools (Visual Studio 2022)
- Source Dependencies [[`ender/lib`](https://github.com/VortexShrimp/ender/tree/master/ender/lib)]

### Building Examples

1. `git clone https://github.com/VortexShrimp/ender.git`
2. Open `ender.sln` in Visual Studio
3. Build as `x64` -> `Release` or `Debug`
4. Find `ender.exe` in `ender/build/x64` by default

<br clear="left"/>

## Usage Example
The following example shows you how to use `ender::imgui_window` to create a simple
ImGui demo window.

```cpp
#include <memory>
#include <ender/platform/window.hpp>
#include <imgui/imgui.h>

/**
 * This is a minimal example of how to use ender with ImGui.
 */
class imgui_demo_window final : public ender::imgui_window {
public:
    simple_app() : imgui_window() {
    }

    bool create(ender::window_details details) noexcept {
        bool result = imgui_window::create(details);

        // Any additional initialization can be done here.
        // For example, setting up ImGui styles or loading fonts.
        // Or setting up app-specific resources.

        return result;
    }

    bool destroy() noexcept {
        // Any additional cleanup can be done here.
        // For example, releasing resources or saving settings.

        return imgui_window::destroy();
    }

    bool process_events() noexcept {
        return imgui_window::process_events();
    }

    void render_frame() noexcept {
        imgui_window::pre_render_frame();

        // Run any ImGui code to render here.
        ImGui::ShowDemoWindow();

        imgui_window::post_render_frame();
    }

private:
    // Any additional member variables can be declared here.
    // For example, to store application state or resources.
    // Just make sure to initialize them in the constructor
    // or the create method.
};

int main() {
    auto app = std::make_unique<imgui_demo_window>();
    if (app->create({.title = L"ImGui Demo Window",
                     .class_name = L"imgui_demo_class",
                     .width = 1280,
                     .height = 720}) == true) {
        while (app->process_events() == true) {
            app->render_frame();
        }

        app->destroy();
        return 0;
    }

    return 1;
}
```

## Roadmap

- [ ] **[Almost Complete]** Debug console logging system.
- [ ] **[Almost Complete]** Consistent error or exception handling system.
- [ ] **[In Progress]** File handling system for configurations, scripting and more.
- [ ] **[In Progress]** Independant 2D batch sprite, primitive & text renderer.
- [ ] Script compressing on disk for distribution and Lua encryption.

<!--
## Example
### Simple Window
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

INT WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, INT) {
    auto app = std::make_unique<ender::window>();
    if (app->create(nullptr, {.title = L"simple window",
                              .class_name = L"simple_class",
                              .width = 1280,
                              .height = 720,
                              .on_message_create = nullptr,
                              .on_message_destroy = nullptr,
                              .on_message_close = nullptr}) == true) {
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
to manage many windows.

All window messages will be routed through their own callbacks, if they've been set
during initialization. Use them to change what your windows do.

This example is using *ender* as a window framework with ImGui, but its much
more capable than that.

<br clear="right"/>

### Crypto Price Checker
<img src="data/price_checker_example.PNG" align="left" width="300px"></img>
This project's user interface is scripted with Lua by setting up an `ender::lua_window`.
See [`examples/crypto_price_checker`](https://github.com/VortexShrimp/ender/tree/master/examples/crypto_price_checker)
for the source code.

It makes asynchronous get requests to a crypto REST API and parses
the json before displaying the coin's information.

<br clear="left"/>

## Distribution
Applications created with *ender* are fully self containing. Projects created
with `ender::window` can be destributed as an executable only.

Applications created with `ender::lua_window` will simply require a "script"
directory alongside the executable containing the desired scripts.

## Support
Only supports 64-bit Windows at the moment.
-->
