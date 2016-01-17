#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <functional>
#include <memory>
#include <algorithm>
#include <boost/random.hpp>

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

        template<typename T>
        std::vector<std::reference_wrapper<T>> to_reference_array(const std::vector<std::unique_ptr<T>>& ptr_array)
        {
            std::vector<std::reference_wrapper<T>> out;
            out.reserve(ptr_array.size());
            for(auto& t_ptr : ptr_array)
            {
                out.push_back(std::ref(*t_ptr));
            }
            return out;
        }

        template<typename T>
        const std::vector<std::reference_wrapper<const T>> to_reference_array_const(const std::vector<std::unique_ptr<T>>& ptr_array)
        {
            std::vector<std::reference_wrapper<const T>> out;
            out.reserve(ptr_array.size());
            for(auto& t_ptr : ptr_array)
            {
                out.push_back(std::ref(static_cast<const T&>(*t_ptr)));
            }
            return out;
        }

        template<typename T>
        std::vector<T> concat(const std::vector<T>& a, const std::vector<T>& b)
        {
            std::vector<T> out;
            out.insert(out.end(), a.begin(), a.end());
            out.insert(out.end(), b.begin(), b.end());
            return out;
        }

        template<typename T>
        std::vector<T> concat(const std::vector<T>& a, const std::vector<T>& b, const std::vector<T>& c)
        {
            std::vector<T> out;
            out.insert(out.end(), a.begin(), a.end());
            out.insert(out.end(), b.begin(), b.end());
            out.insert(out.end(), c.begin(), c.end());
            return out;
        }

        template<typename T>
        std::vector<T> concat(const std::vector<T>& a, const std::vector<T>& b, const std::vector<T>& c, const std::vector<T>& d)
        {
            std::vector<T> out;
            out.insert(out.end(), a.begin(), a.end());
            out.insert(out.end(), b.begin(), b.end());
            out.insert(out.end(), c.begin(), c.end());
            out.insert(out.end(), d.begin(), d.end());
            return out;
        }

        template<typename T, typename RNG>
        void shuffle(std::vector<T>& items, RNG& rng)
        {
            boost::random::uniform_real_distribution<> distribution(0, 1);
            for(int i = items.size() - 1; i > 0; i--)
            {
                int j = std::round(distribution(rng) * i);
                std::swap(items[j], items[i]);
            }
        }

        template<typename T, typename RNG>
        void shuffle_group(std::vector<T>& items, RNG& rng, std::size_t group_size)
        {
            boost::random::uniform_real_distribution<> distribution(0, 1);
            for(int i = std::floor(items.size() / group_size) - 1; i > 0; i--)
            {
                int from = i * group_size;
                int to = std::round(distribution(rng) * i) * group_size;

                for(std::size_t j = 0; j < group_size; j++)
                {
                    std::swap(items[from + j], items[to + j]);
                }
            }
        }
    }
}

#endif // UTILS_H
