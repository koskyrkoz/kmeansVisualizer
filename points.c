#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float float_rand( float min, float max )
{
    float scale = rand() / (float) RAND_MAX;
    return (float)min + scale * ( max - min );
}

void generate_teams(FILE *fp,int P,float xmin,float xmax,float ymin,float ymax){
    int i;
    float x,y;

    for (i=0; i<P; i++){
        x=float_rand(xmin,xmax);
        y=float_rand(ymin,ymax);
        fprintf(fp,"%f %f\n",x,y);
    }
}

void generate_points(){
    FILE *fp;
    fp = fopen("points.txt","w");
    srand(time(NULL));

    if(fp == NULL){
        printf("generate points: Error opening output file!\n");
        exit(1);
    }

    generate_teams(fp,150,0.75,1.25,0.75,1.25);
    generate_teams(fp,150,0.0,0.5,0.0,0.5);
    generate_teams(fp,150,0.0,0.5,1.5,2.0);
    generate_teams(fp,150,1.5,2.0,0.0,0.5);
    generate_teams(fp,150,1.5,2.0,1.5,2.0);
    generate_teams(fp,75,0.6,0.8,0.0,0.4);
    generate_teams(fp,75,0.6,0.8,1.6,2.0);
    generate_teams(fp,75,1.2,1.4,0.0,0.4);
    generate_teams(fp,75,1.2,1.4,1.6,2.0);
    generate_teams(fp,150,0.0,2.0,0.0,2.0);

    printf("generate points: OK!\n");
    fclose(fp);
}

void plot_points(){
    FILE *gnupipe=NULL;
    int i;
    char *gnucommands [] = {"set title \"Points\"",("plot 'points.txt'")};
    gnupipe = _popen("gnuplot -persistent","w");
    for(i=0;i<2;i++){
        fprintf(gnupipe,"%s\n",gnucommands[i]);
    }
    fclose(gnupipe);
    return;
}

int main(){
    generate_points();
    plot_points();
    return 0;
}
