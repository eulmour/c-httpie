project(c-httpie-test C)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")

include_directories("${CMAKE_SOURCE_DIR}/httpie")

add_definitions(-DHTTPIE_PROTOCOL_1_1 -D_CRT_SECURE_NO_WARNINGS)

add_executable(c-httpie-test
    src/main.c
    src/client.c
)

# add_subdirectory(vendor)

# target_include_directories(c-httpie-test PRIVATE vendor)

if (WIN32)
    target_link_libraries(c-httpie-test httpie)
else()
    target_link_libraries(c-httpie-test httpie m)
endif()
