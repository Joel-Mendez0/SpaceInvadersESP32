set(srcs 
    "main.c"
    "draw.c"
    "game.c"
    "state.c"
    "input.c"
    )

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ".")