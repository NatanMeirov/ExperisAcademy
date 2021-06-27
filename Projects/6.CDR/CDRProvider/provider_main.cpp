// A temporary executable to test the providers send and to supply cdr files to the processor

#include <iostream>
#include "inc/Provider.hpp"
#include "inc/EmptyFormatter.hpp"

#define LOOPBACK "127.0.0.1"
#define PROCESSOR_LISTENER_IP LOOPBACK
#define PROCESSOR_LISTENER_PORT 4040
#define PROVIDE_FILES_AMOUNT_IN_MINUTES 5


int main() {
    nm::cdr::Provider provider(PROCESSOR_LISTENER_IP, PROCESSOR_LISTENER_PORT, new EmptyFormatter(), "ProviderCdrFiles", PROVIDE_FILES_AMOUNT_IN_MINUTES);
    provider.Run();

    return 0;
}