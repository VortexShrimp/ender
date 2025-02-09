function crypto_on_create ()
    -- Test get request from Lua.
    local response = get_request("jsonplaceholder.typicode.com", "/todos/1")
    debug_print_raw(response)
    debug_print_raw("\n")
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

    imgui_begin_window("crypto_window")
    if page_number == 0 then
        imgui_text("Crypto App")
        if imgui_button("start") then
            set_page_number(1)
        end
    elseif page_number == 1 then
        imgui_text("Home")
        if imgui_button("back") then
            set_page_number(0)
        end
    end
    imgui_end_window()
end
