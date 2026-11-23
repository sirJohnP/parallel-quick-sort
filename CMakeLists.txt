cmake_minimum_required(VERSION 3.25)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate the compilation database for clangd")

project(parallel_quick_sort LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
    ParlayLib
    URL https://codeload.github.com/cmuparlay/parlaylib/tar.gz/51017699dcc421f80479cdb238d3092233ad0d26
    URL_HASH SHA256=4f2fc759c1ca474db2f45585ee2300997a128d48fba79bf47aaa579aa8858ee8
    TLS_VERIFY TRUE
    SYSTEM
)
FetchContent_MakeAvailable(ParlayLib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")

add_subdirectory(app)
