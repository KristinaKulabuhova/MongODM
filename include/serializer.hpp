// A generic serializer class that takes any C++ class and serializes it to a bson document
#pragma once
#include <string>
#include <vector>

#include <bsoncxx/document/value.hpp>
#include <bsoncxx/document/view.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/impl.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>

#include "type_traits.hpp"

using bsoncxx::builder::basic::kvp;

namespace MongODM {

template<typename T>
class Serializer {
public:
    // Serialize the given object to a bson document
    static bsoncxx::document::value serialize(const T& obj, std::string name) {
        bsoncxx::builder::basic::document builder;
        serializeHelper(builder, name, obj);
        return builder.extract();
    }
private:
    // Helper function to serialize an object
    template<typename U>
    static typename std::enable_if<std::is_arithmetic<U>::value || std::is_same<U, std::string>::value>::type
    serializeHelper(bsoncxx::builder::basic::document& builder, const std::string& key, const U& value) {
        builder.append(kvp(key, value));
    }

    template<typename U>
    static typename std::enable_if<std::is_class<U>::value && !std::is_same<U, std::string>::value && !is_specialization<U, std::vector>::value>::type
    serializeHelper(bsoncxx::builder::basic::document& builder, const std::string& key, const U& value) {
        builder.append(kvp(key, const_cast<U&>(value).to_bson()));
    }


    template<typename U>
    static typename std::enable_if<is_specialization<U, std::vector>{}>::type
    serializeHelper(bsoncxx::builder::basic::document& builder, const std::string& key, const U& value) {
        auto sub_builder = bsoncxx::builder::basic::array{};
        for (const auto& v : value) {
            // FIXME: If vector element is a serializeable object, it is necessary to call to_bson method.
            sub_builder.append(v);
        }
        builder.append(kvp(key, sub_builder));
    }

    template<typename U>
    static typename std::enable_if<!std::is_arithmetic<U>::value && !std::is_same<U, std::string>::value && !std::is_class<U>::value && !std::is_array<U>::value>::type
    serializeHelper(bsoncxx::builder::basic::document& builder, const std::string& key, const U& value) {
        assert(false);
    }
};

}
