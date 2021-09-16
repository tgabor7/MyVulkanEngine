cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(MyVulkanEngine)

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hpp)

set (CMAKE_CXX_STANDARD 17)

set(LIBRARIES
	glfw3.lib
	vulkan-1.lib)

link_directories("libraries/glfw/lib")
link_directories("libraries/VulkanSDK/Lib")

add_executable(main Source.cpp ${sources} ${headers})
target_link_libraries(main ${LIBRARIES})

include_directories("libraries/VulkanSDK/Include")
include_directories("libraries/glfw/include")
include_directories("libraries/glm/include")
include_directories("libraries/tinyobjectloader")

install(TARGETS main DESTINATION bin)