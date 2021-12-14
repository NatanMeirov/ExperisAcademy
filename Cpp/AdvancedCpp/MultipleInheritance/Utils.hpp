#ifndef __UTILS_HPP__
#define __UTILS_HPP__


#include <ostream> // std::ostream


template <typename T>
void PrintCollection(const T& a_collection, std::ostream& a_os) {
    auto iteratorEnd = a_collection.end();
	for (auto iterator = a_collection.begin(); iterator != iteratorEnd; ++iterator) {
		a_os << *iterator << " ";
	}
}


#endif // __UTILS_HPP__