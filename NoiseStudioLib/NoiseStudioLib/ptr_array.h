#ifndef PTR_ARRAY_H
#define PTR_ARRAY_H

#include <array>
#include <cstddef>
#include <stdexcept>

namespace noises
{
    template<class T, std::size_t N>
    class ptr_array
    {
    public:
        ptr_array() : ptr_(nullptr) { }

        ptr_array(const T* ptr) : ptr_(ptr) { }

        ptr_array(const std::array<T, N>& array) : ptr_(&array[0]) { }

        ptr_array(const std::vector<unsigned char>& vector) : ptr_(reinterpret_cast<const T*>(&vector[0])) { }

        ptr_array(const unsigned char* ptr) : ptr_(reinterpret_cast<const T*>(ptr)) { }

        #include "ptr_array_common.hpart"

        std::array<T, N> array() const
        {
            std::array<T, N> arr;
            T* first(arr[0]);
            std::copy(ptr_, ptr_ + N, first);
            return arr;
        }

        void copy_to(std::vector<unsigned char>& vector) const
        {
            const unsigned char* raw_ptr = raw();
            vector.resize(N * sizeof(T));
            std::copy(raw_ptr, raw_ptr + (N * sizeof(T)), &vector[0]);
        }

    private:
        const T* ptr_;
    };

    template<class T>
    class ptr_array<T, 1>
    {
    public:
        ptr_array() : ptr_(nullptr) { }

        ptr_array(const T* ptr) : ptr_(ptr) { }

        ptr_array(const std::array<T, 1>& array) : ptr_(&array[0]) { }

        ptr_array(const std::vector<unsigned char>& vector) : ptr_(reinterpret_cast<const T*>(&vector[0])) { }

        ptr_array(const unsigned char* ptr) : ptr_(reinterpret_cast<const T*>(ptr)) { }

        constexpr std::size_t size() const
        {
            return 1;
        }

        #include "ptr_array_common.hpart"

        T value()
        {
            return *this->ptr();
        }

        const T value() const
        {
            return *this->ptr();
        }

        const T& value_ref() const
        {
            return *this->ptr();
        }

        operator T()
        {
            return value();
        }

        operator T&()
        {
            return value();
        }

        operator const T&() const
        {
            return *ptr_;
        }

        const T& operator*() const
        {
            return *ptr_;
        }

        std::array<T, 1> array() const
        {
            std::array<T, 1> arr;
            T* first(arr[0]);
            std::copy(ptr_, ptr_ + 1, first);
            return arr;
        }

        void copy_to(std::vector<unsigned char>& vector) const
        {
            const unsigned char* raw_ptr = raw();
            vector.resize(sizeof(T));
            std::copy(raw_ptr, raw_ptr + sizeof(T), &vector[0]);
        }

    private:
        const T* ptr_;
    };

    template<std::size_t N>
    class ptr_array<unsigned char, N>
    {
    public:
        ptr_array() : ptr_(nullptr) { }

        ptr_array(const unsigned char* ptr) : ptr_(ptr) { }

        ptr_array(const std::array<unsigned char, N>& array) : ptr_(&array[0]) { }

        ptr_array(const std::vector<unsigned char>& vector) : ptr_(&vector[0]) { }

        #define T unsigned char
        #include "ptr_array_common.hpart"
        #undef T

        std::array<unsigned char, N> array() const
        {
            std::array<unsigned char, N> arr;
            unsigned char* first(arr[0]);
            std::copy(ptr_, ptr_ + sizeof(unsigned char), first);
            return arr;
        }

        void copy_to(std::vector<unsigned char>& vector) const
        {
            vector.resize(N * sizeof(unsigned char));
            std::copy(ptr_, ptr_ + (N * sizeof(unsigned char)), &vector[0]);
        }
    private:
        const unsigned char* ptr_;
    };
}

#endif // PTR_ARRAY_H
