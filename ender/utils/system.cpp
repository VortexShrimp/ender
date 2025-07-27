#include "system.hpp"

#include "console.hpp"

#include <tlhelp32.h>

auto ender::get_system_processes() -> std::vector<process> {
    std::vector<process> process_list = {};

    PROCESSENTRY32W entry = {};
    entry.dwSize = sizeof(decltype(entry));

    const HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        debug_print("Failed to get valid snapshot form ToolHelp32.\n");
        return process_list;
    }

    if (Process32FirstW(snapshot, &entry) == TRUE) {
        do {
            process proc(entry.szExeFile, entry.th32ProcessID);
            process_list.emplace_back(proc);
        } while (Process32NextW(snapshot, &entry) == TRUE);
    }

    CloseHandle(snapshot);
    return process_list;
}
