function on_global_window_create ()
    debug_print_raw("Lua initialized.\n")
end

function on_global_window_destroy ()
end

function on_global_window_process_events ()
end

-- Called when every lua_window renders a frame.
function on_global_window_render_frame ()
    imgui_begin_window("lua window")
    imgui_text("Hello fom Lua!")
    imgui_end_window()
end
