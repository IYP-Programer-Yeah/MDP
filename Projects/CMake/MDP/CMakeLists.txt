project(MDP)

add_custom_target(${PROJECT_NAME}_HEADERS ALL
	${CMAKE_COMMAND} -E make_directory ${LOCAL_INCLUDE_DIR}/${PROJECT_NAME}
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${REPO_ROOT}/${PROJECT_NAME}/Include/ ${LOCAL_INCLUDE_DIR}/${PROJECT_NAME})

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE ${LOCAL_INCLUDE_DIR})
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_HEADERS)

if(WIN32)
	install(DIRECTORY ${REPO_ROOT}/${PROJECT_NAME}/Include/ DESTINATION Include/${PROJECT_NAME})
else()
	install(DIRECTORY ${REPO_ROOT}/${PROJECT_NAME}/Include/ DESTINATION include/${PROJECT_NAME})
endif()

