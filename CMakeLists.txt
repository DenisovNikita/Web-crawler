cmake_minimum_required(VERSION 3.21)
project(Web_crawler)

set(CMAKE_CXX_STANDARD 17)

find_package(CURL)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG beb9e98806bb84bcc130a2cebfbcbbc6ce62b335) # The commit hash for 1.7.2. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

add_executable(Web_crawler main.cpp)

target_link_libraries(Web_crawler PRIVATE cpr::cpr)
