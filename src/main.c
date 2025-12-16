#include <stdio.h>
#include <monic.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
	while(1){
		system("clear");
		printf("===== MINI SYSTEM MONITOR =====\n");
		printf("CPU ");
		print_bar(get_cpu_usage());
		printf("RAM ");
		print_bar(get_ram_usage());
		sleep(1);
	}
	return 0;
}

// gcc src/*.c -o prog -I include