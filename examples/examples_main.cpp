#include "menu_app.hpp"

namespace examples {
    constexpr bool build_menu_app = true;
}

INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR cmd_line, INT cmd_show) {
    if constexpr (examples::build_menu_app == true) {
        menu_app::menu_app app = {};
        if (app.create(menu_app::on_create_handler,
                       {.title = L"menu app",
                        .width = 300,
                        .height = 500,
                        .on_message_create = nullptr,
                        .on_message_destroy = nullptr,
                        .on_message_close = menu_app::on_message_close_handler,  // Confirm Exit.
                        .instance = instance,
                        .cmd_show = cmd_show}) == true) {
            while (app.handle_events(menu_app::on_handle_events_handler) == true) {
                app.render_frame(menu_app::on_render_frame_handler);
            }

            app.destroy(menu_app::on_destroy_handler);

            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
}
