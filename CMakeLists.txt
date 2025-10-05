cmake_minimum_required(VERSION 3.16)
project(Hammurabi_clone)
# C++20 строго, без расширений компилятора
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Исполняемый файл — оставляю старое имя, чтобы было hammurabi.exe
add_executable(Hammurabi_clone
        main.cpp
        GameApp.cpp
        core/GameLogic.cpp
        core/GameRepository.cpp
        ui/ConsoleUI.cpp
        ui/validation/Validation.cpp
        persistence/SaveManager.cpp
        ui/sound/Sound.cpp
)

# Заголовки подключаем через include dirs
target_include_directories(Hammurabi_clone PRIVATE
        .
        core
        ui
        ui/validation
        persistence
        utils
        ui/sound
)

# MSVC: корректная кодировка и строгий стандарт
if (MSVC)
    target_compile_options(Hammurabi_clone PRIVATE /utf-8 /permissive-)
endif()

# Линкуем winmm только на Windows (MSVC/MinGW)
if (WIN32)
    target_link_libraries(Hammurabi_clone PRIVATE winmm)
endif()