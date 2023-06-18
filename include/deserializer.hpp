#pragma once

#include <cassert>
#include <string>
#include <vector>

#include <bsoncxx/document/view.hpp>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/types/bson_value/view.hpp>

#include "type_traits.hpp"

namespace MongODM {

template<typename T>
class Deserializer {
public:
    // Deserialize the given bson document to an object
    static T deserialize(const bsoncxx::document::view& view, const std::string key) {
        T obj;
        deserializeHelper(view[key].get_value(), obj);
        return obj;
    }

private:
    // Helper function to deserialize an object
    template<typename U>
    static typename std::enable_if<std::is_arithmetic<U>::value>::type
    deserializeHelper(const bsoncxx::types::bson_value::view& view, U& value) {
        static_assert(std::is_same_v<U, int32_t> || std::is_same_v<U, int64_t> || std::is_same_v<U, double> || std::is_same_v<U, bool>,
                    "U must be either int32_t, int64_t, or double");
        if constexpr (std::is_same_v<U, int32_t>) {
            value = static_cast<U>(view.get_int32());
        } else if constexpr (std::is_same_v<U, int64_t>) {
            value = static_cast<U>(view.get_int64());
        } else if constexpr (std::is_same_v<U, double>) {
            value = static_cast<U>(view.get_double());
        } else if constexpr (std::is_same_v<U, bool>) {
            value = static_cast<U>(view.get_bool());
        }
    }
    template<typename U>
    static typename std::enable_if<std::is_same<U, std::string>::value>::type
    deserializeHelper(const bsoncxx::types::bson_value::view& view, U& value) {
        value = view.get_string().value.to_string();
    }


    template<typename U>
    static typename std::enable_if<std::is_class<U>::value && !std::is_same<U, std::string>::value && !is_specialization<U, std::vector>::value>::type
    deserializeHelper(const bsoncxx::types::bson_value::view& view, U& value) {
        auto doc = view.get_document();
        value = U::from_bson(doc.view());
    }

    template<typename U>
    static typename std::enable_if<is_specialization<U, std::vector>{}>::type
    deserializeHelper(const bsoncxx::types::bson_value::view& view, U& resVector) {
        bsoncxx::array::view bson_array = view.get_array();
        for (const auto& element : bson_array) {
            typename U::value_type tmpValue;
            deserializeHelper(element.get_value(), tmpValue);
            resVector.push_back(tmpValue);
        }
    }

    template<typename U>
    static typename std::enable_if<!std::is_arithmetic<U>::value && !std::is_same<U, std::string>::value && !std::is_class<U>::value && !std::is_array<U>::value>::type
    deserializeHelper(const bsoncxx::types::bson_value::view& view, const std::string& key, U& value) {
        assert(false);
    }
};

}
