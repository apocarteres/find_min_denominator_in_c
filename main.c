#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

typedef struct {
    int a;
    int b;
} fraction;

void print_dividers(const char *text, const size_t count, const int *divs) {
    printf("%s: ", text);
    for (int i = 0; i < count; ++i) {
        printf("%d", divs[i]);
        if (i == count - 1) {
            printf(" ");
        } else {
            printf(", ");
        }
    }
    printf("\n");
}

fraction multiply(const fraction *f, const int m) {
    return (fraction) {.a = f->a * m, .b = f->b * m};
}

void for_each_divider(const int n, void (*fn)(const int *, void **), int args_len, ...) {
    va_list arguments;
    va_start(arguments, args_len);
    void *fn_args[args_len];
    for (int i = 0; i < args_len; ++i) {
        fn_args[i] = va_arg (arguments, void*);
    }
    va_end (arguments);
    for (int i = 2; i <= n; ++i) {
        if (n % i == 0) {
            fn(&i, fn_args);
        }
    }
}

void count_dividers_fn(const int *_, void **args) {
    size_t *result = (size_t *) *(args + 0);
    (*result)++;
}

size_t count_dividers(const int n) {
    size_t count = 0;
    for_each_divider(n, count_dividers_fn, 1, &count);
    return count;
}

void find_dividers_fn(const int *n, void **args) {
    int *result = (int *) *(args + 0);
    size_t *result_length = (size_t *) *(args + 1);
    size_t *counter = (size_t *) *(args + 2);
    assert(*counter <= *result_length);
    *(result + *counter) = *n;
    (*counter)++;
}

void find_dividers(const int n, int *result, size_t result_length) {
    size_t counter = 0;
    for_each_divider(n, find_dividers_fn, 3, result, &result_length, &counter);
}

int find_max_common_div(int *f1_divs, const size_t f1_div_count, int *f2_divs, const size_t f2_div_count) {
    int *f1_p = f1_divs + f1_div_count - 1;
    for (size_t i = 0; i < f1_div_count; ++i) {
        int *f2_p = f2_divs + f2_div_count - 1;
        for (size_t j = 0; j < f2_div_count; ++j) {
            if (*f1_p == *f2_p) {
                return *f1_p;
            }
            --f2_p;
        }
        --f1_p;
    }
    return -1;
}

void solve(const fraction *f1, const fraction *f2) {
    const size_t f1_div_count = count_dividers(f1->b);
    const size_t f2_div_count = count_dividers(f2->b);
    int f1_divs[f1_div_count];
    int f2_divs[f2_div_count];
    find_dividers(f1->b, f1_divs, f1_div_count);
    find_dividers(f2->b, f2_divs, f2_div_count);
    print_dividers("f1 dividers", f1_div_count, f1_divs);
    print_dividers("f2 dividers", f2_div_count, f2_divs);
    int f1_m;
    int f2_m;
    int mcd = find_max_common_div(f1_divs, f1_div_count, f2_divs, f2_div_count);
    if (mcd == -1) {
        f1_m = f2->b;
        f2_m = f1->b;
        printf("no common denominator, let's just mutually multiply\n");
    } else {
        int common_d = (f1->b * f2->b) / mcd;
        f1_m = common_d / f1->b;
        f2_m = common_d / f2->b;
        printf("max common divider is: %d\n", mcd);
        printf("min common denominator is: %d\n", common_d);
    }
    const fraction f1_d = multiply(f1, f1_m);
    const fraction f2_d = multiply(f2, f2_m);
    printf("%d/%d * %d = %d/%d\n", f1->a, f1->b, f1_m, f1_d.a, f1_d.b);
    printf("%d/%d * %d = %d/%d\n", f2->a, f2->b, f2_m, f2_d.a, f2_d.b);
}

fraction parse_fraction(const char *str) {
    char buf[strlen(str)];
    strcpy(buf, str);
    char *next_token = strtok(buf, "/");
    if (!next_token) {
        printf("%s doesn't seems to be a fraction\n", buf);
        exit(1);
    }
    int a = (int) strtol(next_token, NULL, 10);
    next_token = strtok(NULL, "/");
    if (!next_token) {
        printf("%s doesn't seems to be a fraction\n", buf);
        exit(1);
    }
    int b = (int) strtol(next_token, NULL, 10);
    if (a <= 0 || b <= 0) {
        printf("%s doesn't seems to be a valid fraction\n", str);
        exit(1);
    }
    return (fraction) {.a = a, .b = b};
}

int main(int argc, char **argv) {
    char *first_arg = *(++argv);
    char *second_arg = *(++argv);
    fraction f1 = parse_fraction(first_arg);
    fraction f2 = parse_fraction(second_arg);
    printf("f1: %d/%d\n", f1.a, f1.b);
    printf("f2: %d/%d\n", f2.a, f2.b);
    solve(&f1, &f2);
    return 0;
}
