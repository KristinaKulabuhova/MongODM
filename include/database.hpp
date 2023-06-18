#pragma once
#include <string>
#include <vector>

#include <mongocxx/database.hpp>

#include <bsoncxx/document/value.hpp>
#include <bsoncxx/document/view_or_value.hpp>

#include "collection.hpp"
#include "cursor.hpp"
#include "directives.hpp"
#include "filter.hpp"

namespace MongODM {

class CollectionInfo {
  public:
    bool readOnly;
    std::string uuid;
    BSON_SERIALIZEABLE(CollectionInfo, readOnly, uuid);
};

// TODO: add support of options and idIndex
class CollectionDescription {
  public:
    std::string name;
    std::string type;
    CollectionInfo info;
    BSON_SERIALIZEABLE(CollectionDescription, name, type, info);
};

class Database {
public:
    Database(mongocxx::database& db) :
        db_(db) {}

    template <typename T>
    Collection<T> collection(const std::string& name) {
        auto col = Collection<T>{};
        col.set_collection(db_[name]);
        return col;
    }

    // TODO add options support
    template<typename T>
    Collection<T> create_collection(const std::string& name) {
      db_.create_collection(name);
      return collection<T>(name);
    }

    bool has_collection(const std::string& name) {
      return db_.has_collection(name);
    }

    std::string name() {
      return std::string(db_.name());
    }

    void drop() {
      db_.drop();
    }

    Cursor<CollectionDescription> list_collections(Filter<CollectionDescription>& filter) {
      mongocxx::cursor raw_cursor = db_.list_collections(filter.extract().view());
      return Cursor<CollectionDescription>(raw_cursor);
    }

    std::vector<std::string> list_collection_names(Filter<CollectionDescription>& filter) {
      return db_.list_collection_names(filter.extract().view());
    }

    mongocxx::database get_impl() {
      return db_;
    }

private:
    mongocxx::database db_;
};

};
