#pragma once

#include <mongocxx/cursor.hpp>

namespace MongODM {
template <typename Mapping>
class DecodingIterator {
  private:
      mongocxx::cursor::iterator it_;
  public:
    DecodingIterator<Mapping>(mongocxx::cursor::iterator it) : it_(it) {}

    const Mapping& operator*() const {
      return Mapping::from_bson(*it_);
    }

    DecodingIterator<Mapping>& operator++() {
      ++it_;
      return this;
    }

    void operator++(int) {
      it_++;
    }

    bool operator==(const DecodingIterator<Mapping>& other) {
      return this->it_ == other.it_;
    }

    bool operator!=(const DecodingIterator<Mapping>& other) {
      return !(this == other);
    }

    ~DecodingIterator<Mapping>() = default;
};
};
