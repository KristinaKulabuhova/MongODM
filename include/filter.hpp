#pragma once
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/document/view.hpp>
#include <type_traits>
#include <vector>
#include <string>
#include <utility>
#include <concepts>

#include "type_traits.hpp"

using bsoncxx::builder::basic::kvp;

namespace MongODM {

/* Forward declaration. */
template <typename T> class Collection;


template <typename MappingType>
concept is_mapping = requires(MappingType, const bsoncxx::document::view& view) {
  { MappingType::from_bson(view) } -> std::same_as<MappingType>;
};

template <typename MappingType, typename DataMemberType, bool IsSerializeable>
struct PropertyProvider {
    static_assert(is_mapping<MappingType>, "Cannot take serializeable property from non-serializeable class.");
    static_assert(IsSerializeable, "Not serializeable property.");
    const DataMemberType MappingType::*property;
    const std::string_view name;
  PropertyProvider(const std::string_view& name, DataMemberType MappingType::*property) : name(name), property(property) {}
};

#define S_PROPERTY(mapping, name) MongODM::PropertyProvider<mapping, decltype(std::declval<mapping>().name), GET_SERIALIZEABLE_FLAG_NAME(mapping, name)>(#name, &mapping::name)

template <typename CollectionType>
class Filter {
  public:
  private:
    bsoncxx::builder::basic::document filter_builder{};

  public:
    template <typename DataMemberType, typename ValueType>
    Filter<CollectionType>& eq(const PropertyProvider<CollectionType, DataMemberType, true>& provider, ValueType value) {
      static_assert(std::is_convertible<DataMemberType, ValueType>{}, "Types cannot be converted to one another.");
      bsoncxx::builder::basic::document subdoc{};
      subdoc.append(kvp("$eq", value));
      filter_builder.append(kvp(std::string(provider.name), subdoc.extract()));
      return *this;
    }

    template <typename DataMemberType, typename ValueType>
    Filter<CollectionType>& lt(const PropertyProvider<CollectionType, DataMemberType, true>& provider, ValueType value) {
      static_assert(std::is_convertible<DataMemberType, ValueType>{}, "Types cannot be converted to one another.");
      bsoncxx::builder::basic::document subdoc{};
      subdoc.append(kvp("$lt", value));
      filter_builder.append(kvp(std::string(provider.name), subdoc.extract()));
      return *this;
    }

    template <typename DataMemberType, typename ValueType>
    Filter<CollectionType>& lte(const PropertyProvider<CollectionType, DataMemberType, true>& provider, ValueType value) {
      static_assert(std::is_convertible<DataMemberType, ValueType>{}, "Types cannot be converted to one another.");
      bsoncxx::builder::basic::document subdoc{};
      subdoc.append(kvp("$lte", value));
      filter_builder.append(kvp(std::string(provider.name), subdoc.extract()));
      return *this;
    }

    template <typename DataMemberType, typename ValueType>
    Filter<CollectionType>& gt(const PropertyProvider<CollectionType, DataMemberType, true>& provider, ValueType value) {
      static_assert(std::is_convertible<DataMemberType, ValueType>{}, "Types cannot be converted to one another.");
      bsoncxx::builder::basic::document subdoc{};
      subdoc.append(kvp("$gt", value));
      filter_builder.append(kvp(std::string(provider.name), subdoc.extract()));
      return *this;
    }

    template <typename DataMemberType, typename ValueType>
    Filter<CollectionType>& gte(const PropertyProvider<CollectionType, DataMemberType, true>& provider, ValueType value) {
      static_assert(std::is_convertible<DataMemberType, ValueType>{}, "Types cannot be converted to one another.");
      bsoncxx::builder::basic::document subdoc{};
      subdoc.append(kvp("$gte", value));
      filter_builder.append(kvp(std::string(provider.name), subdoc.extract()));
      return *this;
    }

    template <typename DataMemberType, typename ValueType>
    Filter<CollectionType>& ne(const PropertyProvider<CollectionType, DataMemberType, true>& provider, ValueType value) {
      static_assert(std::is_convertible<DataMemberType, ValueType>{}, "Types cannot be converted to one another.");
      bsoncxx::builder::basic::document subdoc{};
      subdoc.append(kvp("$gte", value));
      filter_builder.append(kvp(std::string(provider.name), subdoc.extract()));
      return *this;
    }

    bsoncxx::document::value extract() {
      return filter_builder.extract();
    }
};

}
