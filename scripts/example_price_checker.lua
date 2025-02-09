function crypto_on_create ()
    -- Handle the request & json parsing in C++.
    crypto_get_and_parse_api()

    -- Add fonts.
    imgui_add_font_from_file_ttf("C:\\Windows\\Fonts\\bahnschrift.ttf", 36)
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
    -- Start page.
    if page_number == 0 then
        imgui_push_font(0)
        imgui_text("Crypto App\n")

        imgui_pop_font()
        imgui_push_font(1)

        imgui_text("price checker\n")
        imgui_separator()
        imgui_spacing()

        if imgui_button("start") then
            set_page_number(1)
        end
        imgui_pop_font()
    -- Home page.
    elseif page_number == 1 then
        imgui_push_font(0)
        imgui_text("Home")
        imgui_pop_font()

        -- Display all the global endpoint data.
        imgui_push_font(1)
        imgui_text("There are " .. global_coins_count .." coins available on\n" .. global_active_markets .. " markets.\n")
        imgui_text("The market consists of " .. global_bitcoin_dominance .. " percent\nBTC and " .. global_etherium_dominance .. " percent ETC.\n")
        imgui_text("The volume changed by " .. global_volume_change .. " with\nan average change of " .. global_change_percent .. ".\n")

        imgui_separator()
        imgui_spacing()

        if imgui_button("back") then
            set_page_number(0)
        end
        imgui_pop_font()
    end
    imgui_end_window()
end
