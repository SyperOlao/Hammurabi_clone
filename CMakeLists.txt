cmake_minimum_required(VERSION 4.0)
project(Hammurabi_clone)

set(CMAKE_CXX_STANDARD 20)
add_executable(Hammurabi_clone main.cpp
        core/GameState.h
        core/Constants.h
        ui/ConsoleUI.cpp
        ui/ConsoleUI.h
        persistence/SaveManager.cpp
        persistence/SaveManager.h
        GameApp.cpp
        GameApp.h
        core/GameLogic.cpp
        core/GameLogic.h
        ui/validation/Validation.cpp
        ui/validation/Validation.h
        ui/validation/ValidationResult.h
        utils/Utils.h
        core/GameRepository.cpp
        core/GameRepository.h
        utils/Color.h
)

target_link_libraries(Hammurabi_clone PRIVATE winmm)
