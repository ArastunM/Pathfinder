cmake_minimum_required(VERSION 3.20)
project(Pathfinder)

set(CMAKE_CXX_STANDARD 14)

add_executable(Pathfinder WIN32 main.cpp appWindow.hpp appWindow.cpp map.hpp pixel.hpp pixel.cpp map.cpp pathfinder.hpp pathfinder.cpp)

set(RESOURCE_FILES assets
        assets/images/pathfinder_tileset.png
        assets/images/pathfinder_tileset2.png)

include_directories(Pathfinder "C:/Program Files/SFML-2.5.1/include")

set(SFML_ROOT "C:/Program Files/SFML-2.5.1")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(Pathfinder ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# Copy dlls to build
if(WIN32)
    file(GLOB BINARY_DEP_DLLS "${SFML_INCLUDE_DIR}/../bin/*.dll")
    file(COPY ${BINARY_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
    file(GLOB MINGW_DEP_DLLS "C:/Program Files/mingw64/bin/*.dll")
    file(COPY ${MINGW_DEP_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()
