#include <bsoncxx/json.hpp>
#include <iostream>
#include "directives.hpp"
#include "client.hpp"
#include "database.hpp"
#include "collection.hpp"
#include "filter.hpp"


class SubClass {
    public:
        std::string subName = "sub class";
        BSON_SERIALIZEABLE(SubClass, subName)
        SubClass() = default;
};

class Example : public MongODM::Collection<Example> {
    public:
        int number = 5;
        std::string name = "aboba";
        bool test = false;
        std::vector<double> arr = {1.5, 1.9, 2.0};
        SubClass elem = SubClass();
        BSON_SERIALIZEABLE(Example, number, test, name, arr, elem)
        Example() = default;
};


int main() {
  auto client = MongODM::Client("mongodb://localhost:27017");
  auto db = client.database("test");
  auto col = db.collection<Example>("Example");
  auto res = true;
  for(int i=0; i< 1000000; ++i) {
    Example ex{};
    ex.number = i;
    ex.test = i%2 ? true :  false;
    res = ex.from_bson(ex.to_bson()).test;
  }
  std::cout << res;
  return 0;
}
