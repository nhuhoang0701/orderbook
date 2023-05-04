cmake_minimum_required(VERSION 3.14)

project(
    orderbook
    VERSION 0.1.0
    DESCRIPTION "Short description"
    HOMEPAGE_URL "https://example.com/"
    LANGUAGES CXX
)

# ---- Declare library ----


# ---- Declare executable ----
# Set the source directory
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.cpp)

add_executable(orderbook_exe ${SOURCES})
add_executable(orderbook::exe ALIAS orderbook_exe)

set_property(TARGET orderbook_exe PROPERTY OUTPUT_NAME orderbook)

target_compile_features(orderbook_exe PRIVATE cxx_std_17)

# enable_testing()
# add_subdirectory(test)
