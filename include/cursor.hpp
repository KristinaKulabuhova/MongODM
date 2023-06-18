#pragma once

#include <mongocxx/cursor.hpp>

#include "decoding_iterator.hpp"

namespace MongODM {
template <typename Mapping>
class Cursor {
  public:

  Cursor(mongocxx::cursor& cursor) : cursor_(std::move(cursor)) {}

  ~Cursor() = default;

  DecodingIterator<Mapping> begin() {
    return DecodingIterator<Mapping>(cursor_.begin());
  }

  DecodingIterator<Mapping> end() {
    return DecodingIterator<Mapping>(cursor_.begin());
  }

  private:
  mongocxx::cursor cursor_;
};

};
