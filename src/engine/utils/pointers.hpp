#pragma once

namespace ender {
    /**
     * @brief Pointer with single ownership.
     * @tparam T Pointer type.
     */
    template <class T>
    class unique_pointer {
    public:
        explicit unique_pointer(T* pointer) noexcept : m_pointer(pointer) {
        }
        unique_pointer(unique_pointer&& other) noexcept : m_pointer(other.release()) {
        }
        unique_pointer(const unique_pointer&) = delete;  // No copy construction.

        ~unique_pointer();

        unique_pointer& operator=(const unique_pointer&) = delete;  // No copy assignment.
        unique_pointer& operator=(unique_pointer&& other) noexcept;
        T& operator*() const;
        T* operator->() const noexcept;
        explicit operator bool() const noexcept;

        /**
         * @brief Release ownership of the pointer.
         * @return
         */
        T* release() noexcept;

        /**
         * @brief Reset this pointer and set it to a new raw pointer.
         * @param pointer
         */
        void reset(T* pointer) noexcept;

        /**
         * @brief Access the raw pointer stored in the class.
         * @return
         */
        T* get() const noexcept;

    private:
        T* m_pointer;
    };

    template <class T>
    inline unique_pointer<T>::~unique_pointer() {
        reset(nullptr);
    }

    template <class T>
    inline T* unique_pointer<T>::release() noexcept {
        T* temp = m_pointer;
        m_pointer = nullptr;
        return temp;
    }

    template <class T>
    inline void unique_pointer<T>::reset(T* pointer) noexcept {
        if (m_pointer != pointer) {
            delete m_pointer;
            m_pointer = pointer;
        }
    }

    template <class T>
    inline T* unique_pointer<T>::get() const noexcept {
        return m_pointer;
    }

    template <class T>
    inline unique_pointer<T>& unique_pointer<T>::operator=(unique_pointer&& other) noexcept {
        if (this != &other) {
            reset(other.release());
        }

        return *this;
    }

    template <class T>
    inline T& unique_pointer<T>::operator*() const {
        // NOTE: Add nullptr dereference check?
        return *m_pointer;
    }

    template <class T>
    inline T* unique_pointer<T>::operator->() const noexcept {
        return m_pointer;
    }

    template <class T>
    inline unique_pointer<T>::operator bool() const noexcept {
        return m_pointer != nullptr;
    }

    /**
     * @brief Helper function for creating a unique pointer.
     * @tparam T
     * @tparam ...Args
     * @param ...args
     * @return
     */
    template <class T, class... Args>
    unique_pointer<T> make_unique_pointer(Args&&... args) {
        return unique_pointer<T>(new T(std::forward<Args>(args)...));
    }
}  // namespace ender
