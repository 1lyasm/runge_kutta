#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum { Debug, Normal, Error } Mode;

typedef struct {
    int *coefs;
    int n;
    double t;
} De;

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

static void *fmalloc(unsigned n) {
    void *allocated = malloc(n);
    if (allocated == NULL) {
        fail("fmalloc: malloc failed");
    }
    return allocated;
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

static int takeInt(Mode mode, char *fmt, ...) {
    int inp;
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    scanf(" %d", &inp);
    debug(mode, "takeInt: inp: %d\n", inp);
    va_end(args);
    return inp;
}

static De *initDe(Mode mode) {
    int i;
    De *de = fmalloc(sizeof(De));
    de->n = takeInt(mode, "Enter n: ");
    while (de->n < 2) {
        de->n = takeInt(mode, "N can not be less than 2, try again: ");
    }
    de->coefs = fmalloc((unsigned)de->n * sizeof(int));
    printf(
        "Differential equation is in this form: \n\tc0 * y’ = c1 * y + c2 * "
        "x^(n - "
        "2) + c3 * x^(n - 3) + c4 * x^(n - 4) + …\n");
    for (i = 0; i < de->n; ++i) {
        de->coefs[i] = takeInt(mode, "Enter coefficient c%d: ", i);
        while (i <= 1 && de->coefs[i] == 0) {
            de->coefs[i] =
                takeInt(mode, "c0 and c1 can not be zero, try again: ");
        }
    }
    printf("Enter independent variable (x) value: ");
    scanf(" %lf", &(de->t));
    return de;
}

static void debugDe(De *de, Mode mode) {
    int i;
    debug(mode, "debugDe: de: \n");
    debug(mode, "\tn: %d\n", de->n);
    for (i = 0; i < de->n; ++i) {
        debug(mode, "\tcoefs[%d]: %d\n", i, de->coefs[i]);
    }
    debug(mode, "\tt: %lf\n", de->t);
}

static void freeDe(De *de) {
    free(de->coefs);
    free(de);
}

int main() {
    Mode mode = initMode();
    De *de = initDe(mode);
    debugDe(de, mode);
    freeDe(de);
    return 0;
}
