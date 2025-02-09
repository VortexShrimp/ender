function crypto_on_create ()
    -- Handle the request & json parsing in C++.
    crypto_get_and_parse_global_response()

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
        imgui_separator()
        imgui_spacing()
        imgui_pop_font()

        -- Display all the global endpoint data.
        imgui_push_font(1)
        imgui_text("Coins Count [ " .. global_coins_count .. " ]\n")
        imgui_text("Active Markets [ " .. global_active_markets .. " ]\n")
        imgui_spacing()
        imgui_text("BTC Dominance [ " .. global_bitcoin_dominance .. " ]\n")
        imgui_text("ETH Dominance [ " .. global_etherium_dominance .. " ]\n")
        imgui_spacing()
        imgui_text("Average Change [ " .. global_change_percent .. " ]\n")


        if imgui_button("back") then
            set_page_number(0)
        end
        imgui_pop_font()
    end
    imgui_end_window()
end
