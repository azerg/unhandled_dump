find_files_by_mask(SOURCE_FILES "*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src")
find_files_by_mask(HEADER_FILES "*.h" "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Create the agent executable
add_executable(test_cli ${SOURCE_FILES} ${HEADER_FILES})

#set_property(TARGET test_cli PROPERTY
#  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(
    test_cli
    unhandled_dump)

source_group("Source Files" FILES ${SOURCE_FILES})

target_include_directories(test_cli PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_dependencies(test_cli unhandled_dump)