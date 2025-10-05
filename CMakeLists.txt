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

if (MSVC)
    target_compile_options(Hammurabi_clone PRIVATE /utf-8 /permissive-)
endif()


if (WIN32)
    add_custom_command(TARGET Hammurabi_clone POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:Hammurabi_clone>/assets"
            COMMENT "Copying assets to executable directory..."
    )
    target_link_libraries(Hammurabi_clone PRIVATE winmm)
endif()