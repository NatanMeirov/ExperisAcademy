#include "String.hpp"
#include <iostream> // std::cout, std::endl


void Foo() {
    String str1("hello");
    str1.Print(); // will print "hello"


    const String *str2 = new String("world");
    str2->Print(); // will print "world"
    std::cout << std::endl;

    str1.operator+=(*str2);
    str1.Print(); // will print "helloworld"
    std::cout << std::endl;

    str2->Print(); // will print "world"
    std::cout << std::endl;

    str1.operator+=(*str2);
    str1.Print(); // will print "helloworldworld"
    std::cout << std::endl;

    String str3(" ");
    str3[0] = ',';
    str1 += str3;
    str1.operator+=(str1);
    str1.Print(); // will print "helloworldworld,helloworldworld,"
    std::cout << std::endl;

    delete str2;
}


int main() {
    Foo();

    return 0;
}