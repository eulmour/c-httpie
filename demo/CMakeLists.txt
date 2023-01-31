include_directories("${CMAKE_SOURCE_DIR}/httpie")

file(GLOB FILES_SRC CONFIGURE_DEPENDS
    "src/*.h"
    "src/*.c"
    "src/perceptron/*.c"
    "src/perceptron/*.h"
)

add_definitions(-DHTTPIE_PROTOCOL_1_1 -D_CRT_SECURE_NO_WARNINGS)

add_executable(demo ${FILES_SRC})

add_subdirectory(vendor)

target_include_directories(demo PRIVATE vendor)

if (WIN32)
    target_link_libraries(demo httpie)
else()
    target_link_libraries(demo httpie m)
endif()
