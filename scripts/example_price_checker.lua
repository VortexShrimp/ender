function crypto_on_create ()
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
    imgui_set_next_window_position_relative(0, 0)
    imgui_set_next_window_size_to_client_size()

    -- The random numbers are ImGui window flags.
    -- TODO: Possibly make them variables?
    imgui_begin_window("crypto_window", 1 | 2 | 32)
    -- Home page.
    if page_number == 0 then
        imgui_push_font(0)
        imgui_text_centered_x("Welcome")
        imgui_pop_font()

        imgui_push_font(1)
        imgui_text("Enter a coin ID below")
        -- Handle the iput through the C++ class.
        imgui_coin_id_input("coin id")
        if imgui_button("begin") then
            set_page_number(1)
            create_coin_table()
            get_and_update_current_coin()
        end
        imgui_pop_font()
    -- Bitcoin page.
    elseif page_number == 1 then
        if current_coin ~= nil then
            imgui_push_font(0)
            imgui_text_centered_x(current_coin.name .. "\n")
            imgui_pop_font()

            imgui_push_font(1)
            imgui_text_centered_x("rank " .. current_coin.rank .. "\n")
            imgui_pop_font()

            imgui_push_font(1)
            imgui_separator()

        -- Display the coin info.
            imgui_text("Price $" .. current_coin.price .. "\n")
            imgui_text("1h " .. current_coin.change_1h .. "\n")
            imgui_text("24h " .. current_coin.change_24h .. "\n")
            imgui_text("7d " .. current_coin.change_7d .. "\n")

            imgui_separator()
            imgui_coin_id_input("coin id")
            if imgui_button("refresh") then
                get_and_update_current_coin()
            end
            imgui_pop_font()
        end
    end
    imgui_end_window()
end
