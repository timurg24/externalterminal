cmake_minimum_required(VERSION 3.31)
project(ExternalTerminalProject)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# Raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
FetchContent_MakeAvailable(raylib)

# Lua
FetchContent_Declare(
    lua
    GIT_REPOSITORY https://github.com/walterschell/Lua.git
    GIT_TAG v5.4.7
)
FetchContent_MakeAvailable(lua)

# Sol2
FetchContent_Declare(
    sol2
    GIT_REPOSITORY https://github.com/ThePhD/sol2.git
    GIT_TAG v3.3.0
)
FetchContent_MakeAvailable(sol2)

# JSON
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

# asio
FetchContent_Declare(
    asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG asio-1-38-0
)
FetchContent_MakeAvailable(asio)


add_executable(ExternalTerminal
    src/main.cpp
    src/Terminal.cpp
    src/Profile.cpp
    src/Shell.cpp
    src/Utils.cpp
)

target_link_libraries(ExternalTerminal PRIVATE
    raylib
    lua_static
    nlohmann_json
)

target_include_directories(ExternalTerminal PRIVATE
    ${sol2_SOURCE_DIR}/include
    ${asio_SOURCE_DIR}/asio/include
)

target_compile_definitions(ExternalTerminal PRIVATE
    ASIO_STANDALONE
)

if(WIN32)
    target_link_libraries(ExternalTerminal PRIVATE ws2_32 wsock32)
endif()