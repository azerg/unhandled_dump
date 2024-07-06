file(GLOB_RECURSE CORE_SRC core/*.cpp)
file(GLOB_RECURSE CORE_INC core/*.h)
file(GLOB_RECURSE API_SRC common/api/*.cpp)
file(GLOB_RECURSE API_INC common/api/*.h)

source_group("Source Files" FILES ${CORE_SRC})
source_group("Include" FILES ${CORE_INC})
source_group("Common/Api/Source Files" FILES ${API_SRC})
source_group("Common/Api/Include" FILES ${API_INC})

add_library(unhandled_dump OBJECT ${CORE_SRC} ${CORE_INC} ${API_SRC} ${API_INC})

#set_property(TARGET unhandled_dump PROPERTY
#  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_include_directories(
  unhandled_dump
  PUBLIC "${CMAKE_CURRENT_LIST_DIR}/common/api/src" "${CMAKE_CURRENT_LIST_DIR}/core/include" "${CMAKE_CURRENT_LIST_DIR}/common/api/include")