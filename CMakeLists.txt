cmake_minimum_required(VERSION 3.14)
project(utilWidgets LANGUAGES CXX)

# Header-only interface library
add_library(utilWidgets INTERFACE)

# Make headers available to consumers
target_include_directories(utilWidgets INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Optional: use with Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets)

target_link_libraries(utilWidgets INTERFACE
    Qt6::Widgets
)
