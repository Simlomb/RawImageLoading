cmake_minimum_required(VERSION 3.10)
project (loadRaw)

# search for pkg-config
include (FindPkgConfig)
if (NOT PKG_CONFIG_FOUND)
    message (FATAL_ERROR "pkg-config not found")
endif ()

# check for libpng
pkg_check_modules (LIBPNG libpng16 REQUIRED)
if (NOT LIBPNG_FOUND)
    message(FATAL_ERROR "You don't seem to have libpng16 development libraries installed")
else ()
    include_directories (${LIBPNG_INCLUDE_DIRS})
    link_directories (${LIBPNG_LIBRARY_DIRS})
    link_libraries (${LIBPNG_LIBRARIES})
endif ()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -lm -lpng -ftree-vectorize -fPIC -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -lm -lpng -ftree-vectorize -fPIC -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O2 -lm -lpng -ftree-vectorize -fPIC -flto -std=c++11")

add_executable(loadRaw loadRaw.cpp ${LIBPNG_LINK_FLAGS})
