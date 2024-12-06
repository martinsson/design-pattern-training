cmake_minimum_required(VERSION 3.29)
Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.5.3 # or a later release
)

FetchContent_MakeAvailable(Catch2)

set(CMAKE_CXX_STANDARD 14)

project(stateKataFinalCode)
add_executable(stateKataFinalCode stateKataFinalCode.cpp)
add_test(NAME stateKataFinalCodeTest COMMAND stateKataFinalCode)
target_link_libraries(stateKataFinalCode Catch2::Catch2)

project(mediatorKataFinalCode)
add_executable(mediatorKataFinalCode mediatorKataFinalCode.cpp)
add_test(NAME mediatorKataFinalCodeTest COMMAND mediatorKataFinalCode)
target_link_libraries(mediatorKataFinalCode Catch2::Catch2)

