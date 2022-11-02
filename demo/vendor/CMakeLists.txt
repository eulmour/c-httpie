
# for simple libraries

file (GLOB FILES_GUI CONFIGURE_DEPENDS
    "stb/*.h" "stb/*.c"
)

target_sources(demo
    PRIVATE
        ${FILES_GUI}
)
