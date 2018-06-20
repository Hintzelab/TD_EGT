//
//  main.cpp
//  IsingModel
//
//  Created by Arend Hintze on 5/16/17.
//  Copyright © 2017 Arend Hintze. All rights reserved.
//


#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

using namespace std;
int xm[9]={0,-1,+1,0,0,+1,+1,-1,-1};
int ym[9]={0,0,0,-1,+1,-1,+1,+1,-1};
#define xDim 1024
#define yDim 1
#define NBHood 3
#define mutationRate 0.02

int agents[xDim][yDim];
int NA[xDim][yDim];
double score[xDim][yDim];
double pEffect=0.8;
double pCost=0.2;
double preComputedPayoff[4][4][4];

void updateScoreForGroup(int x,int y,double R);
void doPrecomputePayoffs(double R,double effect,double cost);
double getScore(int action,int N[4],double rMultiplyer, double effect, double cost);

int main (int argc, const char * argv[])
{
    int popSize=xDim*yDim;
    float R=1.0;
    int replacement=8;
    float T=1.0;
    int i,j,z,x,y;
    float F[4];
    int C[4];
    int w,testG;
    float Cs,Ds,Ms,Is;
    int update=0;
    //double LTH=1.0;
    FILE *f=fopen(argv[1],"w+t");
    R=atof(argv[2]);
    T=atof(argv[3]);
    pEffect=atof(argv[4]);
    int nPlayer=atoi(argv[5]);
    srand(getpid());
    for(i=0;i<xDim;i++)
        for(j=0;j<yDim;j++)
            agents[i][j]=rand()&(nPlayer-1);
    for(update=0;update<250000;update++){
        doPrecomputePayoffs(R, pEffect, pCost);
        int dir[2]={-1,1};
       
        for(z=0;z<replacement;z++){
            x=(int)rand()%xDim;
            y=(int)rand()%yDim;
            double Sme,Sother;
            int who=agents[x][y];
            int me=who;
            int other;
            do{
                other=rand()&(nPlayer-1);
            }while(other==me);
            updateScoreForGroup(x, y, R);
            Sme=score[x][y];
            agents[x][y]=other;
            updateScoreForGroup(x,y, R);
            Sother=score[x][y];
            //updateScoreForGroup(x, y, R);
            //updateScoreForGroup(x+d, y, R);
            agents[x][y]=me;
            if(((float)rand()/(float)RAND_MAX)<(1.0/(1.0+exp(T*(Sme-Sother)))))
                agents[x][y]=other;

//            printf("%f %f\n",score[x][y],score[x+d][y]);
//            if(((float)rand()/(float)RAND_MAX)<(1.0/(1.0+exp(T*(score[x][y]-score[x+d][y])))))
//                agents[x][y]=agents[x+d][y];
             //*/
            /*
            updateScoreForGroup(x, y, R);
            maxFit=-10.0;
            for(i=1;i<NBHood;i++)
                if(score[(x+xm[i])&(xDim-1)][(y+ym[i])&(yDim-1)]>maxFit)
                    maxFit=score[(x+xm[i])&(xDim-1)][(y+ym[i])&(yDim-1)];
            if(maxFit>0.0){
                do{
                    w=rand()%NBHood;
                }while((w==0)||(((double)rand()/(double)RAND_MAX)>(score[(x+xm[w])&(xDim-1)][(y+ym[w])&(yDim-1)]/maxFit)));
                agents[x][y]=agents[(x+xm[w])&(xDim-1)][(y+ym[w])&(yDim-1)];
            } else {
                do{w=rand()%NBHood;}while(w==0);
                agents[x][y]=agents[(x+xm[w])&(xDim-1)][(y+ym[w])&(yDim-1)];
            }
            if(((double)rand()/(double)RAND_MAX)<mutationRate)
                agents[x][y]=rand()&3;
            */
        }
        if((update&255)==0){
//            if((update&1023)==0)
//                cout<<update<<" "<<Cs<<" "<<Ds<<" "<<Ms<<" "<<Is<<" "<<(((Cs+Ms)-(Ds+Is))/(Cs+Ds+Ms+Is))<<endl;
        }
    }
    Cs=0.0;Ds=0.0;Ms=0.0;Is=0.0;
    C[0]=0; C[1]=0; C[2]=0; C[3]=0;
    for(i=0;i<xDim;i++)
        for(j=0;j<yDim;j++)
            C[(int)agents[i][j]]++;
    Cs+=(float)C[0]/(float)popSize;
    Ds+=(float)C[1]/(float)popSize;
    Ms+=(float)C[2]/(float)popSize;
    Is+=(float)C[3]/(float)popSize;
    cout<<update<<" "<<Cs<<" "<<Ds<<" "<<Ms<<" "<<Is<<" "<<(((Cs+Ms)-(Ds+Is))/(Cs+Ds+Ms+Is))<<endl;
    fprintf(f,"%f,%f,%f,%f,%f,%f\n",pEffect,Cs,Ds,Ms,Is,(((Cs+Ms)-(Ds+Is))/(Cs+Ds+Ms+Is)));
    fclose(f);
    return 0;
}
//regular selection
/*
 updateScoreForGroup(x, y, NBHood, R);
 //*/
//*
//regular ising
//for(x=0;x<xDim;x++)
//    for(y=0;y<yDim;y++){


void updateScoreForGroup(int x,int y,double R){
    int n;
    for(n=0;n<3;n++)
        score[(x+xm[n])&(xDim-1)][(y+ym[n])&(yDim-1)]=0.0;
    for(n=0;n<3;n++){
        score[(x+xm[n])&(xDim-1)][(y+ym[n])&(yDim-1)]+=
        preComputedPayoff[agents[(x+xm[n]-1)&(xDim-1)][y]][agents[(x+xm[n])&(xDim-1)][y]][agents[(x+xm[n]+1)&(xDim-1)][y]];
    }
}


void doPrecomputePayoffs(double R,double effect,double cost){
    int N[4],A[3];
    int i,j,k,l;
    for(l=0;l<64;l++){
        for(k=0;k<4;k++)
            N[k]=0;
        for(k=0;k<3;k++){
            A[k]=(l>>(2*k))&3;
            N[A[k]]++;
        }
        double score=getScore(A[1], N, R, effect, cost);
        preComputedPayoff[A[0]][A[1]][A[2]]=score;
        //printf("%i %i %i %i %i %f\n",A[1],N[0],N[1],N[2],N[3],score);
        
    }
}

double getScore(int action,int N[4],double rMultiplyer, double effect, double cost){
    double score=0.0;
    double neighbors=2.0;
    switch(action){
        case 0: //C ooperator
            score+=(rMultiplyer*((double)N[0]+(double)N[2])/((double)neighbors+1.0))-1.0;
            break;
        case 1: //D efector
            score+=(rMultiplyer*((double)N[0]+(double)N[2])/((double)neighbors+1.0))-(effect*((double)N[2]+(double)N[3])/((double)neighbors));
            break;
        case 2://M oralist
            score+=(rMultiplyer*((double)N[0]+(double)N[2])/((double)neighbors+1.0))-1.0-(cost*((double)N[1]+(double)N[3])/((double)neighbors));
            break;
        case 3://I moralist
            score+=(rMultiplyer*((double)N[0]+(double)N[2])/((double)neighbors+1.0))-(effect*((double)N[2]+(double)N[3])/((double)neighbors))-(cost*((double)N[1]+(double)N[3])/((double)neighbors));
            break;
    }
    return score;
}
