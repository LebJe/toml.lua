# Adapted from https://github.com/xpol/lua-rapidjson/blob/master/CMakeLists.txt

cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

set(FALLBACK_VERSION 0.0.1)
set(TOML++ toml++)
set(SOL2 sol2)
set(MAGIC_ENUM magic_enum)

if(NOT TOML_LUA_VERSION)
    find_package(Git)
    if(GIT_FOUND)
        execute_process(COMMAND ${GIT_EXECUTABLE} describe --dirty
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE ret
            OUTPUT_VARIABLE LUA_RAPIDJSON_VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        if(NOT ret EQUAL "0")
            set(TOML_LUA_VERSION ${FALLBACK_VERSION})
        endif()
    else()
        set(TOML_LUA_VERSION ${FALLBACK_VERSION})
    endif()
endif()

project(toml.lua VERSION ${TOML_LUA_VERSION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# compile in release mode by default
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

include(FetchContent)

FetchContent_Declare(
	${TOML++}
	GIT_REPOSITORY "https://github.com/marzer/tomlplusplus.git"
	GIT_SHALLOW ON
    GIT_SUBMODULES ""
	GIT_TAG "ad55bae8a11a6eee39e2292b01e95b529b105767"
)

FetchContent_Declare(
	${SOL2}
	GIT_REPOSITORY "https://github.com/ThePhD/sol2.git"
	GIT_SHALLOW ON
    GIT_SUBMODULES ""
	GIT_TAG "v3.3.0"
)

FetchContent_Declare(
	${MAGIC_ENUM}
	GIT_REPOSITORY "https://github.com/Neargye/magic_enum.git"
	GIT_SHALLOW ON
    GIT_SUBMODULES ""
	GIT_TAG "v0.8.2"
)

FetchContent_GetProperties(${TOML++})
if(NOT ${TOML++}_POPULATED)
    message(STATUS "Cloning ${TOML++}")
	FetchContent_Populate(${TOML++}) 
    FetchContent_MakeAvailable(${TOML++})
endif()

FetchContent_GetProperties(${SOL2})
if(NOT ${SOL2}_POPULATED)
    message(STATUS "Cloning ${SOL2}")
	FetchContent_Populate(${SOL2})
    FetchContent_MakeAvailable(${SOL2})
endif()

FetchContent_GetProperties(${MAGIC_ENUM})
if(NOT ${MAGIC_ENUM}_POPULATED)
    message(STATUS "Cloning ${MAGIC_ENUM}")
	FetchContent_Populate(${MAGIC_ENUM})
    FetchContent_MakeAvailable(${MAGIC_ENUM})
endif()

if(UNIX)
    if(APPLE)
        set(PLAT "macosx")
        set(LINK_FLAGS "-bundle -undefined dynamic_lookup -all_load")
    else(APPLE)
        set(PLAT "linux")
        set(LINK_FLAGS "-shared")
    endif(APPLE)
	add_compile_options(-g -Wall -fPIC)
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_ARCH_NATIVE)
    if (COMPILER_SUPPORTS_ARCH_NATIVE)
        add_compile_options(-march=native)
    endif()
else(UNIX)
    if(WIN32)
        set(PLAT "win32")
        add_compile_definitions(LUA_BUILD_AS_DLL LUA_LIB _CRT_SECURE_NO_WARNINGS)
        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_definitions(MAGIC_ENUM_NO_CHECK_SUPPORT)
        endif()
    else(WIN32)
        set(PLAT "unsupported")
    endif(WIN32)
endif(UNIX)


if(NOT LUA_INCLUDE_DIR OR (WIN32 AND NOT LUA_LIBRARIES))
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
    find_package(Lua)
endif()

include_directories(${LUA_INCLUDE_DIR} src src/include ${${TOML++}_SOURCE_DIR} ${${SOL2}_SOURCE_DIR}/include ${${MAGIC_ENUM}_SOURCE_DIR}/include)

set(SOURCES
    src/toml.cpp
    src/decoding/decoding.cpp
    src/encoding/encoding.cpp
	src/DataTypes/DateAndTime/dateAndTime.cpp
	src/DataTypes/TOMLInt/TOMLInt.cpp
    src/utilities/utilities.cpp
)

add_library(toml.lua MODULE ${SOURCES})
source_group(src FILES ${SOURCES})

if(WIN32 AND "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	target_link_options(toml.lua PUBLIC ${PROJECT_SOURCE_DIR}\\libs\\lua51.lib)
else() 
    target_link_libraries(toml.lua ${LUA_LIBRARIES})    
endif()

if (LINK_FLAGS)
    set_target_properties(toml.lua PROPERTIES
        LINK_FLAGS ${LINK_FLAGS}
    )
endif()

set_target_properties(toml.lua PROPERTIES
    OUTPUT_NAME "toml"
    PREFIX ""
)

if (CMAKE_INSTALL_PREFIX)
  install(TARGETS toml.lua DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
endif(CMAKE_INSTALL_PREFIX)
