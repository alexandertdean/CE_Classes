// Part A //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Checks what arguments are passed to the program
void check_args(int argc, char *argv[], int *h,int *p,int *m, int *f, int *out_file);
// Displays Help message for the user
void help(int argc, int h);         
// Checks cpu info file in proc directory and outputs what user wants 
void check_file(FILE *fp,FILE *out, int p,int m, int f);

int main(int argc, char * argv[])
{
    /* Initialize variables used */
	char file_name[]="/proc/cpuinfo";
	FILE *fp,*out;
	int i,h,p,m,f,out_file;
    /* Check users arguments and check if help is needed */
    check_args(argc,argv,&h,&p,&m,&f,&out_file);
	help(argc,h);
    /* Open file to read from and an output file to write to if it is needed */
	fp=fopen(file_name, "r");
    if(fp == NULL){
        printf("%s not opened properly.\n", file_name);
    }
    else
        printf("%s was opened.\n", file_name);
	out=fopen(argv[out_file],"w");
    if(out == NULL){
        printf("Output file was not opened properly.\n");
    }
    else
        printf("%s was opened.\n", argv[out_file]);
    /* Check cpu info file and print what the user wants */
    check_file(fp,out,p,m,f);
    /* close the files that were opened */
	if(fp!=NULL) {
		fclose(fp);
        fp = NULL;
    }
	if(out!=NULL){
		fclose(out);
        fp =NULL;
    }
	return 0;  // terminate program
}
// Checks what arguments are passed to the program
void check_args(int argc, char *argv[], int *h,int *p,int *m, int *f, int *out_file)
{
    /* Initialize pointers to zero */
    int i =0;
    *h =0;
    *p =0;
    *m = 0;
    *f =0;
    *out_file=0;
    /* Loop through arguments and check what user entered and set variable */
    for(i=0;i<argc;i++){
        if(strcmp(argv[i],"-h")==0)
            *h = 1;
        else if(strcmp(argv[i],"-p")==0)
            *p = 1;
        else if(strcmp(argv[i],"-m")==0)
            *m = 1;
        else if(strcmp(argv[i],"-f")==0){
            *f = 1;
            *out_file = i+1;
        }
    }
}
// Checks cpu info file in proc directory and outputs what user wants
void check_file(FILE *fp,FILE *out,int p, int m, int f)
{
    /* Initialize buffer to hold value of each line */
    char line[1024];
    /* Read cpu infor file line by line checking for what user wants */
    while(fp!=NULL && fgets(line,sizeof(line),fp)!=NULL){
        if(f==1){     // if user wants an output file
            if(p==1){ // if user want to see processor number find line with processor
                if(strstr(line, "processor"))
                    fprintf(out,"%s",line);
            }   
            if(m==1){ // if user want to see model name find line with model name
                if(strstr(line, "model name"))
                    fprintf(out,"%s",line);
            }
        }
        else{         // if user doesnt want output file
            if(p==1){ // if user want to see processor number find line with processor
                if(strstr(line, "processor"))
                    printf("%s",line);
            }
            if(m==1){ // if user want to see model name find line with model name
                if(strstr(line, "model name"))
                    printf("%s",line);
            }
        }
    }
}
// Displays Help message for the user
void help(int argc, int h) 
{
    if(argc == 1 || h == 1) // Displays message if no argument was used or if -h was used
    {
        printf("Help Message:\n");
        printf("-h: help options\n");
        printf("-p: number of proccessors\n");
        printf("-m: model name of processor\n");
        printf("-f: followed by file name to write output to\n\n");
    }
}
