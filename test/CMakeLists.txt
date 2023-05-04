# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(orderbookTests LANGUAGES CXX)

# ---- Tests ----

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/source)

add_executable(utils_test source/test_utils.cpp ${SOURCE_DIR}/utils.cpp)
target_include_directories(utils_test PUBLIC ${CMAKE_SOURCE_DIR}/source)

add_executable(limit_order_test source/test_limit_order.cpp
								${SOURCE_DIR}/utils.cpp
								${SOURCE_DIR}/trade.cpp
								${SOURCE_DIR}/order.cpp
								${SOURCE_DIR}/limit_order.cpp
								${SOURCE_DIR}/matching_engine.cpp
								${SOURCE_DIR}/order_book.cpp)
target_include_directories(limit_order_test PUBLIC ${CMAKE_SOURCE_DIR}/source)

add_executable(iceberg_order_test source/test_iceberg_order.cpp
								${SOURCE_DIR}/utils.cpp
								${SOURCE_DIR}/trade.cpp
								${SOURCE_DIR}/order.cpp
								${SOURCE_DIR}/limit_order.cpp
								${SOURCE_DIR}/iceberg_order.cpp
								${SOURCE_DIR}/matching_engine.cpp
								${SOURCE_DIR}/order_book.cpp)
target_include_directories(iceberg_order_test PUBLIC ${CMAKE_SOURCE_DIR}/source)

add_executable(order_book_test source/test_order_book.cpp
								${SOURCE_DIR}/utils.cpp
								${SOURCE_DIR}/trade.cpp
								${SOURCE_DIR}/order.cpp
								${SOURCE_DIR}/limit_order.cpp
								${SOURCE_DIR}/iceberg_order.cpp
								${SOURCE_DIR}/matching_engine.cpp
								${SOURCE_DIR}/order_book.cpp)
target_include_directories(order_book_test PUBLIC ${CMAKE_SOURCE_DIR}/source)


# add_test
add_test(NAME utils_test COMMAND utils_test)
add_test(NAME limit_order_test COMMAND limit_order_test)
add_test(NAME iceberg_order_test COMMAND iceberg_order_test)
add_test(NAME order_book_test COMMAND order_book_test)

# ---- End-of-file commands ----
