#include <monic.h>
#include <stdio.h>

/* ===== CPU ===== */

float get_cpu_usage(){
        FILE *f_cpu = fopen("/proc/stat", "r");
        if (!f_cpu) {
            perror("fopen");
            return 1;
        }

        char line[256];
        fgets(line, sizeof(line), f_cpu);
        fclose(f_cpu);

        long user, nice, system, idle;
        sscanf(line, "cpu %ld %ld %ld %ld", &user, &nice, &system, &idle);

        long total = user + nice + system + idle;
        float cpu_usage = 100.0 * (total - idle) / total;

        return cpu_usage;
}