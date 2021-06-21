#include "Directory.hpp"
#include <dirent.h> // Directory functions
#include <stdexcept> // std::logic_error


nm::Directory::Directory(const std::string& a_directoryPath)
: m_directory(opendir(a_directoryPath.c_str())) {
    if(!m_directory) {
        throw std::logic_error(std::string("Failed to open directory: ") + a_directoryPath);
    }
}


nm::Directory::~Directory() {
    closedir(this->m_directory);
}


nm::Directory::DirectoryItem nm::Directory::GetNextItem() {
    return DirectoryItem(readdir(this->m_directory));
}