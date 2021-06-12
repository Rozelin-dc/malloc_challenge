#include <stdio.h>
#include <windows.h>

void *mmap_for_windows(char* fpath, HANDLE* map_handle, size_t size);
void munmap_for_windows(char** inp, HANDLE* map_handle);
