find_package(Boost COMPONENTS serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${complx-tools_SOURCE_DIR}/liblc3)
set(CMAKE_SKIP_RPATH TRUE)

set(SRC_REPLAY
    lc3_replay.cpp
    BinaryStreamReader.cpp
)

add_library(
    lc3_replay
    STATIC
    ${SRC_REPLAY}
)

target_link_libraries(
    lc3_replay
    lc3
)

