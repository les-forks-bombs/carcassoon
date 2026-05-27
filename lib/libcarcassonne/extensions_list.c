#include <libcarcassonne/ext_base_game.h>
#include <libcarcassonne/extensions_list.h>
#include <libcarcassonne/libcarcassonne.h>

const extension_t* LIBCARCASSONNE_EXTENSIONS[] = {
    &LIBCARCASSONNE_EXT_BASE_GAME};

const unsigned int LIBCARCASSONNE_EXTENSIONS_SIZE =
    SIZE(LIBCARCASSONNE_EXTENSIONS);
