include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.16.0  
)
# Don't install gtest with our package
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "Disable gtest installation" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(gtest REQUIRED)

add_executable(mfri_test)

file(GLOB IDL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/idl/*.cxx")


target_include_directories(
    mfri_test
    PUBLIC    
    ${MFRI_INCLUDE_DIRS}
)


target_sources(
    mfri_test
    PUBLIC    
    test_mfri.cpp   
    ${IDL_SOURCES}
    ${MFRI_IDL_SRCS}
)

target_link_libraries(
    mfri_test
    PUBLIC
     ${MFRI_DEP_LIBS}
    gtest_main
)


gtest_discover_tests(mfri_test)

# Explicitly mark test executable as not to be installed
set_target_properties(mfri_test PROPERTIES EXCLUDE_FROM_ALL TRUE)

