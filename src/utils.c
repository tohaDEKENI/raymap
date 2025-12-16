#include <monic.h>
#include <stdio.h>

void print_bar(float percentage) {
    int bar_length = (int)(percentage / 100.0 * 50);
    printf("[");
    for (int i = 0; i < bar_length; i++) {
        printf("#");  
    }
    for (int i = bar_length; i < 50; i++) {
        printf(" ");  // Espaces vides
    }
    printf("] %.2f%%\n", percentage);
}