# CMakeList.txt : CMake project for MarchingCubes, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project ("MarchingCubes")




find_package(OpenGL REQUIRED) 

message(STATUS " hello")
#get glfw
set(GLFW_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/GLFW")
set(GLFW_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/GLFW/glfw3.lib" )
#get glew
set(GLEW_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/GLEW")
set(GLEW_LIBRARY "${CMAKE_CURRENT_SOURCE_DIR}/lib/GLEW/glew32.lib" )
#get glm
set(GLM_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/glm" )
#get imgui
set(IMGUI_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/imgui")

#get source headers
file(GLOB MY_SOURCE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")

#get source files
file(GLOB MY_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")


#set include directories
include_directories(
		${GLFW_INCLUDE_PATH}
		${GLEW_INCLUDE_PATH}
		${GLM_INCLUDE_PATH}
		${IMGUI_INCLUDE_PATH}
)
add_executable(MarchingCubes ${MY_SOURCE_HEADERS} ${MY_SOURCE_FILES})

target_link_libraries(MarchingCubes
	${OPENGL_LIBRARY}
	${GLFW_LIBRARY}
	${GLEW_LIBRARY}


)
