cmake_minimum_required(VERSION 3.20)
project(game_cell)

set(CMAKE_CXX_STANDARD 17)
add_executable(game_cell main.cpp src/Assets.cpp src/Assets.h src/Collision.cpp src/Collision.h src/Server.cpp src/Server.h)

if(WIN32)
    file(COPY ${CMAKE_SOURCE_DIR}/steam/redistributable_bin/win64/steam_api64.dll
            DESTINATION ${CMAKE_BINARY_DIR}/)

    set(STEAMLIB ${CMAKE_SOURCE_DIR}/steam/redistributable_bin/win64/steam_api64.lib)
    set(CMAKE_C_COMPILER c:/mingw64/bin/g++)
    set(SMFL_folder c:/SFML)

    set(SFML_ROOT ${SMFL_folder}/lib/cmake/SFML)
    set(SFML_STATIC_LIBRARIES TRUE)
    find_package(SFML COMPONENTS audio graphics window system REQUIRED)

    include_directories(${SMFL_folder}/include)
    target_link_libraries(game_cell sfml-audio sfml-graphics sfml-window sfml-system ${STEAMLIB})

endif()

if(UNIX AND NOT APPLE)
    set(STEAMLIB ${CMAKE_SOURCE_DIR}/steam/redistributable_bin/linux64/libsteam_api.so)
    find_package(SFML REQUIRED system window graphics network audio)

    if(SFML_FOUND)
        include_directories(${SFML_INCLUDE_DIR})
        target_link_libraries(game_cell -lsfml-graphics -lsfml-window -lsfml-system ${STEAMLIB})
    endif()
endif()

