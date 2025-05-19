cmake_minimum_required(VERSION 3.30)
project(elaborato_OpenMP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -fopenmp")


find_package(OpenMP REQUIRED)  # Trova OpenMP

add_executable(elaborato_OpenMP main_total.cpp)

if(OpenMP_CXX_FOUND)
	message(STATUS "OpenMP found!")
	target_compile_options(elaborato_OpenMP PRIVATE -fopenmp)
	target_link_libraries(elaborato_OpenMP PRIVATE -fopenmp)
	target_link_libraries(elaborato_OpenMP PRIVATE libgomp)
	target_link_libraries(elaborato_OpenMP PRIVATE OpenMP::OpenMP_CXX)
endif()

# Abilita Thread Sanitizer
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-fsanitize=thread)
    add_link_options(-fsanitize=thread)
endif()



