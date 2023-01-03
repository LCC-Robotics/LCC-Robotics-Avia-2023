#ifndef LCC_ROBOTICS_22_23_INCLUDE_UTILS_H_
#define LCC_ROBOTICS_22_23_INCLUDE_UTILS_H_

#include <math.h>

namespace utils {

template <typename T>
struct Range {
    T lower;
    T upper;
};

// custom c++ std
namespace ustd {
    // std::remove_reference implementation
    template <typename T>
    struct remove_reference {
        using type = T;
    };

    template <typename T>
    struct remove_reference<T&> {
        using type = T;
    };

    template <typename T>
    struct remove_reference<T&&> {
        using type = T;
    };

    // std::remove_reference_t implementation
    template <typename T>
    using remove_reference_t = typename remove_reference<T>::type;

    // std::move implementation
    template <typename T>
    constexpr inline remove_reference_t<T>&& move(T&& arg)
    {
        return static_cast<remove_reference_t<T>&&>(arg);
    }
}
} // namespace utils

#endif // LCC_ROBOTICS_22_23_INCLUDE_UTILS_H_
