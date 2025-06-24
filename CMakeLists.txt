cmake_minimum_required(VERSION 3.20)

list(APPEND CMAKE_PREFIX_PATH "D:/ws/lib/FastDDS")

project(mfri VERSION 1.0.0)


find_package(fastcdr CONFIG REQUIRED)
find_package(fastdds CONFIG REQUIRED)

list(
    APPEND
    mfri_libs
    fastdds
    fastcdr
)

# Create an INTERFACE library for header-only libraries
add_library(${PROJECT_NAME} INTERFACE)

# Set the include directories for users of this library
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/mfri>
    $<INSTALL_INTERFACE:include>
)

# Link FastDDS and FastCDR to the interface library
target_link_libraries(${PROJECT_NAME} INTERFACE 
    ${mfri_libs}
)

# Add header files to the library target
target_sources(${PROJECT_NAME} INTERFACE
    FILE_SET HEADERS BASE_DIRS include FILES
    include/mfri/publisher.hpp    
    include/mfri/subscriber.hpp
    include/mfri/srv_client.hpp        
    include/mfri/srv_server.hpp        
)

# Installation rules for the header-only library
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    FILE_SET HEADERS DESTINATION include
)

# Export targets for use by other projects
install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)

# Install a basic config file for find_package support
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    @ONLY
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    DESTINATION lib/cmake/${PROJECT_NAME}
)

enable_testing()
add_subdirectory(test)