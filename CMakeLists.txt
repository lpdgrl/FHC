cmake_minimum_required(VERSION 3.10)
project (fhc)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BINACPP_DIR _deps/binacpp)
set(LIBPQXX_DIR _deps/libpqxx)
set(FLATBUFFERS_DIR _deps/flatbuffers)
set(CPP_HTTPLIB_DIR _deps/cpp-httplib)

set(FHC_HEADERS_DIR include)
set(FHC_SOURCE_DIR src)
set(EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)
# Add binacpp
add_subdirectory(${BINACPP_DIR})

# Add libpqxx
add_subdirectory(${LIBPQXX_DIR})

# Add flattbuffers
add_subdirectory(${FLATBUFFERS_DIR})

# Add cpp-httplib
add_subdirectory(${CPP_HTTPLIB_DIR})

add_subdirectory(${EXAMPLES_DIR})

# Generate flatbuffers files
set(FLATC ${FLATBUFFERS_DIR}/flatc)
set(SCHEMA ${CMAKE_CURRENT_SOURCE_DIR}/schemas/klines.fbs)
set(GENERATED ${CMAKE_CURRENT_SOURCE_DIR}/include/flatbuffers/klines_generated.h)

add_custom_command(
    OUTPUT ${GENERATED}
    COMMAND ${FLATC} --cpp -o ${CMAKE_CURRENT_SOURCE_DIR}/include/flatbuffers/ ${SCHEMA}
    DEPENDS ${SCHEMA} flatc
    COMMENT "Generating FlatBuffers code from ${SCHEMA}"
)

add_custom_target(generate_flatbuffers DEPENDS ${GENERATED})

# Target executable file
add_library(fhc STATIC 
    ${FHC_SOURCE_DIR}/server/server.cpp
    ${FHC_SOURCE_DIR}/server/request_handler.cpp
    ${FHC_SOURCE_DIR}/server/utils.cpp
    ${FHC_SOURCE_DIR}/server/httplib_adapter.cpp
    ${FHC_SOURCE_DIR}/base/database.cpp
    ${FHC_SOURCE_DIR}/base/pqxx_adapter.cpp
    ${FHC_SOURCE_DIR}/base/binance_client.cpp
    ${FHC_SOURCE_DIR}/base/sql_loader.cpp
)

add_dependencies(fhc generate_flatbuffers)

target_include_directories(fhc PUBLIC
    ${BINACPP_DIR}/src
    ${FHC_HEADERS_DIR}
) 

target_link_libraries(fhc PUBLIC
    binacpp
    pqxx
    flatbuffers
    httplib
)

target_compile_definitions(fhc PRIVATE
    CMAKE_CONFIG_PATH="${CMAKE_SOURCE_DIR}/config/"
)

target_compile_definitions(fhc PRIVATE
    CMAKE_SQL_PATH="${CMAKE_SOURCE_DIR}/sql/"
)