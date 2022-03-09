#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>

struct point_t {
       int x;
       int y;
       int **new;
       int **a;
};
void* thread_f1(void*arg){
		
	struct point_t *data = arg;
	data->new[data->x][data->y] = 0;	
	for(int k = 0; k < 3; k++){	
		data->new[data->x][data->y] += data->a[data->x][k] * data->a[k][data->y];
		}

	printf(" %d ",data->new[data->x][data->y]);

}

int main(){
	struct point_t A;
	int n;
	FILE *myfile;
	myfile = fopen("matrix1.txt", "r");
	fscanf(myfile, "%d", &n);

	
	A.a = (int**) malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++) {
   		A.a[i] = (int*) malloc(n * sizeof(int));
	}


	
	A.new = (int**) malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++) {
   		A.new[i] = (int*) malloc(n * sizeof(int));
	}



	FILE *file;
	file = fopen("matrix.txt", "r");
	pthread_t **thr = NULL;
 	thr = (pthread_t**) malloc(n * sizeof(pthread_t*));
	for (int i = 0; i < n; i++) {
   		thr[i] = (pthread_t*) malloc(n * sizeof(pthread_t));
	}
	for (int i = 0; i < 3  ; ++i){
               for (int j = 0;j < 3 ; j++){
			fscanf(file,"%d", &(A.a[i][j]));
		}
	       
	}
	
	for (int i = 0; i < 3; ++i){
		for (int j = 0;j < 3; j++){
			A.x = i;
			A.y = j;
			if(0 != pthread_create(&thr[i][j], NULL, thread_f1,&A)){
				perror("Can't cread thred");
				exit(-1);
			}	
			if(0 != pthread_join(thr[i][j],NULL)){
                        	fprintf(stderr,"Can't cread thred");
                        	exit(-1);
  			}
		}
		printf("\n");
	}
  
	

	fclose(file);
	fclose(myfile);
	return 0;

}
