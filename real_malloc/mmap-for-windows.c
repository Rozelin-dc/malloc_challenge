#include "mmap-for-windows.h"

void *mmap_for_windows(char* fpath, HANDLE* map_handle, size_t size) {
  int size;
  wchar_t fname[80];
  int len;
  char* p = fpath;
  int i = 0;
  while(*p != '\0')  {
    mbtowc(&fname[i++], p, MB_CUR_MAX);
    p++;
  }
  fname[i] = '\0';

  HANDLE handle = CreateFileW(fname, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

  if(handle == INVALID_HANDLE_VALUE) {
    fprintf(stderr, "Failed to open file.\n");
    exit(1);
  }

  size = GetFileSize(handle, 0);
  *map_handle = CreateFileMapping(handle, 0, PAGE_READWRITE, 0, 0, NULL);
  return (void *)MapViewOfFile(*map_handle, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, size);
}

void munmap_for_windows(char** inp, HANDLE* map_handle) {
  UnmapViewOfFile(*inp);
  if(*map_handle != INVALID_HANDLE_VALUE) {
    CloseHandle(*map_handle);
    *map_handle = INVALID_HANDLE_VALUE;
  }
  *inp = NULL;
}
