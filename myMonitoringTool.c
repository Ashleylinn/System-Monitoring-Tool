#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>

double cpuUtilization(long *preInitial, long *preTotal){
    FILE *fp = fopen("/proc/stat", "r"); //using this system file to calculate the cpu usage 
    if(fp == NULL){
        perror("Cannot open /proc/stat");
        return -1.0;
    }

    char buffer[256];
    if(fgets(buffer, sizeof(buffer), fp) == NULL){
        perror("Error getting /proc/stat");
        fclose(fp);
        return -1.0;
    }
    fclose(fp);

    
    long user, nice, sys, idle, IOwait, irq, softirq, st;
    sscanf(buffer, "cpu %ld %ld %ld %ld %ld %ld %ld %ld", &user, &nice, &sys, &idle, &IOwait, &irq, &softirq, &st);

    long initial = user + nice + sys + irq + softirq + st;
    long total = initial + idle + IOwait;

    if(*preTotal == 0 && *preInitial == 0){
        *preInitial = initial;
        *preTotal = total;
        return 0.0;
    }

    //calculate the differences
    long changeInitial = initial - *preInitial;
    long changeTotal = total - *preTotal;

    if(changeTotal == 0){
        return 0.0;
    } 

    *preInitial = initial;
    *preTotal = total;

    double CPU_usage = (100.0 * changeInitial) / changeTotal;
    return CPU_usage;
}

void Memory_graph(int samples, int tdelay){
    struct sysinfo info;
    int Width = samples;
    int Height = 10;
    int position = 0;//initialize to start at the first column
    int memory_data[samples];

    //initialize the arrays
    for(int i = 0; i < samples; i++){
        memory_data[i] = -1;
    }

    for(int i = 0; i<samples; i++){
        printf("\033[H\033[J"); //clearing the terminal in each iteration

        sysinfo(&info);
        double total_memory = info.totalram / (1024.0 * 1024.0 * 1024.0);
        double free_memory = info.freeram / (1024.0 * 1024.0 * 1024.0);
        double used_memory = total_memory - free_memory; 

        int memory_y_position = (int)((used_memory / total_memory) * Height);
        memory_data[position] = memory_y_position; //storing the current position for memory

        printf("Nbr of samples: %d -- every %d microSecs (%0.2f secs)\n\n", samples, tdelay, (tdelay / 1000000.0));
        printf("V Memory");
        printf("  %.2fGB\n", used_memory);
        printf("  %.f GB |\n", total_memory); 

        for(int row = Height; row >= 0; row--){
            printf("        |"); 
            for(int col = 0; col < Width; col++){
                if(memory_data[col] == row){
                    printf("#");
                }else{
                    printf(" ");
                }
            }
            printf("\n");
        }
        printf("   0 GB ");
        for(int col = 0; col < Width + 1; col++){
            printf("-");
        }
        printf("\n\n");

        position = (position + 1) % Width;

        usleep(tdelay);
    }
}

void CPU_graph(int samples, int tdelay){
    int Width = samples; 
    int Height = 10;
    long preInitial = 0;
    long preTotal = 0;
    int position = 0; //Initialize it as to start at the first column for cpu
    int cpu_data[samples];

    //initialize the arrays
    for(int i = 0; i < samples; i++){
        cpu_data[i] = -1;
    }

    for(int i = 0; i < samples; i++){
        printf("\033[H\033[J");

        double CPU_usage = cpuUtilization(&preInitial, &preTotal);
        int cpu_y_position = (int)((CPU_usage / 100.0) * Height);
        cpu_data[position] = cpu_y_position;

        printf("Nbr of samples: %d -- every %d microSecs (%0.2f secs)\n\n", samples, tdelay, (tdelay / 1000000.0));
        printf("V CPU ");
        printf("  %.2f %%\n", CPU_usage);
        printf("  100%%  |\n");

        for(int row = 8; row >= 0; row--){
            printf("        |"); 
            for(int col = 0; col < Width; col++){
                if(cpu_data[col] == row){
                    printf(":");
                }else{
                    printf(" ");
                }
            }
            printf("\n");
        }

        printf("    0%%  ");
        for(int col = 0; col < Width + 1; col++){
            printf("-");
        }
        printf("\n\n");

        position = (position + 1) % Width;

        usleep(tdelay);

    }   
}

