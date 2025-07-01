#include <stdio.h>
#include <stdlib.h>

int main() {
    char *addr = getenv("SHELLCODE");
    if (addr) {
        printf("%p\n", addr);
    } else {
        printf("Address not found\n");
    }
    return 0;
}