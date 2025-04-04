# System Monitoring Tool 

This program will present the information in real-time in graphical form by displaying the memory, cpu, and cores. This program is intend to run un a Linux OS (IA lab machines).

## Implementation  

<code><sys/sysinfo.h></code>
 * Gets memory usage using sysinfo()

<code>/proc/stat</code>
 * Gets cpu usage and calculates CPU utilization through the differences in the total time and the initial time between samples

<code>/proc/cpuinfo</code>
 * Gets the number of cores in the system

<code>usleep()</code>
 * In the <unistd.h> library and takes the argument in microseconds
 * Allow precise control over the timing of each sample collection 
 * The usleep is used in displaying the memory graph and cpu graph which is showing the delay between samples
 * When compiling the program, it may encounter the warning since it has been marked as deprecated in POSX.1-2008. However, it will not affect the functionality as it provide precision sampling in system matrics. 

### Functions Documentation

<code>double cpuUtilization(long *preInitial, long *preTotal)</code>
 * Calculate the CPU utilization according to <code>/proc/stat</code> data
 * Store the previous initial time and previous total time as parameters to compute the usage over time
 * It will return a CPU usage percentage as a double when calling the function

<code>void Memory_graph(int samples, int tdelay)</code>
 * Show a memory graph with a given samples and delay

<code>void CPU_graph(int samples, int tdelay)</code>
 * Show CPU usage graph for a given number of samples and delay

<code>void MemoryandCpu(int samples, int tdelay)</code>
 * Show both the memory graph and CPU graph simultaneously

<code>void cores_CountAndFreq()</code>
 * Display the number of cores and the maximum frequency
 * The data is read from <code>/proc/cpuinfo</code> for counting the number of cores
 * The maximum frequency is read from <code>/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq</code>

<code>int main(int argc, char **argv)</code>
 * The main function handles the command line arguments
 * Call specific functions based on user input
 * Checks for users input must be larger than 0

## How to run the program

1. Compile 

        gcc -std=c99 -Wall -o myMonitoringTool myMonitoringTool.c


2. Run
   
        ./myMonitoringTool or
        ./myMonitoringTool [inputs / command line arguments]

<br> 

## CLAs

* <code>--memory</code>
  * Only generate the memory usage graph
* <code>--cpu</code>
  * Only generate the cpu usage graph
* <code>--cores</code>
  * Only generate the cores information and the maximum frequency 
* <code>--samples=N</code>
  * N samples will be collected and based on the N numbers of repetition 
  * Also taken as positional argument
  * The default value is 20
* <code>--tdelay=T</code>
  * Delay in samples between microseconds
  * Also taken as positional argument
  * The default value is 500000 microsec

<br> 

## Examples

<code>./myMonitoringTool</code>

![Image](https://drive.google.com/uc?export=view&id=1zOqrn9akIi9AZV4cdpQmmhzqj8QQoHmc)





<code>./myMonitoringTool 50 100000</code>
        
![Image](https://drive.google.com/uc?export=view&id=1plgghWWBYmBUtvOREbxdGH5YI3_zZI3l)




<code>./myMonitoringTool --samples=40 --tdelay=100000 --memory --cpu</code>

![Image](https://drive.google.com/uc?export=view&id=1Zcx-aO4EuLhRVvZPG6CVJ7WJ8Z2sdvHU)







<code>./myMonitoringTool --memory</code>

![Image](https://drive.google.com/uc?export=view&id=1k-pegCXj0CMcNUGWvujLDAErWpIVUwQC)





<code>./myMonitoringTool --cpu</code>

![Image](https://drive.google.com/uc?export=view&id=1fyCJWvJzle5Pjt1B1657Lcl4eWowinGR)
