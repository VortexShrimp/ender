function crypto_on_create ()
    -- Add fonts.
    imgui_add_font_from_file_ttf("C:\\Windows\\Fonts\\bahnschrift.ttf", 36)
    imgui_add_font_from_file_ttf("C:\\Windows\\Fonts\\bahnschrift.ttf", 16)
    imgui_add_font_from_file_ttf("C:\\Windows\\Fonts\\tahomabd.ttf", 28)
    imgui_add_font_from_file_ttf("C:\\Windows\\Fonts\\tahoma.ttf", 18)

    imgui_set_defualt_font(1)

    create_coin_table()
    get_and_update_current_coin()
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

    -- Create the window (with flags).
    imgui_begin_window("crypto_window",
        imgui_window.no_title | imgui_window.no_resize | imgui_window.no_move | imgui_window.no_collapse)
    -- Home page.
    if page_number == 0 then
        if current_coin ~= nil then
            imgui_push_font(2)
            imgui_text_centered_x(current_coin.name .. "\n")
            imgui_pop_font()

            imgui_push_font(1)
            imgui_text_centered_x("rank " .. current_coin.rank .. "\n")
            imgui_text_centered_x("$" .. current_coin.price .. "\n")

            imgui_pop_font()

            imgui_push_font(3)
            imgui_separator()

            -- Display the coin info.
            imgui_text("1h " .. current_coin.change_1h .. "\n")
            imgui_spacing()
            imgui_text("24h " .. current_coin.change_24h .. "\n")
            imgui_spacing()
            imgui_text("7d " .. current_coin.change_7d .. "\n")

            imgui_separator()
            imgui_coin_id_input("coin id")
            if imgui_button("Update Coin") then
                get_and_update_current_coin()
            end
            imgui_pop_font()
        end
    -- Bitcoin page.
    elseif page_number == 1 then
    end
    imgui_end_window()
end
