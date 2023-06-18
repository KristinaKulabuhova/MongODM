#include <gtest/gtest.h>
#include <bsoncxx/json.hpp>
//#include "converting.hpp"

#include <string>
#include <vector>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/types.hpp>


class Example {
    public:
        int number = 5;
        std::string name = "aboba";
        bool test = false;
        BSON_SERIALIZEABLE(Example, number, name, test)
        Example() = default;
};

TEST(simpleTypes, serializeHelper)
{
    Example ex;
    //std::cout << bsoncxx::to_json(serializer.serialize(number).view());
    EXPECT_EQ(bsoncxx::to_json(ex.to_bson().view()), "{ number : 5 }");
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


// class Example {
//     public: 
//       std::string id = "1";
//       int count = 20;
//       std::vector<char> arr = {'a', 'f', 'x', 'q'};
// };

//   bsoncxx::document::value serialization(Example ex) {
//     bsoncxx::builder::basic::document basic_builder{};

//     basic_builder.append(kvp("id", ex.id));
//     basic_builder.append(kvp("count", ex.count));
//     auto array_builder = bsoncxx::builder::basic::array{};
//     for (const auto& element : ex.arr) {
//         array_builder.append(element);
//     }
//     basic_builder.append(kvp("arr", array_builder.extract()));

//     return basic_builder.extract();
//   }

// int main() {
//   Example ex;
//   bsoncxx::document::value doc = serialization(ex);
//   std::cout << bsoncxx::to_json(doc.view());
// }