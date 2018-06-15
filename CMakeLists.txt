cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME solver)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(OpenGL_GL_PREFERENCE "GLVND")

include(${CMAKE_CURRENT_SOURCE_DIR}/source/matrixGenerator/Generator.cmake)

if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
	EXECUTE_PROCESS( COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
		${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
	)
endif()

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

set(SOURCES source/main.cpp
		source/solver/Solver.cpp
		source/solver/SolverGPU.cpp
		source/solver/CreateSolver.cpp
		source/utils/MatrixUtils.cpp
		${ADDITIONAL_SOURCE}
)

include_directories(
		${PROJECT_SOURCE_DIR}
		${OPENGL_INCLUDE_DIRS}
		${QT_INCLUDES}
)

add_executable(${PROJECT_NAME} ${SOURCES} resources/resources.qrc)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} Qt5::Core Qt5::Widgets)
