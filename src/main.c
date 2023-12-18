#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { Debug, Normal, ModeError } Mode;

typedef enum { Sin, Cos, Log, Exp, Pol, FuncTError } FuncT;

typedef struct {
    int *coefs;
    FuncT *types;
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
    Mode mode = ModeError;
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

static int inpInt(Mode mode, char *fmt, ...) {
    int inp;
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    scanf(" %d", &inp);
    debug(mode, "inpInt: inp: %d\n", inp);
    va_end(args);
    return inp;
}

static char *funcTToStr(FuncT funcT) {
    unsigned nMaxChars = 32;
    char *str = fmalloc(nMaxChars * sizeof(char));
    if (funcT == Sin) {
        strcpy(str, "sine");
    } else if (funcT == Cos) {
        strcpy(str, "cosine");
    } else if (funcT == Log) {
        strcpy(str, "logarithmic");
    } else if (funcT == Exp) {
        strcpy(str, "exponential");
    } else if (funcT == Pol) {
        strcpy(str, "polynomial");
    } else {
        fail("funcTToStr: funcT is in invalid state");
    }
    return str;
}

static FuncT charToFuncT(char c) {
    FuncT funcT = FuncTError;
    if (c == 's') {
        funcT = Sin;
    } else if (c == 'c') {
        funcT = Cos;
    } else if (c == 'l') {
        funcT = Log;
    } else if (c == 'e') {
        funcT = Exp;
    } else if (c == 'p') {
        funcT = Pol;
    } else {
        funcT = FuncTError;
        fail("charToFuncT: funcT is in invalid state");
    }
    return funcT;
}

static FuncT inpFuncT() {
    char resp;
    printf(
        "Enter function type ('s' - sine, 'c' - cosine, 'l' - "
        "logarithmic, 'e' - exponential, 'p' - polynomial): ");
    scanf(" %c", &resp);
    while (resp != 's' && resp != 'c' && resp != 'l' && resp != 'e' &&
           resp != 'p') {
        printf("Invalid function type, try again: ");
        scanf(" %c", &resp);
    }
    return charToFuncT(resp);
}

static De *initDe(Mode mode) {
    int i;
    De *de = fmalloc(sizeof(De));
    de->n = inpInt(mode, "Enter n: ");
    while (de->n < 2) {
        de->n = inpInt(mode, "N can not be less than 2, try again: ");
    }
    de->coefs = fmalloc((unsigned)(de->n) * sizeof(int));
    de->types = fmalloc((unsigned)(de->n) * sizeof(FuncT));
    printf(
        "Differential equation is in this form: \n\tc0 * y' = c1 * y + "
        "P(x)\n\tP(x) = c2 * f(x) + c3 * g(x) + c4 * h(x) + …\n");
    for (i = 0; i < de->n; ++i) {
        de->coefs[i] = inpInt(mode, "Enter coefficient c%d: ", i);
        while (i <= 1 && de->coefs[i] == 0) {
            de->coefs[i] =
                inpInt(mode, "c0 and c1 can not be zero, try again: ");
        }
        if (i > 1) {
            de->types[i] = inpFuncT();
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
    for (i = 2; i < de->n; ++i) {
        char *asStr = funcTToStr(de->types[i]);
        debug(mode, "\ttypes[%d]: %s\n", i, asStr);
        free(asStr);
    }
    debug(mode, "\tt: %lf\n", de->t);
}

static void freeDe(De *de) {
    free(de->coefs);
    free(de->types);
    free(de);
}

int main() {
    Mode mode = initMode();
    De *de = initDe(mode);
    debugDe(de, mode);
    freeDe(de);
    return 0;
}
