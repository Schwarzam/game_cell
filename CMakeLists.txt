cmake_minimum_required(VERSION 3.20)
project(game_cell)

set(CMAKE_CXX_STANDARD 17)
add_executable(game_cell main.cpp src/Assets.cpp src/Assets.h src/Collision.cpp src/Collision.h src/Server.cpp src/Server.h)

if(WIN32)
    set(CMAKE_C_COMPILER c:/mingw64/bin/g++)
    set(SMFL_folder c:/SFML)

    set(SFML_ROOT ${SMFL_folder}/lib/cmake/SFML)
    set(SFML_STATIC_LIBRARIES TRUE)
    find_package(SFML COMPONENTS audio graphics window system REQUIRED)

    include_directories(${SMFL_folder}/include)
    target_link_libraries(game_cell sfml-audio sfml-graphics sfml-window sfml-system ${LINK_LIBS})

endif()

