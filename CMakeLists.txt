cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME thecell)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
add_executable(${PROJECT_NAME} main.cpp src/Assets.cpp src/Assets.h src/Collision.cpp src/Collision.h src/Server.cpp src/Server.h src/Entity.cpp src/Entity.h src/GameManager.cpp src/GameManager.h src/Core.cpp src/Core.h src/utils/fps.h src/Player.cpp src/Player.h src/map/Map.cpp src/map/Map.h src/utils/BaseMenu.cpp src/utils/BaseMenu.h src/State.h src/utils/ServerMenu.cpp src/utils/ServerMenu.h src/utils/Message.h)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR}/)

#set(STEAM_CONTENT ${CMAKE_SOURCE_DIR}/steam/tools/ContentBuilder/content)
add_subdirectory(libs/json-develop/json-develop)
set(LINK_LIBS nlohmann_json::nlohmann_json)

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