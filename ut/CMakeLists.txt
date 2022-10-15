# Prepare doctest for other targets to use
# find_package(doctest REQUIRED)

# Make test executable
add_executable(tests main.cpp)

include_directories(tests ../include)
include_directories(tests ../interface)