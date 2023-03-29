//  Konstantinos Kyriakou 3015
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 1200
#define M 9

float X[N][2];
float C[M][2];
int T[N];
int flag;
int iterations=0;

void find_distance();
void update_centers();
void read_file();
void plot_files(char *filename);
void log_centers();
void init_centers();
void cleanup_files();

int main(){

    int i;
    float deviation=0.0;
    char *lastcenters[16];

    read_file();
    init_centers();
    flag=1;
    while(flag==1){
        flag=0;
        log_centers();
        find_distance();
        update_centers();
        iterations++;
    }
    iterations--;
    //printf("> kmeans (main): OK!\n");

    for(i=0;i<N;i++){
        deviation+=sqrt(pow((C[T[i]][0]-X[i][0]),2)+pow((C[T[i]][1]-X[i][1]),2));
    }
    deviation/=N;

    printf("> for M=%d centers\n",M);
    printf("> total iterations: %d\n",iterations);
    printf("> deviation: %f \n",deviation);

    sprintf(lastcenters,"center%d.txt",iterations);
    plot_files(lastcenters);
    for(i=0;i<M;i++){
        printf("    Center #%d: %f , %f\n",(i+1),C[i][0],C[i][1]);
    }

    cleanup_files();
    return 0;
}

void find_distance(){
    int i,j;
    float distance,mindistance,mincluster;
    for(i=0;i<N;i++){
        mindistance=INFINITY;
        for(j=0;j<M;j++){
            distance=sqrt(pow(C[j][0]-X[i][0],2)+pow(C[j][1]-X[i][1],2));
            if(distance<mindistance){
                mindistance=distance;
                mincluster=j;
            }
        }
        if(T[i]!=mincluster){
            flag=1;
        }
        T[i]=mincluster;
    }
    return;
}

void update_centers(){
    int i;
    float sum[M][2]={0};
    int count[M]={0};

    for(i=0;i<N;i++){
        count[T[i]]++;
        sum[T[i]][0]=sum[T[i]][0]+X[i][0];
        sum[T[i]][1]=sum[T[i]][1]+X[i][1];
    }
    for(i=0;i<M;i++){
        C[i][0]=sum[i][0]/count[i];
        C[i][1]=sum[i][1]/count[i];
    }
    return;
}


void read_file(){
	FILE *fp;
	int i;

	fp=fopen("points.txt","r");
	if(fp == NULL){
        printf("read files: Error opening input file!\n");
        exit(1);
    }
	for(i=0;i<N;i++){
        fscanf(fp,"%f %f",&X[i][0],&X[i][1]);
	}
    //printf("read files: OK!\n");
	fclose(fp);
	return;
}

void plot_files(char *filename){
    FILE *gnupipe=NULL;
    char *gnutitle[16];
    sprintf(gnutitle,"set key outside ", "set title \"Iteration %d\"",iterations);
    int i;

    char *gnucommands[16];
    sprintf(gnucommands,"plot 'points.txt' with points ,'%s' with circles",filename);

    gnupipe = _popen("gnuplot -persistent","w");
    fprintf(gnupipe,"%s\n",gnutitle);
    fprintf(gnupipe,"%s\n",gnucommands);
    fclose(gnupipe);
    return;
}
void log_centers(){
    FILE *fp;
    char *filename[16];
    int i;
    sprintf(filename,"center%d.txt",iterations);
    fp=fopen(filename,"w");
    for(i=0;i<M;i++){
        fprintf(fp,"%f %f\n",C[i][0],C[i][1]);
    }
    fclose(fp);
    return;
}

void init_centers(){
    int i,random;
    srand(time(NULL));

    for(i=0;i<M;i++){
        random=rand()%N;
        C[i][0]=X[random][0];
        C[i][1]=X[random][1];
    }
    //printf("initialize centers: OK!\n");
    return;
}

void cleanup_files(){
    int i;
    char *filename[16];
    for(i=0;i<(iterations);i++){
        sprintf(filename,"center%d.txt",i);
        remove(filename);
    }
    //printf("> cleanup files: OK!\n");
    return;
}
