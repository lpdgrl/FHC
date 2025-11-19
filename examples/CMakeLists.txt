# Пример CMakeLists.txt в examples/example1/
add_executable(http-example http_example.cpp)


target_link_libraries(http-example PRIVATE fhc)

target_compile_definitions(http-example PRIVATE
    CMAKE_PATH_TO_WEB_FILES="${CMAKE_SOURCE_DIR}/examples/web/"
)