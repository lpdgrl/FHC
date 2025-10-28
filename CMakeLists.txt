cmake_minimum_required(VERSION 3.10)
project (fhc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BINACPP_DIR extern/binacpp)
set(LIBPQXX_DIR extern/libpqxx)

set(FHC_HEADERS_DIR include)
set(FHC_CPP_DIR src)
# set(FHC_CPP
#     ${FHC_CPP_DIR}/main.cpp

# )

# Add lib binacpp
add_subdirectory(${BINACPP_DIR})

# Add lib libpqxx
add_subdirectory(${LIBPQXX_DIR})

# Paths to include
include_directories(
    ${CMAKE_SOURCE_DIR}/extern/binacpp/src
    FHC_HEADERS_DIR
)

# Target executable file
add_executable(fhc 
    ${FHC_CPP_DIR}/main.cpp
    ${FHC_CPP_DIR}/server/server.cpp
    ${FHC_CPP_DIR}/server/request_handler.cpp
    ${FHC_CPP_DIR}/base/database.cpp
    ${FHC_CPP_DIR}/base/pqxx_adaptor.cpp
    ${FHC_CPP_DIR}/base/binance_client.cpp
)

target_link_libraries(fhc PRIVATE
    binacpp
    pqxx
)