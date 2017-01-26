// Part A //
// timer.c //
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/io.h>

// Sets up permissions on the ports
void permissions(int pti, int channel, int mode);
// Get time values preform calculations and output results 
void get_set_times(int pti, int channel, FILE *fp);
// Find time between when timer is enabled and timer is disabled
double calc_time(int channel, int pti,int i);

int main(int argc, char *argv[])
{ 
    // Initialize Port Variables
    int mode = 0x43;     // Mode/Command register (write only, a read is ignored)
    int channel = 0x42;  // Channel 2 data port (read/write)
    int pti = 0x61;      // Progammable Interval Timer channel 2 controlled by 0x61
    // Set up permissions
    permissions(pti,channel,mode);
    // 0111 0100 read (0 = binary mode, 1-3 =Mode 2 rate generator, 4-5= access lobyte/hibyte, 6-7= channel 2)
    outb(0x74, mode);
    // Opens csv excel file to write times too and error check
    FILE *fp = fopen("time_output.csv", "w"); 
    if (fp == NULL) {     // error check if file opened
      printf("Failed to open output CSV file.");
      return -1;
    }
    else 
      printf("CSV file opened properly.\n");
    // Get times and output them for the user
    get_set_times(pti,channel,fp);
    // Close csv file if it was opened
    if(fp!=NULL) {
        fclose(fp);
        fp = NULL;
    }
    return 0;
}
// Sets up permissions on the ports
void permissions(int pti, int channel, int mode)
{
    // Sets the id of user ID of the calling process (from ioshell.c)
    setuid(getuid());   
    // Allows permission to access the ports initialized 
    ioperm(pti,1,1);      
    ioperm(channel,1,1);
    ioperm(mode,1,1);
}
// Get time values preform calculations and output results 
void get_set_times(int pti, int channel, FILE *fp)
{
    int i,j;             // counters
    int data_point = 0;  // counts data points
    double time = 0;     // sets time to zero
    // Calls usleep 100 times of integers between 1 - 200
    for (i=1;i<201;i++) {   // loops through 1 to 200 us
      for(j=1;j<101;j++){   // calls of usleep 100 times
        data_point++;       // increments to next data point
        // Call calculate functon to find time between timer enabled and return of usleep
        time = calc_time(channel,pti,i);
        // Output results to standard out and a csv file 
        printf("Data point %d: Time of %d is %f microseconds\n",data_point,i,time); 
        fprintf(fp, "%d, %f,\n",i,time);    
      }
    }
}
// Find time between when timer is enabled and timer is disabled
double calc_time(int channel, int pti, int i)
{
    // Initialize variables used within function
    int data, upper_bytes, lower_bytes, init_count, end_count, count;

    // Finds the initial count based of initial lower and upper byte count
    lower_bytes = 0xFF;  
    upper_bytes = 0xFF;   
    init_count = (upper_bytes << 8) + lower_bytes;
    // Enable Counter (init_count represents begin time)
    data = inb(pti);    // Pulls bits from timer at port 0x61
    data = data | 0x01; // Enables counter bit 
    outb(data, pti);    // Initializes counter bit
    // Lower and Upper Byte set to FF
    outb(0xFF, channel);// lower byte set to FF
    outb(0xFF, channel);// upper byte set to FF
    usleep(i);          // sleeps based on i (1-200 us)
    // Disable counter (end_count represents end time)
    data = data & 0xFE; // disable counter bit
    outb(data, pti);    // Un-initialize counter bit
    // Finds the end count based of end of lower and upper byte count
    lower_bytes = inb(channel); 
    upper_bytes = inb(channel);  
    end_count = (upper_bytes << 8) + lower_bytes;
    // Calculates difference between when timer was enabled and then disabled
    count = init_count - end_count;
    // Converts counts to time and returns to get_set_time function  
    return count * (1/1.1925);    
}