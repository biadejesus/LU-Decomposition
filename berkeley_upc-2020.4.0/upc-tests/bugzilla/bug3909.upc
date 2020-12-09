#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <upc.h>

#define LEN 64

// This is the work-around
#if defined(__BERKELEY_UPC_RUNTIME__) && !defined(BUPC_DISABLE_EXTENSIONS)
  #undef system
  #define system(cmd) bupc_system(cmd)
#endif

shared char *buffer;

int main() {
    buffer = upc_all_alloc(LEN, THREADS);

    // If these lines are moved to after the upc_memget
    // below, then it works in all cases.
    char *cmd = malloc(LEN);
    strcpy(cmd, "/bin/true");
    if (access(cmd, X_OK)) strcpy(cmd, "/usr/bin/true");
    if (access(cmd, X_OK)) strcpy(cmd, "true");

    char *dummy = malloc(LEN);
    upc_memget(dummy, buffer, LEN);
    
    int rv = system(cmd);
    puts(rv ? "FAIL" : "PASS");

    return 0;
}
