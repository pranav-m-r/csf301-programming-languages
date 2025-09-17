#include <stdio.h>
#include <string.h>

void test_c_array_bounds() {
    int a[11][3];
    
    memset(a, 0, sizeof(a));
    
    printf("Array declared as int a[11][3]\n");
    printf("Valid indices: a[0][0] to a[10][2]\n");
    printf("Array size: %zu bytes\n", sizeof(a));
    printf("Array layout in memory:\n");
    
    for(int i = 0; i < 11; i++) {
        for(int j = 0; j < 3; j++) {
            printf("a[%d][%d] at address: %p\n", i, j, (void*)&a[i][j]);
        }
    }
    
    printf("\nExecuting: a[12][1] = 1;\n");
    
    int *base = (int*)a;
    printf("This accesses element at offset: 12*3 + 1 = %d\n", 12*3 + 1);
    printf("Which is %d elements beyond the array!\n", (12*3 + 1) - (11*3));
    
    a[12][1] = 1;
    
    printf("Statement executed (undefined behavior occurred)\n");
    
    printf("Checking if any valid array elements changed:\n");
    for(int i = 0; i < 11; i++) {
        for(int j = 0; j < 3; j++) {
            if(a[i][j] != 0) {
                printf("a[%d][%d] = %d\n", i, j, a[i][j]);
            }
        }
    }
}

int main() {
    test_c_array_bounds();
    return 0;
}