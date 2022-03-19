cmake_minimum_required(VERSION 3.0.0)
project(hw1)

add_subdirectory(hw1_lib)

message("HW1_INCLUDE_DIRS = ${HW1_INCLUDE_DIRS}")
message("HW1_LIBRARIES = ${HW1_LIBRARIES}")

add_executable(${PROJECT_NAME} main.c)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -coverage -lgcov" )

target_include_directories(${PROJECT_NAME} PUBLIC ${HW1_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${HW1_LIBRARIES})

add_subdirectory(tests)
