cmake_minimum_required(VERSION 3.10)
project (fhc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BINACPP_DIR extern/binacpp)
set(LIBPQXX_DIR extern/libpqxx)
set(FLATBUFFERS_DIR extern/flatbuffers)


set(FHC_HEADERS_DIR include)
set(FHC_CPP_DIR src)

# set(FHC_CPP
#     ${FHC_CPP_DIR}/main.cpp
# )

# Add binacpp
add_subdirectory(${BINACPP_DIR})

# Add libpqxx
add_subdirectory(${LIBPQXX_DIR})

# Add flattbuffers
add_subdirectory(${FLATBUFFERS_DIR})

# Generate flatbuffers files
set(FLATC ${FLATBUFFERS_DIR}/flatc)
set(SCHEMA ${CMAKE_CURRENT_SOURCE_DIR}/schemas/klines.fbs)
set(GENERATED ${CMAKE_CURRENT_SOURCE_DIR}/include/flatbuffers/klines_generated.h)

add_custom_command(
    OUTPUT ${GENERATED}
    COMMAND ${FLATC} --cpp -o ${CMAKE_CURRENT_SOURCE_DIR}/include/flatbuffers/ ${SCHEMA}
    DEPENDS ${SCHEMA}
    COMMENT "Generating FlatBuffers code from ${SCHEMA}"
)

add_custom_target(generate_flatbuffers DEPENDS ${GENERATED})

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
    ${FHC_CPP_DIR}/base/pqxx_adapter.cpp
    ${FHC_CPP_DIR}/base/binance_client.cpp
)

add_dependencies(fhc generate_flatbuffers)

target_link_libraries(fhc PRIVATE
    binacpp
    pqxx
    flatbuffers
)