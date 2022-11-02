file(GLOB FILES_SRC CONFIGURE_DEPENDS "*.h" "src/*.h" "src/*.c")

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_library(${CMAKE_PROJECT_NAME} STATIC ${FILES_SRC})

if (MSVC)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /W4)
    # target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /W4 /fsanitize=address /Zi)
else()
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

if (WIN32)
    target_link_libraries(${CMAKE_PROJECT_NAME} Ws2_32)
else()
    target_link_libraries(${CMAKE_PROJECT_NAME})
endif()
