-- Called when Lua state has been initialized.
function on_create ()
    -- Custom fonts can be used with imgui_push_font and imgui_pop_font
    -- Fonts are indexed by order of being loaded. The first font is 0,
    -- the sencond font is 1, and so on...
    imgui_add_font_from_file_ttf("C:\\Windows\\Fonts\\bahnschrift.ttf", 36)
    imgui_add_font_from_file_ttf("C:\\Windows\\Fonts\\bahnschrift.ttf", 16)
    imgui_add_font_from_file_ttf("C:\\Windows\\Fonts\\tahomabd.ttf", 28)
    imgui_add_font_from_file_ttf("C:\\Windows\\Fonts\\tahoma.ttf", 18)

    imgui_set_defualt_font(1)

    -- Create and update the table tht will hold the info of each coin from the API.
    create_coin_table()
    update_coin_table()
end

function on_destroy ()
end

function on_process_events ()
end

-- Page number is stored in the C++ class.
function on_render_imgui(page_number)
    imgui_set_next_window_position_relative(0, 0)
    -- Make the ImGui window the same size the os window.
    imgui_set_next_window_size_to_client_size()

    -- Create the window (with flags).
    imgui_begin_window("crypto_window",
        imgui_window.no_title | imgui_window.no_resize | imgui_window.no_move | imgui_window.no_collapse)
    -- Home page.
    if page_number == 0 then
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
        -- Display the ImGui::InputInt for the coin ID.
        imgui_coin_id_input("coin id")
        if imgui_button("Update Coin") then
            update_coin_table()
        end
        imgui_pop_font()
    elseif page_number == 1 then
        -- Currently nothing on this page.
        -- Use set_page_number(number) to change the page.
    end
    imgui_end_window()
end
