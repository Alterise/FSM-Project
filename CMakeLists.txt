cmake_minimum_required(VERSION 3.22)
project(FSM)

set(CMAKE_CXX_STANDARD 20)

add_executable(FSM main.cpp FSM_impl/guard.cpp utility/Character.cpp)

include_directories(headers ./SFML/include ./tinyfsm/include ./utility ./FSM_impl)

set(SFML_ROOT ./SFML)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(FSM ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()
