#include "InputStreamer.hpp"
#include <cstdio>


#define UNUSED(x) ((void)(x))


InputStreamer& InputStreamer::operator>>(const EndLine& a_endl) {
    UNUSED(a_endl);

    char c;
    scanf("%c", &c); // To read the '\n'

    return *this;
}


InputStreamer& InputStreamer::operator>>(unsigned long& a_unsignedLongNumber) {
    scanf("%lu", &a_unsignedLongNumber);

    return *this;
}


InputStreamer& InputStreamer::operator>>(long& a_signedLongNumber) {
    scanf("%ld", &a_signedLongNumber);

    return *this;
}


InputStreamer& InputStreamer::operator>>(unsigned int& a_unsignedNumber) {
    scanf("%u", &a_unsignedNumber);

    return *this;
}


InputStreamer& InputStreamer::operator>>(int& a_signedNumber) {
    scanf("%d", &a_signedNumber);

    return *this;
}


InputStreamer& InputStreamer::operator>>(double& a_floationNumber) {
    scanf("%lf", &a_floationNumber);

    return *this;
}


InputStreamer& InputStreamer::operator>>(char& a_char) {
    scanf("%c", &a_char);

    return *this;
}