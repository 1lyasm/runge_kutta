#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { Debug, Normal, Error } Mode;

static void debug(Mode mode, const char *fmt, ...) {
    if (mode == Debug) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}

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
    debug(mode, "initMode: mode: %d\n", mode);
    return mode;
}

static int takeInt(char *msg, Mode mode) {
    int inp;
    printf("%s", msg);
    scanf(" %d", &inp);
    debug(mode, "takeInt: inp: %d\n", inp);
    return inp;
}

int main() {
    Mode mode = initMode();
    int n = takeInt("Enter n: ", mode);
    return 0;
}
