#include "../Thread.hpp"
#include <iostream>
#include <unistd.h>


void* FirstTask(void* _notInUse) {
    std::cout << "Hello " << std::endl;
    return nullptr;
}


void* SecondTask(void* _name) {
    std::cout << ((const char*)_name) << std::endl;
    return _name;
}


void* UnlimitedTask(void* _none) {
    while(true) {
        std::cout << "*" << std::endl;
        usleep(100000);
    }
}


int main() {
    const char* name = "Natan Meirov";
    nm::Thread thread1(&FirstTask);
    nm::Thread thread2(&SecondTask, (void*)name);
    nm::Thread thread3(&UnlimitedTask, (void*)name);

    std:: cout << thread1.Join() << std::endl;
    std:: cout << thread1.Join() << std::endl;
    std:: cout << (const char*)thread2.Join() << std::endl;
    thread3.Detach();

    sleep(5);

    return 0;
}
