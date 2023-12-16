#include <stdio.h>
#include <stdlib.h>

typedef enum { Debug, Normal, Error } Mode;

static void fail(char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

static Mode initMode() {
    Mode mode = Error;
    char resp;
    printf("Enter running mode ('n' -> normal, 'd' -> debug): ");
    scanf(" %c", &resp);
    while (resp != 'n' && resp != 'd') {
        printf("Invalid input, try again: ");
        scanf(" %c", &resp);
    }
    if (resp == 'n') {
        mode = Normal;
    } else if (resp == 'd') {
        mode = Debug;
    } else {
        fail("initMode: mode is in invalid state");
    }
    return mode;
}

static int takeInt(char *msg) {
    int inp;
    printf("%s", msg);
    scanf(" %d", &inp);
    return inp;
}

int main() {
    Mode mode = initMode();
    int n = takeInt("Enter n: ");
    return 0;
}
