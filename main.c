#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
    int a;
    int b;
} fraction;

void print_div(const char *text, const size_t count, const int *primes) {
    printf("%s: ", text);
    for (int i = 0; i < count; ++i) {
        printf("%d", primes[i]);
        if (i == count - 1) {
            printf(" ");
        } else {
            printf(", ");
        }
    }
    printf("\n");
}

void print_fraction(const fraction *f) {
    printf("%d/%d", f->a, f->b);
}

void print_fraction_n(const fraction *f) {
    print_fraction(f);
    printf("\n");
}

fraction multiply(const fraction *f, const int m) {
    return (fraction) {.a = f->a * m, .b = f->b * m};
}

void print_result(const fraction *f1, const fraction *f2) {
    printf("result: ");
    print_fraction(f1);
    printf(", ");
    print_fraction_n(f2);
}

void handle_mutual_dividable(const fraction *f1, const fraction *f2) {
    int d = f1->b / f2->b;
    fraction f2_d = multiply(f2, d);
    print_result(f1, &f2_d);
}

char is_prime(int n) {
    for (int i = 2; i < n; ++i) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

size_t count_dividers(const int n) {
    size_t count = 0;
    for (int i = 2; i <= n; ++i) {
        if (!is_prime(i)) {
            continue;
        }
        if (n % i == 0) {
            ++count;
        }
    }
    return count;
}

void find_dividers(const int n, int *result, size_t result_length) {
    for (int i = 2; i <= n; ++i) {
        if (!is_prime(i)) {
            continue;
        }
        if (n % i == 0) {
            assert(result_length >= 0);
            *result = i;
            ++result;
            --result_length;
        }
    }
}

void handle_find_div(const fraction *f1, const fraction *f2) {
    const size_t f1_div_count = count_dividers(f1->b);
    const size_t f2_div_count = count_dividers(f2->b);
    int f1_divs[f1_div_count];
    int f2_divs[f2_div_count];
    find_dividers(f1->b, f1_divs, f1_div_count);
    find_dividers(f2->b, f2_divs, f2_div_count);
    print_div("f1 dividers", f1_div_count, f1_divs);
    print_div("f2 dividers", f2_div_count, f2_divs);
    int f1_m = f2_divs[f2_div_count - 1];
    int f2_m = f1->b * f1_m / f2->b;
    fraction f1_d = multiply(f1, f1_m);
    fraction f2_d = multiply(f2, f2_m);
    print_result(&f1_d, &f2_d);
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
    printf("first fraction: %d/%d\n", f1.a, f1.b);
    printf("second fraction: %d/%d\n", f2.a, f2.b);
    if (f1.b == f2.b) {
        print_result(&f1, &f2);
        return 0;
    }
    if (f1.b % f2.b == 0) {
        handle_mutual_dividable(&f1, &f2);
        return 0;
    }
    if (f2.b % f1.b == 0) {
        handle_mutual_dividable(&f2, &f1);
        return 0;
    }
    if (f1.b < f2.b) {
        handle_find_div(&f1, &f2);
        return 0;
    }
    if (f1.b > f2.b) {
        handle_find_div(&f2, &f1);
        return 0;
    }
    return 0;
}
