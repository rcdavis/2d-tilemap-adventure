project(Game)

add_executable(Game
    src/Main.cpp
)

target_include_directories(Game PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
