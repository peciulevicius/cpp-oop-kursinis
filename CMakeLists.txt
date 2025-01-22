cmake_minimum_required(VERSION 3.10)
project(cpp_oop_kursinis)

set(CMAKE_CXX_STANDARD 17)

# Find SFML version 3.0 or newer
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add executable and link SFML libraries
add_executable(cpp_oop_kursinis main.cpp
        Snake.cpp
        Snake.h
        Food.cpp
        Food.h
        Game.cpp
        Game.h
        Entity.cpp
        Entity.h
        GameObject.cpp
        GameObject.h
        Container.cpp
        Container.h)
target_link_libraries(cpp_oop_kursinis sfml-graphics sfml-window sfml-system)
