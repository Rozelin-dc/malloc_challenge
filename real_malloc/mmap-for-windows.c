#include "mmap-for-windows.h"

void *mmap_for_windows(HANDLE* map_handle, size_t size) {
  char fname[] = "LAPTOP-CCU0KHN9"; // input your PC name here

  HANDLE handle = CreateFileW((LPCWSTR)fname, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

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
