#pragma once
#include <type_traits>
#include <string_view>
#include <algorithm>

#define GET_SERIALIZEABLE_FLAG_NAME(CLASSNAME, NAME) CLASSNAME::NAME##_IS_SERIALIAZEABLE

template <class T, template <class...> class Template>
struct is_specialization : std::false_type {};

template <template <class...> class Template, class... Args>
struct is_specialization<Template<Args...>, Template> : std::true_type {};
