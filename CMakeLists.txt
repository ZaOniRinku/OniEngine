cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
cmake_policy(VERSION 3.0)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

SET(NAME ONIEngine)

PROJECT(${NAME})

include_directories(glm)
include_directories(tiny_obj_loader)
include_directories(glfw)

IF (NOT CMAKE_VERSION VERSION_LESS 3.7.0)
	message(STATUS "Looking for Vulkan...")
	find_package(Vulkan)
ENDIF()

IF (NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
ELSE()
	message(STATUS ${Vulkan_LIBRARY})
ENDIF()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX -D_USE_MATH_DEFINES}

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-switch-enum)
ENDIF()

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB SOURCE src/*.cpp)

add_executable(${PROJECT_NAME} )

target_link_l