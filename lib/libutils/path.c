#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L // NOLINT(bugprone-reserved-identifier)
#endif

#ifndef _WIN32
#endif
#include <libutils/path.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef _WIN32
#include <shlwapi.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#define LIBUTILS_DIR_DELIMITER "\\"
#else
#include <libgen.h>
#include <stdio.h>
#define LIBUTILS_DIR_DELIMITER "/"
#endif

return_code_t current_executable_path(char ret[LIBUTILS_PATH_BUF]) {
  ssize_t len;
#ifdef _WIN32
  len = GetModuleFileNameA(NULL, ret, MAX_PATH);
  if (len == 0) {
    printf("GetModuleFileName failed. Error: %lu\n", GetLastError());
    return ERROR;
  } else if (len == MAX_PATH) {
    printf("Path truncated (buffer too small).\n");
    return ERROR;
  }
#else

  len = readlink("/proc/self/exe", ret, LIBUTILS_PATH_BUF - 1);
  if (len == -1) {
    perror("readlink failed");
    return ERROR;
  }
  ret[len] = '\0';
#endif

  return SUCCESS;
}

return_code_t current_executable_dir(char ret[LIBUTILS_PATH_BUF]) {
  char path[LIBUTILS_PATH_BUF];
  if (current_executable_path(path) != SUCCESS) {
    return ERROR;
  }
  char* out = path;

#ifdef _WIN32
  PathRemoveFileSpecA(out);
#else
  out = dirname(path);
#endif
  strcpy(ret, out);

  return SUCCESS;
}

return_code_t create_path_resolver(path_resolver_t* resolver) {
  current_executable_dir(resolver->base);
  resolver->size = strlen(resolver->base);

  return SUCCESS;
}

char* path_resolver_resolve(path_resolver_t* resolver, char* file) {
  unsigned int size = resolver->size + strlen(file) + 2;
  char*        ret  = calloc(size, sizeof(char));

  snprintf(ret, size, "%s" LIBUTILS_DIR_DELIMITER "%s", resolver->base, file);
  return ret;
}
