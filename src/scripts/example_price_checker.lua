function crypto_on_create ()
end

function crypto_on_destroy ()
end

function crypto_on_process_events ()
end

-- Called when every lua_window renders a frame.
function crypto_on_render_imgui(page_number)
    if page_number == 0 then
        imgui_begin_window("start window")

        imgui_text("Crypto App")
        if imgui_button("start") then
            set_page_number(1)
        end

        imgui_end_window()
    elseif page_number == 1 then
        imgui_begin_window("main window")

        imgui_text("Home")
        if imgui_button("back") then
            set_page_number(0)
        end

        imgui_end_window()
    end
end
