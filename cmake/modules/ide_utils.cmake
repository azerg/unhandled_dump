function(find_files file_mask root_group files)
    # Collect all files matching the mask
    file(GLOB_RECURSE ALLFILES "${CMAKE_CURRENT_SOURCE_DIR}/${file_mask}")

    # Loop through each file
    foreach(file ${ALLFILES})
        # Get the relative path from the current source directory
        file(RELATIVE_PATH relative_path "${CMAKE_CURRENT_SOURCE_DIR}" "${file}")

        # Get the directory part of the relative path
        get_filename_component(group_name "${relative_path}" DIRECTORY)

        # Check if the file is in the root directory
        if(group_name STREQUAL "" OR "${file}" MATCHES "${CMAKE_CURRENT_SOURCE_DIR}/[^/]*$")
            set(group_name "${root_group}")
        else()
            # Replace '/' with '\' for Windows compatibility
            string(REPLACE "/" "\\" group_name "${group_name}")
        endif()

        # Create the source group
        source_group("${group_name}" FILES "${file}")
    endforeach()

    # Return the list of files
    set(${files} "${ALLFILES}" PARENT_SCOPE)
endfunction()