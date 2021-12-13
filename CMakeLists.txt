cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME thecell)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)

set(CANDLE_HEADERS
        src/utils/Candle/include/Candle/LightingArea.hpp
        src/utils/Candle/include/Candle/LightSource.hpp
        src/utils/Candle/include/Candle/RadialLight.hpp
        src/utils/Candle/include/Candle/DirectedLight.hpp
        src/utils/Candle/include/Candle/geometry/Line.hpp
        src/utils/Candle/include/Candle/geometry/Polygon.hpp
        src/utils/Candle/include/Candle/geometry/Vector2.hpp
        src/utils/Candle/include/Candle/graphics/Color.hpp
        src/utils/Candle/include/Candle/graphics/VertexArray.hpp
        src/utils/Candle/include/Candle/Constants.hpp
        )

set(CANDLE_SRC
        src/utils/Candle/src/LightingArea.cpp
        src/utils/Candle/src/LightSource.cpp
        src/utils/Candle/src/RadialLight.cpp
        src/utils/Candle/src/DirectedLight.cpp
        src/utils/Candle/src/Line.cpp
        src/utils/Candle/src/Polygon.cpp
        src/utils/Candle/src/Color.cpp
        src/utils/Candle/src/VertexArray.cpp
        src/utils/Candle/src/Constants.cpp
        src/P2Pserver.cpp src/P2Pserver.h src/map/StringUtils.h src/P2PClient.cpp src/P2PClient.h)

add_executable(${PROJECT_NAME} main.cpp ${CANDLE_HEADERS} ${CANDLE_SRC} src/Assets.cpp src/Assets.h src/Collision.cpp src/Collision.h src/old/DedicatedServer.cpp src/old/DedicatedServer.h src/Entity.cpp src/Entity.h src/GameManager.cpp src/GameManager.h src/Core.cpp src/Core.h src/utils/fps.h src/Player.cpp src/Player.h src/map/Map.cpp src/map/Map.h src/utils/BaseMenu.cpp src/utils/BaseMenu.h src/State.h src/utils/ServerMenu.cpp src/utils/ServerMenu.h)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}/)

#set(STEAM_CONTENT ${CMAKE_SOURCE_DIR}/steam/tools/ContentBuilder/content)

if(WIN32)
    if (CMAKE_BUILD_TYPE MATCHES Release)
        target_link_options(${PROJECT_NAME} PRIVATE -mwindows)
    endif ()

    file(COPY ${CMAKE_SOURCE_DIR}/steam/redistributable_bin/win64/steam_api64.dll
            DESTINATION ${CMAKE_BINARY_DIR}/)

    set(STEAMLIB ${CMAKE_SOURCE_DIR}/steam/redistributable_bin/win64/steam_api64.lib)
    set(CMAKE_C_COMPILER c:/mingw64/bin/g++)
    set(SMFL_folder c:/SFML)

    set(SFML_ROOT ${SMFL_folder}/lib/cmake/SFML)
    set(SFML_STATIC_LIBRARIES TRUE)
    find_package(SFML COMPONENTS audio graphics window system REQUIRED)

    include_directories(${SMFL_folder}/include)
    target_link_libraries(${PROJECT_NAME} sfml-audio sfml-graphics sfml-window sfml-system ${STEAMLIB} ${LINK_LIBS})

    file(REMOVE ${CMAKE_SOURCE_DIR}/steam/tools/ContentBuilder/content/win/${PROJECT_NAME}.exe)
    file(COPY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.exe DESTINATION ${CMAKE_SOURCE_DIR}/steam/tools/ContentBuilder/content/win/)
    file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_SOURCE_DIR}/steam/tools/ContentBuilder/content/win/)
endif()

if(UNIX AND NOT APPLE)
    set(STEAMLIB ${CMAKE_SOURCE_DIR}/steam/redistributable_bin/linux64/libsteam_api.so)
    find_package(SFML REQUIRED system window graphics network audio)

    if(SFML_FOUND)
        include_directories(${SFML_INCLUDE_DIR})
        target_link_libraries(${PROJECT_NAME} -lsfml-graphics -lsfml-window -lsfml-system ${STEAMLIB} ${LINK_LIBS})
    endif()
elseif(UNIX)
    set(STEAMLIB ${CMAKE_SOURCE_DIR}/steam/redistributable_bin/osx/libsteam_api.dylib)
    find_package(SFML REQUIRED COMPONENTS system window graphics network audio)

    if(SFML_FOUND)
        include_directories(${SFML_INCLUDE_DIR})
        target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system ${STEAMLIB} ${LINK_LIBS})
    endif()
endif()