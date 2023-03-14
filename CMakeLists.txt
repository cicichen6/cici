cmake_minimum_required(VERSION 3.1)
set(project_name async-start)
project(${project_name})
set(CMAKE_CXX_STANDARD 11)
include_directories(async-start)
add_library(${project_name}-lib
	async-start/appComponent.hpp
	async-start/myController.cpp
	async-start/myController.hpp
	async-start/dtos.hpp
)

find_package(oatpp 1.3.0 REQUIRED)
target_link_libraries(${project_name}-lib
	PUBLIC oatpp::oatpp
	
)
add_executable(${project_name}-exe 
	async-start/app.cpp)
target_link_libraries(${project_name}-exe ${project_name}-lib)
add_dependencies(${project_name}-exe ${project_name}-lib)
