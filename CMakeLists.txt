#====================================================
# dependencies:
# - mongodb
# - mongocxx
# - bsoncxx
#====================================================

cmake_minimum_required(VERSION 3.13)
project(mongodbODM)
set(CMAKE_CXX_STANDARD 20)

option(BUILD_TESTS "build with tests" OFF)
set(TEST_TARGETS converter)

find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)

set(INCROOT ${PROJECT_SOURCE_DIR}/include/)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(BUILD_TESTS)
    set(CURRENT_TARGETS ${TEST_TARGETS})
    add_subdirectory(lib/googletest)
else()
    set(CURRENT_TARGETS main)
    add_executable(${CURRENT_TARGETS}
      src/main.cpp
      include/type_traits.hpp
      include/serializer.hpp
      include/deserializer.hpp
      include/directives.hpp
      include/collection.hpp
      include/database.hpp
      include/client.hpp
      include/filter.hpp)
endif()

foreach(CURRENT_TARGET ${CURRENT_TARGETS})
    if(BUILD_TESTS)
        add_executable(${CURRENT_TARGET}
          test/test_${CURRENT_TARGET}.cpp
          include/type_traits.hpp
          include/serializer.hpp
          include/deserializer.hpp
          include/directives.hpp
          include/collection.hpp
          include/database.hpp
          include/client.hpp
          include/filter.hpp
        )
        target_include_directories(${CURRENT_TARGET} PUBLIC lib/googletest/googletest/include)
        target_link_libraries(${CURRENT_TARGET} PUBLIC gtest)
        add_custom_command(TARGET ${CURRENT_TARGET} POST_BUILD COMMAND ${PROJECT_BINARY_DIR}/${CURRENT_TARGET})
    endif()

    target_include_directories(${CURRENT_TARGET}
                               PUBLIC
                                ${LIBMONGOCXX_LIBRARIES}
                                ${LIBBSONCXX_LIBRARIES}
                                ${LIBMONGOCXX_INCLUDE_DIRS}
                                ${LIBBSONCXX_INCLUDE_DIRS}
                                ${INCROOT})



    target_link_libraries(${CURRENT_TARGET}
                          PUBLIC
                           ${LIBBSONCXX_LIBRARIES}
                           ${LIBMONGOCXX_LIBRARIES})


endforeach(CURRENT_TARGET)
