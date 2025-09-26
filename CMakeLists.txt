cmake_minimum_required(VERSION 4.0)
project(Hammurabi_clone)

set(CMAKE_CXX_STANDARD 20)

add_executable(Hammurabi_clone main.cpp
        core/GameEngine.cpp
        core/GameEngine.h
        core/GameState.h
        core/Constants.h
        ui/ConsoleUI.cpp
        ui/ConsoleUI.h
        persistence/SaveManager.cpp
        persistence/SaveManager.h
        GameApp.cpp
        GameApp.h)
