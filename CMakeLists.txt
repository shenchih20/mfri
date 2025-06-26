cmake_minimum_required(VERSION 3.20)

list(APPEND CMAKE_PREFIX_PATH "D:/ws/lib/FastDDS")

project(mfri VERSION 1.0.0)


find_package(fastcdr CONFIG REQUIRED)
find_package(fastdds CONFIG REQUIRED)

list(
    APPEND
    MFRI_DEP_LIBS
    fastdds
    fastcdr
)


add_library(${PROJECT_NAME})

list(APPEND MFRI_INCLUDE_DIRS
 ${CMAKE_CURRENT_SOURCE_DIR}/include
 ${CMAKE_CURRENT_SOURCE_DIR}/include/mfri
 ${CMAKE_CURRENT_SOURCE_DIR}/include/mfri/idl
 )


target_include_directories(${PROJECT_NAME} PUBLIC
    ${MFRI_INCLUDE_DIRS}
)


target_link_libraries(${PROJECT_NAME} PUBLIC 
    ${MFRI_DEP_LIBS}
)


file(GLOB MFRI_IDL_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/idl/*.cxx")

target_sources(${PROJECT_NAME} PUBLIC
    ${MFRI_IDL_SRCS}
)

enable_testing()
add_subdirectory(test)