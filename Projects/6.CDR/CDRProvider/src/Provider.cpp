#include "../inc/Provider.hpp"
#include "fstream"
#include "../../Infrastructure/Network/TCPSocket.hpp"
#include "../../Infrastructure/System/Directory.hpp"
#include <ios>
#include <ostream>


static std::streampos GetFileSize(const std::string& a_filePath) {
    std::streampos fileSize = 0;
    std::ifstream file(a_filePath, std::ios::binary);

    fileSize = file.tellg();
    file.seekg(0, std::ios::end);
    fileSize = file.tellg() - fileSize;

    file.close();

    return fileSize;
}


void nm::cdr::Provider::SendCdrFiles() {
    std::string dirName("ProviderCdrFiles");
    Directory cdrFilesDir(dirName);
    Directory::DirectoryItem singleCdrFile;

    while((singleCdrFile = cdrFilesDir.GetNextItem())) {
        if(singleCdrFile.GetName() == "." || singleCdrFile.GetName() == "..") {
            continue;
        }

        TCPSocket socket(this->m_processorIpAddress, this->m_processorPortNumber);
        std::string filePath = dirName + "/" + singleCdrFile.GetName();

        // Open cdr file
        std::streampos fileSize = GetFileSize(filePath);
        std::ifstream file(filePath, std::ios::binary);

        char* fileContentBuffer = new char[fileSize];
        file.read(fileContentBuffer, fileSize); // Writes the file's bytes to the buffer
        TCPSocket::BytesBufferProxy cdrFileAsBytes(reinterpret_cast<unsigned char*>(fileContentBuffer), fileSize);
        delete[] fileContentBuffer;

        socket.Connect();
        socket.Send(cdrFileAsBytes);
        // TODO: delete the sent file
    }
}