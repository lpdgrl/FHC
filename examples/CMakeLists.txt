# Пример CMakeLists.txt в examples/example1/
add_executable(http-example http_example.cpp)


target_link_libraries(http-example PRIVATE fhc)