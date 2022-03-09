#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <string.h>

typedef struct{     // здесь хранятся названия файла и время задержки 
	char name[100];
	int time;
} file;

file razdel(char *s,int size){  //сначала циклом доходит до пробела и засовывает  в f.sec  время задержки,затем идет дальше и оставшуюся строчку сует в name 
	char str[100];
	int j;
	for(int i=0;s[i]!=' '&& i<size;i++){
		str[i]=s[i];
		j=i;		
	}
	str[size-1]='\0';
	file f;
	f.time=atoi(str);
	for(int i=j+1;s[i]!='\0';i++){
		f.name[i-j-1]=s[i];
	}
	return f;
}

int comprasion(const void *a,const void *b){  //функция чтобы сравнивать время задержки у программ в файле 
        return (((file *)a)->time)-(((file*)b)->time);
}


int main(int argc, char **argv){
	char *myfile = argv[1];
	FILE * FiLe;
	int count = 0;//КОЛИЧЕСТВО ЗАПУСКОВ 
	FiLe=fopen(myfile,"r"); 
	file * a =(file *)malloc(100*sizeof(file));   
	//разделяет время и сами программы и засосвывет их в массив а
	for(int i=0;fscanf(FiLe,"%s",myfile)!=EOF;i++){
		a[i].time = atof(myfile);
		fscanf(FiLe,"%s",myfile);
		strcpy(a[i].name,myfile);
		count+=1;
	}
	qsort(a,count,sizeof(a[0]),comprasion);  
	int stop=0;// хранит в себе время которое мы прождали 
	for(int i=0;i<count;i++){
		sleep(a[i].time-stop); //задержка времени
		stop=a[i].time;
		int pip=fork(); // процесс чтобы запустить программу из файла
		if(pip==0){                            
			char **arg=(char **) malloc(sizeof(char*));
        		arg[0]=(char *) malloc(100*sizeof(char));  
        		arg[1]=NULL; 
			strcpy(arg[0],a[i].name);
			pip=execvp(arg[0],arg);//выполнение программы из файла

		}
	}
	free(a);
	fclose(FiLe);
}




