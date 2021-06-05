#include "InputStreamer.hpp"
#include "OutputStreamer.hpp"


// ---------------------
// Global streamers:
InputStreamer cin;
OutputStreamer cout;
EndLine endl;
Lowercase lowercase;
Uppercase uppercase;
Normalcase normalcase;
// ---------------------


int main() {
    cout << "abc"; // will print "abc" to stdout without newline
    cout << 3; // without new line
    cout << 7; // without new line (Total: abc37)
    cout << endl; // prints a newline to stdout
    cout << 2.17 << endl; // prints 2.17 + newline
    cout << 'a' << 'b' << endl; // prints ab + newline
    cout << 288373616235213 << endl; // prints 288373616235213 + newline
    cout << -288373616235213 << endl; // prints -288373616235213 + newline

    // ADDED AT 14:00:
    cout << uppercase << "abc" << lowercase << "DEF" << normalcase << "HeLLo"  << endl; // prints ABCdefHeLLo + newline

    int i;
    cin >> i;

    // CHANGED AT 14:00:
    char c, k, r;
    cin >> c >> i >> k >> endl >> r; // allows user to input: "a23b"+newline+"m"
                                    // c will contain 'a'
                                    // i will contain '23'
                                    // k will contain 'b'
                                    // r will contain 'm'
    double d;
    cin >> d;

    cout << "Total inputs:" << endl;
    cout << "c: " << c << endl;
    cout << "i: " << i << endl;
    cout << "k: " << k << endl;
    cout << "r: " << r << endl;
    cout << "d: " << d << endl;

    return 0;
}