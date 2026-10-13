idf_component_register(
    SRCS "main.c"
         "antonym_game.c"
    INCLUDE_DIRS "."
    REQUIRES bsp
)

target_sources(${COMPONENT_LIB} PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/../assets/fonts/antonym_font_16.c"
    "${CMAKE_CURRENT_LIST_DIR}/../assets/fonts/antonym_prompt_font_28.c"
)
