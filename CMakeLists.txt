cmake_minimum_required(VERSION 3.10)
project (fhc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BINACPP_DIR extern/binacpp)
set(LIBPQXX extern/libpqxx)

# Add lib binacpp
add_subdirectory(${BINACPP_DIR})

# Add lib libpqxx
add_subdirectory(${LIBPQXX})

# Paths to include
include_directories(
    ${CMAKE_SOURCE_DIR}/extern/binacpp/src
)

# Target executable file
add_executable(fhc src/main.cpp)

target_link_libraries(fhc PRIVATE
    binacpp
    pqxx
)