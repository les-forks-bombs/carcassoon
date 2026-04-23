
#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif

#include <libcarcassonne/consts.h>
#include <libutils/path.h>
#include <string.h>

#ifdef _WIN32
#include <shlwapi.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#else
#include <libgen.h>
#include <linux/limits.h>
#include <stdio.h>
#include <unistd.h>
#endif

return_code_t current_executable_path(char path[LIBUTILS_PATH_BUF]) {
  ssize_t len;
#ifdef _WIN32
  len = GetModuleFileNameA(NULL, path, MAX_PATH);
  if (len == 0) {
    printf("GetModuleFileName failed. Error: %lu\n", GetLastError());
    return ERROR;
  } else if (len == MAX_PATH) {
    printf("Path truncated (buffer too small).\n");
    return ERROR;
  }
#else

  len = readlink("/proc/self/exe", path, LIBUTILS_PATH_BUF - 1);
  if (len == -1) {
    perror("readlink failed");
    return ERROR;
  }
  path[len] = '\0';
#endif

  return SUCCESS;
}

return_code_t current_executable_dir(char ret[LIBUTILS_PATH_BUF]) {
  char path[LIBUTILS_PATH_BUF];
  if (current_executable_path(path) != SUCCESS) return ERROR;
  char* out = path;

#ifdef _WIN32
  PathRemoveFileSpecA(out);
#else
  out = dirname(path);
#endif
  strcpy(ret, out);

  return SUCCESS;
}
