#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/read_concern.hpp>
#include <mongocxx/read_preference.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/write_concern.hpp>

#include "database.hpp"
#include "directives.hpp"
#include "cursor.hpp"

namespace MongODM {

static mongocxx::instance instance;

class DBDescription {
  public:
    std::string name;
    int64_t sizeOnDisk;
    bool empty;

    BSON_SERIALIZEABLE(DBDescription, name, sizeOnDisk, empty);
};

class Client {
public:
    Client(const std::string& uri) : client_(mongocxx::uri(uri)) {}

    Database database(const std::string& name) const {
      mongocxx::database raw_db = client_[name];
      return Database(raw_db);
    }

    std::vector<std::string> list_database_names() const {
        return client_.list_database_names();
    }

    void drop_database(const std::string& name) {
        client_.database(name).drop();
    }

    std::string uri() const {
      return client_.uri().to_string();
    }

    void reset() {
      client_.reset();
    }

    Cursor<DBDescription> list_databases() const {
      mongocxx::cursor raw_cursor = client_.list_databases();
      return Cursor<DBDescription>(raw_cursor);
    }

private:
    mongocxx::client client_;
};

};
