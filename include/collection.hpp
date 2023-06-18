#pragma once

#include <cstdint>
#include <string>

#include <mongocxx/collection.hpp>
#include <mongocxx/result/delete.hpp>
#include <mongocxx/result/update.hpp>
#include <mongocxx/result/replace_one.hpp>
#include <bsoncxx/document/value.hpp>
#include <bsoncxx/document/view_or_value.hpp>
#include <bsoncxx/stdx/optional.hpp>

#include "cursor.hpp"
#include "filter.hpp"

namespace MongODM {
template <typename T>
class Collection {
private:
    mongocxx::collection collection_impl_;
public:
    Collection() = default;

    void set_collection(mongocxx::collection col) {
      collection_impl_ = col;
    }

    std::string name() const {
      return collection_impl_.name();
    }

    void insert_one(const T& document) {
        bsoncxx::document::value doc_value = document.to_bson();
        collection_impl_.insert_one(doc_value.view());
    }

    void rename(
        const std::string& new_name, bool drop_target_before_rename=false
    ) {
      collection_impl_.rename(new_name, drop_target_before_rename);
    }

    std::optional<T> find_one(Filter<Collection<T>>& filter) {
        auto result = Collection<T>::collection_impl_.find_one(filter.extract().view());
        if (!result) {
            return std::nullopt;
        }
        return T::from_bson(*result);
    }

    int64_t estimated_document_count() {
      return collection_impl_.estimated_document_count();
    }

    int64_t count_documents(Filter<Collection<T>>& filter) {
      return collection_impl_.count_documents(filter.extract().view());
    }

    Cursor<T> find(Filter<Collection<T>>& filter) {
        auto result = Collection<T>::collection_impl_.find(filter.extract().view());
        return Cursor<T>(std::move(result));

    }

    void drop() {
      collection_impl_.drop();
    }

    bsoncxx::stdx::optional<mongocxx::result::delete_result> delete_one(Filter<Collection<T>>& filter) {
      return collection_impl_.delete_one(filter.extract().view());
    }

    bsoncxx::stdx::optional<mongocxx::result::delete_result> delete_many(Filter<Collection<T>>& filter) {
      return collection_impl_.delete_many(filter.extract().view());
    }

    bsoncxx::stdx::optional<mongocxx::result::replace_one> replace_one(Filter<Collection<T>>& filter, T& replacement) {
      return collection_impl_.replace_one(filter.extract().view(), replacement.to_bson());
    }
};


}
