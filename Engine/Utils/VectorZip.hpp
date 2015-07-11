#include <vector>
#include <tuple>
#include <algorithm>
#include <type_traits>

namespace zip
{
    template<class T>
    using Invoke = typename T::type;

    template<class T>
    using Unqualified = Invoke < std::remove_cv<Invoke<std::remove_reference<T>>> > ;

    template<class T>
    using ValueType = typename Unqualified<T>::value_type;

    template<class T>
    T const& forward_index(std::vector<T> const& v, unsigned i)
    {
        return v[i];
    }

    template<class T>
    T&& forward_index(std::vector<T>&& v, unsigned i)
    {
        return std::move(v[i]);
    }

    template<class... Vs>
    std::vector<std::tuple<ValueType<Vs>...>> zip(Vs&&... vs)
    {
        const auto lo = std::min({ vs.size()... });
        std::vector<std::tuple<ValueType<Vs>...>> v;
        v.reserve(lo);
        for(unsigned i = 0; i < lo; ++i)
            v.emplace_back(forward_index(std::forward<Vs>(vs), i)...);
        return v;
    }
}
