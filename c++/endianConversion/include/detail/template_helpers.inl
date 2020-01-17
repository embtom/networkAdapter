namespace EtEndian {
namespace detail {

template <typename F, typename... Args, std::size_t... Idx>
void for_tuple_impl(F&& f, const std::tuple<Args...>& tuple, std::index_sequence<Idx...>)
{
    (f(std::get<Idx>(tuple)), ...);
}

template <typename F, typename... Args>
void for_tuple(F&& f, const std::tuple<Args...>& tuple)
{
    for_tuple_impl(f, tuple, std::index_sequence_for<Args...>{});
}

template <typename F>
void for_tuple(F&& /* f */, const std::tuple<>& /* tuple */)
{ /* do nothing */ }

} // end of namespace detail
} // end of namespace meta
