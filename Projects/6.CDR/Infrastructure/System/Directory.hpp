#ifndef __NM_DIRECTORY_HPP__
#define __NM_DIRECTORY_HPP__


#include <dirent.h> // Directory functions
#include <string>


namespace nm {

class Directory {
public:
    class DirectoryItem {
    public:
        DirectoryItem(struct dirent* a_directoryEntry) : m_directoryEntry(a_directoryEntry) {}

        bool operator==(const std::nullptr_t& a_nullptr) { return this->m_directoryEntry == nullptr; }
        bool operator!=(const std::nullptr_t& a_nullptr) { return this->m_directoryEntry != nullptr; }
        std::string GetName() const {return std::string(this->m_directoryEntry->d_name); }

    private:
        struct dirent* m_directoryEntry;
    };

    Directory(const std::string& a_directoryPath);
    ~Directory();

    DirectoryItem GetNextItem(); // Returns nullptr (as DirectoryItem) when there are no more items in the directory [already READ al the directory item], open NEW directory (even with the SAME DIR PATH) to read once again the directory items again

private:
    DIR* m_directory;
};

} // nm


#endif // __NM_DIRECTORY_HPP__