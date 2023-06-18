#pragma once
#include <string_view>
#include <array>

#include "serializer.hpp"
#include "deserializer.hpp"
#include <bsoncxx/builder/stream/array.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>


using bsoncxx::document::view;
using bsoncxx::builder::concatenate;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;


#define PARENS ()

// Rescan macro tokens 256 times
#define EXPAND(arg) EXPAND1(EXPAND1(EXPAND1(EXPAND1(arg))))
#define EXPAND1(arg) EXPAND2(EXPAND2(EXPAND2(EXPAND2(arg))))
#define EXPAND2(arg) EXPAND3(EXPAND3(EXPAND3(EXPAND3(arg))))
#define EXPAND3(arg) EXPAND4(EXPAND4(EXPAND4(EXPAND4(arg))))
#define EXPAND4(arg) arg

#define FOR_EACH(macro, ...)                                    \
  __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...)                         \
  macro(a1)                                                     \
  __VA_OPT__(FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER


#define BSON_CONCAT(NAME) << concatenate(MongODM::Serializer<decltype(NAME)>::serialize(NAME, #NAME).view())

#define BSON_GET(NAME) NAME = MongODM::Deserializer<decltype(NAME)>::deserialize(view, #NAME);

#define SET_SERIALIZEABLE_FLAG(NAME) static constexpr bool NAME##_IS_SERIALIAZEABLE = true;

#define BSON_SERIALIZEABLE(CLASSNAME, ...) \
bsoncxx::document::value to_bson() const {\
    return document{} FOR_EACH(BSON_CONCAT, __VA_ARGS__) << finalize; \
} \
\
void from_bson_impl(const bsoncxx::document::view& view) {\
    FOR_EACH(BSON_GET, __VA_ARGS__) \
} \
\
static CLASSNAME from_bson(const bsoncxx::document::view& view) { \
    auto obj = CLASSNAME (); \
    obj.from_bson_impl(view);\
    return obj;\
}\
\
FOR_EACH(SET_SERIALIZEABLE_FLAG, __VA_ARGS__)

