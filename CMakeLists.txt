cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
SET(NAME ONIEngine)
PROJECT(${NAME} VERSION 1.0.0 LANGUAGES CXX)

include_directories(external/glm)
include_directories(external/tinyobjloader)
include_directories(external/stb)

SET(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
SET(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
SET(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
link_libraries(glfw)

IF (NOT CMAKE_VERSION VERSION_LESS 3.7.0)
	message(STATUS "Looking for Vulkan...")
	find_package(Vulkan)
ENDIF()

IF (NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
ELSE()
	message(STATUS ${Vulkan_LIBRARY})
	link_libraries(Vulkan::Vulkan)
ENDIF()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX -D_USE_MATH_DEFINES")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-switch-enum")
ENDIF()

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(SOURCES src/Camera.cpp src/DirectionalLight.cpp src/Material.cpp src/MemoryAllocator.cpp src/Mesh.cpp src/Model.cpp src/Object.cpp src/PointLight.cpp src/Renderer.cpp src/Scene.cpp src/SGNode.cpp src/Skybox.cpp src/SpotLight.cpp)
SET(HEADERS src/Camera.h src/DirectionalLight.h src/Material.h src/MemoryAllocator.h src/Mesh.h src/Model.h src/Object.h src/PointLight.h src/Renderer.h src/Scene.h src/SGNode.h src/Skybox.h src/SpotLight.h)

add_executable(${PROJECT_NAME} main.cpp ${SOURCES} ${HEADERS})