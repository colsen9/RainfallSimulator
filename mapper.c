#include <stdio.h>
#include "data.h"

int main() {
    int result = validate();

    if (result == 0) {
        printf("VALID\n");
    } else {
        printf("%d\n", result);
    }

    return 0;
}














