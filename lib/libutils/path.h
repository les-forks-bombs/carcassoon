#pragma once

#include <libcarcassonne/consts.h>
#ifdef _WIN32
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#define LIBUTILS_PATH_BUF MAX_PATH
#else
#include <linux/limits.h>
#include <unistd.h>

#define LIBUTILS_PATH_BUF PATH_MAX
#endif

return_code_t current_executable_path(char ret[LIBUTILS_PATH_BUF]);
return_code_t current_executable_dir(char ret[LIBUTILS_PATH_BUF]);
