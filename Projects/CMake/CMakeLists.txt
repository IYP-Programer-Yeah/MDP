cmake_minimum_required (VERSION 3.5)

set(REPO_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(CMAKE_PROJECT_ROOT ${REPO_ROOT}/Projects/CMake)
set(LOCAL_BUILD_DIR ${CMAKE_PROJECT_ROOT}/Build)
if(WIN32)
	set(LOCAL_INCLUDE_DIR ${LOCAL_BUILD_DIR}/Include)
else()
	set(LOCAL_INCLUDE_DIR ${LOCAL_BUILD_DIR}/include)
endif()
set(CMAKE_CXX_STANDARD 11)

add_subdirectory(Utility)
add_subdirectory(googletest)
add_subdirectory(MDP)
add_subdirectory(Test)

