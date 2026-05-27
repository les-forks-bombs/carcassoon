#include <libcarcassonne/ext_base_game.h>
#include <libcarcassonne/options.h>

static const extension_t* base_ptr_table[] = {&LIBCARCASSONNE_EXT_BASE_GAME};

static const extension_vector_t base = {.meta = {
                                            .size = 1,
                                            .caps = 1,
                                            .data = &base_ptr_table,
                                        }};

options_t options = {
    .mode       = CARCASSONNE_MODE_CLI,
    .players    = 3,
    .seed       = 500,
    .ai         = 0,
    .max_turns  = 0,
    .extensions = base,
};