#include<cstdio>
#define EXPORT __attribute__((visibility("default")))
extern "C" double print(double x) {
    fprintf(stdout, "%f\n",x);
    return 0;
}