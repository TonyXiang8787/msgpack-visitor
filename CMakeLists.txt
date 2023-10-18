# CMakeList.txt : CMake project for msgpack-visitor, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.23)

project ("msgpack-visitor")

find_package(msgpack-cxx REQUIRED)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Add source to this project's executable.
add_executable (msgpack-visitor "msgpack-visitor.cpp" )

target_link_libraries(msgpack-visitor
	PRIVATE msgpack-cxx
)
