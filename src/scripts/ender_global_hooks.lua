function on_global_create ()
    debug_print_raw("Hello from Lua.\n")
end

function on_global_destroy ()
end

function on_global_process_events ()
end

-- Called when every lua_window renders a frame.
function on_global_render_frame ()
    if imgui_begin_window("lua window") then
        imgui_text("Hello fom Lua!")
        if imgui_button("click me") then
            debug_print_raw("Button clicked!\n")
        end
        imgui_end_window()
    end
end
