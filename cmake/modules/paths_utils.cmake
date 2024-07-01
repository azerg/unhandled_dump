function(find_files_by_mask RESULT MASK PATH)
    file(GLOB_RECURSE FILES "${PATH}/*${MASK}")
    set(${RESULT} ${FILES} PARENT_SCOPE)
endfunction()