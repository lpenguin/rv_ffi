cmake_minimum_required(VERSION 3.5)

project(rv_ffi LANGUAGES CXX)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(SDL2_image REQUIRED)
FIND_PACKAGE(GLES2 REQUIRED)
FIND_PACKAGE(RustyVangers REQUIRED)

set(CMAKE_CXX_STANDARD 20) # MSVC only
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 ")

add_executable(rv_ffi
	main.cpp
        vange_rs_stub.cpp
	vange_rs.h
	)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})


set(RV_FFI_LIBS
    ${RUSTY_VANGERS_LIBRARY}
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${GLES2_LIBRARY}
    # ld
    pthread
    )

if(WIN32 AND NOT MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "-lmingw32") # MinGW: fixes "undefined reference to `WinMain'"

    set(MINGW_WINDOWS_LIBS
        userenv
        bcrypt
        d3d12
        d3dcompiler
        ws2_32)

    set(RV_FFI_LIBS
        ${RV_FFI_LIBS}
        ${MINGW_WINDOWS_LIBS}
        )
endif(WIN32 AND NOT MSVC)

TARGET_LINK_LIBRARIES(rv_ffi
    ${RV_FFI_LIBS}
)
