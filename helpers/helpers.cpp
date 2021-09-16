#include "helpers.hpp"

template<>
int readv<int>(const char* msg) {
    int n;
    do {
        printf("%s", msg);
    } while (scanf_s("%d", &n) != 1);
    return n;
}

template<>
unsigned int readv<unsigned int>(const char* msg) {
    int n;
    do {
        printf("%s", msg);
    } while (scanf_s("%u", &n) != 1);
    return n;
}

template<>
double readv<double>(const char* msg) {
    double n;
    do {
        printf("%s", msg);
    } while (scanf_s("%lf", &n) != 1);
    return n;
}

template<>
float readv<float>(const char* msg) {
    float n;
    do {
        printf("%s", msg);
    } while (scanf_s("%f", &n) != 1);
    return n;    
}