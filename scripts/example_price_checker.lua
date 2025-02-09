function crypto_on_create ()
    -- Test get request from Lua.
    local response = get_request("jsonplaceholder.typicode.com", "/todos/1")
    debug_print_raw(response)
    debug_print_raw("\n")

    -- Add fonts.
    imgui_add_font_from_file_ttf("C:\\Windows\\Fonts\\bahnschrift.ttf", 42)
    imgui_add_font_from_file_ttf("C:\\Windows\\Fonts\\bahnschrift.ttf", 16)

    imgui_set_defualt_font(1)
end

function crypto_on_destroy ()
end

function crypto_on_process_events ()
end

-- Page number is stored in the C++ class.
function crypto_on_render_imgui(page_number)
    -- Make the ImGui window the same size the os window.
    imgui_set_next_window_position(0, 0)
    imgui_set_next_window_size_to_client_size()

    -- The random numbers are ImGui window flags.
    -- TODO: Possibly make them variables?
    imgui_begin_window("crypto_window", 1 | 2 | 4 | 32)
    if page_number == 0 then
        imgui_push_font(0)

        imgui_text("Crypto App\n")

        imgui_pop_font()
        imgui_push_font(1)

        imgui_text("dope price checker\n")
        imgui_separator()
        imgui_spacing()

        if imgui_button("start") then
            set_page_number(1)
        end

        imgui_pop_font()

    elseif page_number == 1 then
        imgui_push_font(0)
        imgui_text("Home")
        imgui_separator()
        imgui_spacing()
        imgui_pop_font()

        imgui_push_font(1)
        if imgui_button("back") then
            set_page_number(0)
        end
        imgui_pop_font()
    end
    imgui_end_window()
end
