#pragma once
#include <string>
#include <vector>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace ender {
    class process {
    public:
        process(std::wstring_view process_name, DWORD process_id)
            : m_name(process_name), m_handle(nullptr), m_pid(process_id) {
        }

        void open_process();
        bool is_process_open() const;

        std::wstring_view get_name() const;

    private:
        std::wstring m_name;
        HANDLE m_handle;
        DWORD m_pid;
    };

    inline void process::open_process() {
        m_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, m_pid);
    }

    inline bool process::is_process_open() const {
        return m_handle != nullptr && m_handle != INVALID_HANDLE_VALUE;
    }

    inline std::wstring_view process::get_name() const {
        return m_name;
    }

    /**
     * @brief Get a list of all system processes.
     *
     * @return A vector of ender::process objects representing the system processes.
     */
    std::vector<process> get_system_processes();
}  // namespace ender
