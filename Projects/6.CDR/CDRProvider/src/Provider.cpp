#include "../inc/Provider.hpp"
#include <fstream>
#include <ios>
#include <memory> // std::shared_ptr
#include <unistd.h> // sleep
#include "../../Infrastructure/Network/TCPSocket.hpp"
#include "../../Infrastructure/System/Directory.hpp"
#include "../../Infrastructure/Multithreaded/TThreadPool.hpp"



static std::streampos GetFileSize(const std::string& a_filePath) {
    std::streampos fileSize = 0;
    std::ifstream file(a_filePath, std::ios::binary);

    fileSize = file.tellg();
    file.seekg(0, std::ios::end);
    fileSize = file.tellg() - fileSize;

    file.close();

    return fileSize;
}


void nm::cdr::Provider::Run() const {
    while(true) { // Polling
        this->ProvideFiles();
        sleep(this->SleepingAmount());
    }
}


// Async ProvideFiles() - Not needed...
// void nm::cdr::Provider::ProvideFiles() const {
//     auto sendFileTask = [this](const std::string a_fullFilePath) {
//         this->m_fileFormatter->Format(a_fullFilePath);

//         TCPSocket socket(this->m_processorIpAddress, this->m_processorPortNumber);

//         // Open cdr file
//         std::streampos fileSize = GetFileSize(a_fullFilePath);
//         std::ifstream file(a_fullFilePath, std::ios::binary);

//         char* fileContentBuffer = new char[fileSize];
//         file.read(fileContentBuffer, fileSize); // Writes the file's bytes to the buffer
//         TCPSocket::BytesBufferProxy cdrFileAsBytes(reinterpret_cast<unsigned char*>(fileContentBuffer), fileSize);
//         delete[] fileContentBuffer;

//         socket.Connect();
//         socket.Send(cdrFileAsBytes);

//         // TODO: Add support at deleting the sent file
//     };

//     compiletime::ThreadPool<std::string, decltype(sendFileTask)> threadPool(Provider::THREADS_NUMBER, Provider::WORKING_TASKS_QUEUE_SIZE, sendFileTask);
//     Directory cdrFilesDir(this->m_cdrFilesDirectoryPath);
//     Directory::DirectoryItem singleCdrFile;

//     while((singleCdrFile = cdrFilesDir.GetNextItem())) {
//         if(singleCdrFile.GetName() == "." || singleCdrFile.GetName() == "..") {
//             continue;
//         }

//         std::string fullFilePath = this->m_cdrFilesDirectoryPath + "/" + singleCdrFile.GetName();
//         threadPool.PushWork(fullFilePath);
//     }
// }


void nm::cdr::Provider::ProvideFiles() const {
    Directory cdrFilesDir(this->m_cdrFilesDirectoryPath);
    Directory::DirectoryItem singleCdrFile;

    while((singleCdrFile = cdrFilesDir.GetNextItem())) {
        if(singleCdrFile.GetName() == "." || singleCdrFile.GetName() == "..") {
            continue;
        }

        std::string fullFilePath = this->m_cdrFilesDirectoryPath + "/" + singleCdrFile.GetName();

        this->m_fileFormatter->Format(fullFilePath);

        TCPSocket socket(this->m_processorIpAddress, this->m_processorPortNumber);

        // Open cdr file
        std::streampos fileSize = GetFileSize(fullFilePath);
        std::ifstream file(fullFilePath, std::ios::binary);

        char* fileContentBuffer = new char[fileSize];
        file.read(fileContentBuffer, fileSize); // Writes the file's bytes to the buffer
        TCPSocket::BytesBufferProxy cdrFileAsBytes(reinterpret_cast<unsigned char*>(fileContentBuffer), fileSize);
        delete[] fileContentBuffer;

        socket.Connect();
        socket.Send(cdrFileAsBytes);

        // TODO: Add support at deleting the sent file
    }
}