#include "../inc/RAMDataBase.hpp"
#include <cstddef> // size_t
#include "../../Infrastructure/inc/Cdr.hpp"


bool nm::cdr::RAMDataBase::Save(const std::string &a_databaseFileName) {
    // TODO
    return true;
}


bool nm::cdr::RAMDataBase::Load(const std::string &a_databaseFileName) {
    // TODO
    return true;
}

nm::cdr::Cdr nm::cdr::RAMDataBase::Get(const size_t& a_query) {
    // TODO
    return Cdr();
}


bool nm::cdr::RAMDataBase::Update(const size_t& a_query) {
    // TODO
    return false;
}


bool nm::cdr::RAMDataBase::Delete(const size_t& a_query) {
    // TODO
    return false;
}


bool nm::cdr::RAMDataBase::Add(const size_t& a_query, const Cdr& _dataToAdd) {
    // TODO
    return true;
}