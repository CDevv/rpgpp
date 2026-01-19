#include "fileSystemService.hpp"
#include <nfd.h>
#include <nfd.hpp>

FileSystemService::FileSystemService() {
    NFD_Init();
}

void FileSystemService::unload() {
    NFD_Quit();
}