void MemoryandCpu(int samples, int tdelay){
    struct sysinfo info;
    int Width = samples;
    int memoryHeight = 10; 
    int cpuHeight = 10;
    long preInitial = 0;
    long preTotal = 0; 

    int memoryPosition = 0; //memory start at the first column
    int cpuPosition = 0; //cpu start at the first column

    int memory_data[samples];
    int cpu_data[samples];

    for(int i = 0; i < samples; i++){
        //since there is no previous data so is -1
        memory_data[i] = -1;
        cpu_data[i] = -1;
    }

    for(int i = 0; i < samples; i++){
        printf("\033[H\033[J");

        sysinfo(&info); //trying to retrieve the memory usage
        double total_memory = info.totalram / (1024.0 * 1024.0 * 1024.0);
        double free_memory = info.freeram / (1024.0 * 1024.0 * 1024.0);
        double used_memory = total_memory - free_memory; 

        double CPU_usage = cpuUtilization(&preInitial, &preTotal); //getting the cpu usage from the cpuUtilization function

        //calculate the dynamic position for memory and cpu 
        int memory_y_Position = (int)((used_memory / total_memory) * memoryHeight);
        int cpu_y_Position = (int)((CPU_usage / 100.0) * cpuHeight);

        memory_data[memoryPosition] = memory_y_Position;
        cpu_data[cpuPosition] = cpu_y_Position;

        printf("Nbr of samples: %d -- every %d microSecs (%0.2f secs)\n\n", samples, tdelay, (tdelay / 1000000.0));
        printf("V Memory");
        printf("  %.2fGB\n", used_memory);
        printf("  %.f GB |\n", total_memory); 

        for(int row = memoryHeight; row >= 0; row--){
            printf("        |"); 
            for(int col = 0; col < Width; col++){
                if(memory_data[col] == row){
                    printf("#");
                }else{
                    printf(" ");
                }
            }
            printf("\n");
        }
        printf("   0 GB ");
        for(int col = 0; col < Width + 1; col++){
            printf("-");
        }
        printf("\n\n");

        printf("V CPU ");
        printf("  %.2f %%\n", CPU_usage);
        printf("  100%%  |\n");

        for(int row = 8; row >= 0; row--){
            printf("        |"); 
            for(int col = 0; col < Width; col++){
                if(cpu_data[col] == row){
                    printf(":");
                }else{
                    printf(" ");
                }
            }
            printf("\n");
        }

        printf("    0%%  ");
        for(int col = 0; col < Width + 1; col++){
            printf("-");
        }
        printf("\n");

        memoryPosition = (memoryPosition + 1) % Width;
        cpuPosition = (cpuPosition + 1) % Width;

        usleep(tdelay);

    }
    printf("\n");
}

void cores_CountAndFreq(){
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if(fp == NULL){
        perror("Cannot open /proc/cpuinfo");
        return;
    }

    int count = 0;
    char buffer[256];

    while(fgets(buffer, sizeof(buffer), fp)){
        if(strncmp(buffer, "processor", 9) == 0){
            count++;
        }
    }
    fclose(fp);

    long maxFreq = 0;
    fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", "r");
    if(fp != NULL){
        if(fscanf(fp, "%ld", &maxFreq) != 1){
            perror("Cannot read maximum frequency");
        }
        fclose(fp);
    }else{
        perror("Cannot open cpuinfo_max_freq");
    }
    
    printf("V Number of Cores: %d @ %.2f GHz\n", count, maxFreq * 0.000001);

    int cols = 4;
    int rows = (count +  1) / 4;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(i * cols + j < count){
                printf(" +---+ ");
            }
        }
        printf("\n");
        for(int j = 0; j < cols; j++){
            if(i * cols + j < count){
                printf(" |   | ");
            }
        }
        printf("\n");
        for(int j = 0; j < cols; j++){
            if(i * cols + j < count){
                printf(" +---+ ");
            }
        }
        printf("\n");
    }
    printf("\n\n");
}

int main(int argc, char **argv){
    int memory, cpu, cores = 0;
    int samples = 20; //default value 
    int tdelay = 500000; //default value

    int samples_input = 0;
    int tdelay_input = 0;

    if(argc == 1){
        MemoryandCpu(samples, tdelay);
        cores_CountAndFreq();
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "--memory") == 0){
            memory = 1;
        }else if(strcmp(argv[i], "--cpu") == 0){
            cpu = 1;
        }else if(strcmp(argv[i], "--cores") == 0){
            cores = 1;
        }else if (strncmp(argv[i], "--samples=", 10) == 0) {
            samples = atoi(argv[i] + 10);
            samples_input = 1;
        } else if (strncmp(argv[i], "--tdelay=", 9) == 0) {
            tdelay = atoi(argv[i] + 9);
            tdelay_input = 1;
        } else if (i == 1) { // Positional argument for samples
            samples = atoi(argv[i]);
            samples_input = 1;
        } else if (i == 2) { // Positional argument for tdelay
            tdelay = atoi(argv[i]);
            tdelay_input = 1;
        }
    }

    if (samples <= 0) {
        fprintf(stderr, "Error for the number of samples. Must be > 0.\n");
        return 1;
    }
    if (tdelay <= 0) {
        fprintf(stderr, "Error for the tdelay. Must be > 0.\n");
        return 1;
    }
    
    if(!memory && !cpu && !cores){
        MemoryandCpu(samples, tdelay);
        cores_CountAndFreq();
        return 0;
    }
    
    if(memory && cpu){
        //if there is not input for samples and tdelay, then will show the default
        //if there is input for samples and tdelay, then will show the input for the specify numbers
        MemoryandCpu((samples_input ? samples : 20), (tdelay_input ? tdelay : 500000)); 
        return 0;
    }
    
    if(memory){
        Memory_graph(samples, tdelay);
    }
    if(cpu){
        CPU_graph(samples, tdelay);
    }
    if(cores){
        cores_CountAndFreq();
    }

    return 0;
}