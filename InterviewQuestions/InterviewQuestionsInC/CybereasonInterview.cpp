#include <iostream> // std::cout, std::endl
#include <string> // std::string
#include <list> // std::list

bool IsDirectory(const std::string& a_path);
std::list<std::string> GetDirectoryEntries(const std::string& a_path);



void FetchDirectoriesTree(const std::string& a_path) {
	std::cout << a_path << std::endl;
	
	if(IsDirectory(a_path)) {
		std::list<std::string> directoryEntries = GetDirectoryEntries(const std::string& a_path);
		std::list<std::string>::iterator itr = directoryEntries.begin();
		std::list<std::string>::iterator endItr = directoryEntries.end();

		while(itr != endItr) {
			FetchDirectoriesTree(*itr);
			++itr;
		}
	}
}

// Iterative approach: using Stack to push the path
