cmake_minimum_required(VERSION 3.10)
project(internetchesskiller VERSION 1.71 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows-specific settings
if(WIN32)
    add_definitions(-DWIN32 -D_WINDOWS)
    # Use static runtime for MinGW
    if(MINGW)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
    endif()
endif()

# Fruit chess engine library
set(FRUIT_SOURCES
    fruit/attack.cpp
    fruit/board.cpp
    fruit/fen.cpp
    fruit/hash.cpp
    fruit/list.cpp
    fruit/move.cpp
    fruit/move_check.cpp
    fruit/move_do.cpp
    fruit/move_evasion.cpp
    fruit/move_gen.cpp
    fruit/move_legal.cpp
    fruit/my_util.cpp
    fruit/pawn.cpp
    fruit/piece.cpp
    fruit/posix.cpp
    fruit/pst.cpp
    fruit/random.cpp
    fruit/square.cpp
    fruit/value.cpp
    fruit/vector.cpp
)

set(FRUIT_HEADERS
    fruit/attack.h
    fruit/board.h
    fruit/colour.h
    fruit/fen.h
    fruit/hash.h
    fruit/list.h
    fruit/move.h
    fruit/move_check.h
    fruit/move_do.h
    fruit/move_evasion.h
    fruit/move_gen.h
    fruit/move_legal.h
    fruit/my_util.h
    fruit/pawn.h
    fruit/piece.h
    fruit/posix.h
    fruit/pst.h
    fruit/random.h
    fruit/square.h
    fruit/value.h
    fruit/vector.h
)

# Core application sources (portable, no VCL)
set(CORE_SOURCES
    TBoardCapture.cpp
    TBoardRecognize.cpp
    TEngine.cpp
    TUCIInterface.cpp
    TState.cpp
    TPlayer.cpp
    TDebug.cpp
    TMainThreadObject.cpp
    process.cpp
    find_pos.cpp
    parse.cpp
)

set(CORE_HEADERS
    TBoardCapture.h
    TBoardRecognize.h
    TEngine.h
    TUCIInterface.h
    TState.h
    TPlayer.h
    TDebug.h
    TMainThreadObject.h
    process.h
    find_pos.h
    parse.h
    ColorUtils.h
    EdgeDetection.h
    DetectionConfig.h
    ConfigLoader.h
    DebugOverlay.h
)

# Main executable
add_executable(internetchesskiller
    main_win32.cpp
    ${CORE_SOURCES}
    ${CORE_HEADERS}
    ${FRUIT_SOURCES}
    ${FRUIT_HEADERS}
)

# Link Windows libraries
if(WIN32)
    target_link_libraries(internetchesskiller
        gdi32
        user32
        kernel32
        advapi32
    )
endif()

# Include directories
target_include_directories(internetchesskiller PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/fruit
)

# Set output directory
set_target_properties(internetchesskiller PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Installation
install(TARGETS internetchesskiller DESTINATION bin)
install(FILES detection.ini DESTINATION bin)
install(FILES standard.lrn DESTINATION bin)
