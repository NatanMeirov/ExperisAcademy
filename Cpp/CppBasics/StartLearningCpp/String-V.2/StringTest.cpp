#include "String.hpp"
#include <iostream> // std::cout, std::endl

void Foo() {
    String s1("hello");
    const String cs2("bOoM");

    if (s1 == cs2) { std::cout << "Equal" << std::endl; }
    if (cs2 != s1) { std::cout << "Not Equal" << std::endl; }
    if (cs2 > cs2) { std::cout << "Is Greater" << std::endl; }
    if (s1 < cs2) { std::cout << "Is Smaller" << std::endl; }
    if (s1 >= cs2) { std::cout << "Is Greater or Equal" << std::endl; }
    if (cs2 <= s1) { std::cout << "Is Smaller or Equal" << std::endl; }

    const String cs3 = s1 + cs2; // result is a string that contains "hellobOoM"
    cs3.Print(); // -> "hellobOoM"
    std::cout << std::endl;

    const String toRemove("Oo");
    const String s4 = cs2 - toRemove; // result is a string that contains "bO"
    s4.Print(); // -> "bM"
    std::cout << std::endl;

    String test("ABCDEFGHIGKLMNOPQRSTUVWXYZ");
    String testToRemove("FGHIGKLMNOP");
    const String testResult = test - testToRemove;
    testResult.Print(); // -> "ABCDEQRSTUVWXYZ"
    std::cout << std::endl;

    s1 += String("world"); //  result is to change s1 to contain "helloworld"
    s1.Print(); // -> "helloworld"
    std::cout << std::endl;

    ++s1; // result is to change s1 to contain "helloworld!"
    ++s1; // result is to change s1 to contain "helloworld!!"
    s1++; // result is to change s1 to contain "helloworld!!!" (but what is returned???)
    s1.Print(); // -> "helloworld!!!"
    std::cout << std::endl;

    (cs3 << 3).Print(); // -> "lobOoM" // result is a string that contains "lobOoM"
    std::cout << std::endl;

    (cs3 >> 2).Print(); // -> "hellobO" // result is a string that contains "hellobO"
    std::cout << std::endl;

    char c1 = cs2[3];
    std::cout << c1 << std::endl; // -> "M"
    s1[1] = 'x';
    s1.Print(); // -> "hxlloworld!!!"
    std::cout << std::endl;

    const String plusString(+s1); // returns a string where all characters are replaced by a '+' ("+++++++++++++")
    const String minusString(-cs2); // returns a string where all characters are replaced by a '-' ("----")
    plusString.Print(); // -> "+++++++++++++"
    std::cout << std::endl;
    minusString.Print(); // -> "----"
    std::cout << std::endl;

    const String switchedCasesString(!cs2); // returns a string where the case of each letter is switched: "BoOm"
    switchedCasesString.Print(); // -> "BoOm"
    std::cout << std::endl;
}


int main() {

    Foo();
    return 0;
}