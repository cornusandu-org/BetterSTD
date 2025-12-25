#pragma once

namespace bstd {
    class fixed_ptr_base {
        public:
            virtual ~fixed_ptr_base() = default;
            virtual void* get_void_ptr();
    };

    template<typename T>
    class fixed_ptr : fixed_ptr_base {
        public:
            fixed_ptr(T* ptr);

            ~fixed_ptr();

            T& operator*();
            const T& operator*() const;
            T* operator->();
            const T* operator->() const;

            T get() const;
            T* get_ptr();
            void* get_void_ptr();
            const T* get_ptr() const;

            char is_null() const;

        private:
           T* ptr;
    };
}
