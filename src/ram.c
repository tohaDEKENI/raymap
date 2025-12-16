#include <monic.h>
#include <stdio.h>

/* ===== RAM ===== */

float get_ram_usage()
{
    FILE *f_mem = fopen("/proc/meminfo", "r");
    if (!f_mem) {
        perror("fopen");
        return 1;
    }
    char line[256];
    long mem_total = 0;
    long mem_free = 0;

    while (fgets(line, sizeof(line), f_mem)) {
        if (sscanf(line, "MemTotal: %ld kB", &mem_total) == 1) {}
        if (sscanf(line, "MemAvailable: %ld kB", &mem_free) == 1) {}
    }
    fclose(f_mem);

    float ram_usage = 100.0 * (mem_total - mem_free) / mem_total;

    return ram_usage;
}