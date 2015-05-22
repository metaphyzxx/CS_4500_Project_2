#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAX 1024
#define NR_THREADS 4


int n1,n2;
char *s1,*s2;
FILE *fp;


typedef struct str_thdata{
    int thread_no;
    int partial_count;
} thdata;



int readf(FILE *fp)
{
	if((fp=fopen("strings.txt", "r"))==NULL){
		printf("ERROR: can't open string.txt!\n");
		return 0;
	}
	s1=(char *)malloc(sizeof(char)*MAX);
	if(s1==NULL){
		printf("ERROR: Out of memory!\n");
		return -1;
	}
	s2=(char *)malloc(sizeof(char)*MAX);
	if(s1==NULL){
		printf("ERROR: Out of memory\n");
		return -1;
	}
	/*read s1 s2 from the file*/
	s1=fgets(s1, MAX, fp);
	s2=fgets(s2, MAX, fp);
	n1=strlen(s1);  /*length of s1*/
	n2=strlen(s2)-1; /*length of s2*/
	if(s1==NULL || s2==NULL || n1<n2)  /*when error exit*/
		return -1;
}

int num_substring(void){
	int i,j,k;
	int count;
    int total = 0;

	for (i = 0; i <= (n1-n2); i++){   
		count=0;
		for(j = i,k = 0; k < n2; j++,k++){  /*search for the next string of size of n2*/  
			if (*(s1+j)!=*(s2+k)){
				break;
			}
			else
				count++;
			if(count==n2)    
				total++;		/*find a substring in this step*/                          
		}
	}
	return total;
}

void num_substring_par( void *ptr ){
	int i,j,k;
	int count;
    int total = 0;

	for (i =((thdata*) ptr)->thread_no; i <= (n1-n2); i = i + NR_THREADS){   
		count=0;
		for(j = i,k = 0; k < n2; j++,k++){  /*search for the next string of size of n2*/  
			if (*(s1+j)!=*(s2+k)){
				break;
			}
			else
				count++;
			if(count==n2)    
				total++;		/*find a substring in this step*/                          
		}
	}
	((thdata*)ptr)->partial_count = total;
}

int main(int argc, char *argv[])
{
	int count;
    pthread_t thread0, thread1, thread2, thread3;  
    thdata data0,data1, data2,data3;  
    int partial_sum;
    
    data0.thread_no = 0;
    data1.thread_no = 1;
    data2.thread_no = 2;
    data3.thread_no = 3;

    
    /* create threads 1 and 2 */    
    pthread_create (&thread0, NULL, (void *) &num_substring_par, (void *) &data0);
    pthread_create (&thread1, NULL, (void *) &num_substring_par, (void *) &data1);
    pthread_create (&thread2, NULL, (void *) &num_substring_par, (void *) &data2);
    pthread_create (&thread3, NULL, (void *) &num_substring_par, (void *) &data3);
 
	readf(fp);
	count = num_substring();
    
    partial_sum = 0;
    pthread_join(thread0, NULL);
    partial_sum += data0.partial_count;
    pthread_join(thread1, NULL);
    partial_sum += data1.partial_count;
    pthread_join(thread2, NULL);
    partial_sum += data2.partial_count;
    pthread_join(thread3, NULL);
    partial_sum += data3.partial_count;
    
 	printf("The number of substrings is: %d\n", count);
    printf("the number of substrings counted in parallel is: %d\n",partial_sum);
	return 1;
}
