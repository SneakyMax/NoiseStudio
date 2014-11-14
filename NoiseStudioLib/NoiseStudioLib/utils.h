#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <functional>
#include <memory>
#include <algorithm>

namespace noises
{
    namespace utils
    {
        template<typename T>
        void remove_by_pointer(std::vector<std::unique_ptr<T>>& list, const T* ptr)
        {
            auto it = std::find_if(list.begin(), list.end(), [ptr](std::unique_ptr<T>& item) { return item.get() == ptr; });
            if(it != list.end())
                list.erase(it);
        }

        template<typename T>
        void remove_by_pointer(std::vector<std::reference_wrapper<T>>& list, const T* ptr)
        {
            auto it = std::find_if(list.begin(), list.end(), [ptr](std::reference_wrapper<T>& item) { return &item.get() == ptr; });
            if(it != list.end())
                list.erase(it);
        }

        template<typename T>
        void remove_by_pointer(std::vector<std::reference_wrapper<const T>>& list, const T* ptr)
        {
            auto it = std::find_if(list.begin(), list.end(), [ptr](std::reference_wrapper<const T>& item) { return &item.get() == ptr; });
            if(it != list.end())
                list.erase(it);
        }
    }
}

#endif // UTILS_H
