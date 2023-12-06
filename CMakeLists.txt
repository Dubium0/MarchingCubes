# CMakeList.txt : CMake project for MarchingCubes, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project ("MarchingCubes")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)



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
set(IMGUI_BACKENDS_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/backends")
file(GLOB IMGUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/backends/*.cpp")
#get stb_image
set(STB_IMAGE_INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include/STB_IMAGE" )
set(STB_IMAGE_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/include/STB_IMAGE/stb_image.cpp" )


#get source headers
file(GLOB MY_SOURCE_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" )

#get source files
file(GLOB MY_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")



#set include directories
include_directories(
		${GLFW_INCLUDE_PATH}
		${GLEW_INCLUDE_PATH}
		${GLM_INCLUDE_PATH}
		${IMGUI_INCLUDE_PATH}
		${IMGUI_BACKENDS_INCLUDE_PATH}
		${STB_IMAGE_INCLUDE_PATH}
		
		
	
		
)
add_executable(MarchingCubes ${MY_SOURCE_HEADERS} ${MY_SOURCE_FILES} ${IMGUI_SOURCES}  "src/computeShader.h")

target_link_libraries(MarchingCubes
	${OPENGL_LIBRARY}
	${GLFW_LIBRARY}
	${GLEW_LIBRARY}
)
