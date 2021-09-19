#include "pthread.h"
#include <iostream>

namespace // no name, i.e. anonymous for file scope
          // this is the C++ way for decorator STATIC
{
void* start_routine(void*) {
    std::cout << "-- Hello from POSIX Thread" << std::endl;
    return 0; // calls pthread_exit()
}
} // namespace

int main() {
    pthread_t thread;
    int rc;
    void* retval;

    std::cout << "-- starting POSIX Thread" << std::endl;

    rc = pthread_create(&thread, NULL, &start_routine, NULL);
    if (rc != 0) {
        std::cerr << "Error! unable to create thread, " << rc << std::endl;
        exit(1);
    }

    rc = pthread_join(thread, &retval);
    if (rc != 0) {
        std::cerr << "Error! Unable to join thread with rc=" << rc << std::endl;
        exit(1);
    }
    if (retval != NULL) {
        std::cerr << "Error! Thread failed with retval=" << retval << std::endl;
        exit(1);
    }

    return 0;
}
