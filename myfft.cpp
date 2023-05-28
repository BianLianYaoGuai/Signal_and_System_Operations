#include <iostream>
#include <cmath>
#include <complex>
#include <time.h>
#include <pthread.h>
using namespace std;

int m;
int N;
double* x;
complex<double>* x_1;
complex<double>* e;

void read_array(const char* filename);
void write_array(const char* filename);

int bitreverse(int x, int bits){
    int y = 0;
    for(int i=0; i<bits; i++){
        y = (y << 1) | (x & 1);
        x >>= 1;
    }
    return y;
}

const int THREAD_NUM = 2;// 双线程
typedef struct{
    int r;
}threadParm_t;
pthread_barrier_t barrier;


// 线程算法一
void* fun_thread_1(void* parm){
    threadParm_t* p=(threadParm_t *) parm;
    int r=p->r;
    
//     第一步：给x换序
    {
        int w = (N)/THREAD_NUM;
        int begin = r*w ;
        int last;
        if (r != THREAD_NUM-1)
            last = (r+1)*w ;
        else
            last = N;
        for(int i=begin; i<last; i++){
            int j = bitreverse(i,m);
            x_1[j] = x[i];
        }
    }
    
//     第二步：计算e的幂次
    {
        if (r == 0){
            int B=1;
            for (int i = 1; i <= m-1; i++) {
                for(int k= 0; k<B ; k++)
                    e[B+k-1] = complex<double>( cos(-(M_PI/B*k)),sin(-(M_PI/B*k)) )  ;
                B = B << 1;
            }
        }
        if (r == 1){
            int B = pow(2,m-1);
            for(int k= 0; k<B ; k++)
                e[B+k-1] = complex<double>( cos(-(M_PI/B*k)),sin(-(M_PI/B*k)) )  ;
        }
    }
    
    pthread_barrier_wait(&barrier);
    
//     第三步：蝴蝶算法
    {
        complex<double> Wk ;
        int B = 1;
        int tempINT = pow(2,m-1);
        for(int i=1; i<=m; i++){        
            int w = (tempINT)/THREAD_NUM;
            int begin = r * w ;
            int last;
            if (r != THREAD_NUM-1)
                last = begin + w ;
            else
                last = tempINT;
            for(int j=begin; j<last; j++){
                int tempINT2 = j * ( B<<1 );
                for(int k=0; k<B; k++){
                    Wk = e[B+k-1] * x_1[tempINT2+B];
                    x_1[tempINT2+B] = x_1[tempINT2] - Wk;
                    x_1[tempINT2] += Wk;
                    tempINT2++;
                 }                   
             }
            B = B << 1;
            tempINT = tempINT >> 1;
            pthread_barrier_wait(&barrier);
        }
    }
    pthread_exit(nullptr);
}


void base2_dit_fft(){
//     线程初始化
    pthread_t thread[THREAD_NUM];
    threadParm_t threadParm[THREAD_NUM];
    pthread_barrier_init(&barrier,NULL,THREAD_NUM);
    for(int j=0;j<THREAD_NUM;j++){
        threadParm[j].r=j;
    }  
//     启动线程
    for(int j=0;j<THREAD_NUM;j++){
        pthread_create(&thread[j],nullptr,fun_thread_1,(void*)&threadParm[j]);
    }
//     等待线程
    for(int j=0;j<THREAD_NUM;j++){
        pthread_join(thread[j],nullptr);
    }
    return;
}



int main(int argc, char *argv[]){
    if(argc<4){
        cout<<"参数过少！"<<endl;
        return -1;
    }
    if(argc>4){
        cout<<"参数过多！"<<endl;
        return -1;
    }


    m = atoi(argv[1]);
    N = pow(2, m);
    x = new double[N];
    x_1 = new complex<double>[N];
    e = new complex<double>[N];
    
    read_array(argv[2]);
    
    base2_dit_fft();

    write_array(argv[3]);

    delete[] x;
    delete[] x_1;
    delete[] e;
    
    return 0;
}


void read_array(const char* filename){
    FILE* fp; 
    fp = fopen(filename, "r");
    if(fp == NULL)
    {
        printf("Failed to open file!\n");
        return ;
    }
    int num_fields_read;
    for(int i = 0; i < N; i++)
    {
        num_fields_read = fscanf(fp, "%lf", &x[i]);
    }
    fclose(fp);
    return ;
}

void write_array(const char* filename){
    FILE* fp; 
    fp = fopen(filename, "w");
    if(fp == NULL)
    {
        printf("Failed to open file!\n");
        return ;
    }
    int num_fields_read;
    for(int i = 0; i < N; i++)
    {
        num_fields_read = fprintf(fp, "%lf ", x_1[i].real());
        num_fields_read = fprintf(fp, "%lf ", x_1[i].imag());
    }
    fclose(fp);
    return ;
}
