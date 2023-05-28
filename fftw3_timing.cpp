#include<iostream>
#include<iomanip>
#include<fftw3.h>
#include <time.h>
#include <cmath>
using namespace std;


int m;
int N;
double* x;



void read_array(const char* filename);


int main(int argc, char *argv[]){
    if(argc<2){
        cout<<"请输入m！"<<endl;
        return -1;
    }
    if(argc>2){
        cout<<"参数过多！"<<endl;
        return -1;
    }
    m = atoi(argv[1]);
    N = pow(2, m);
    x = new double[N];
    fftw_complex *in,*out;
    
    double time = 0;
    for(int i=0;i<10;i++)
    {
        read_array(("./array/array_"+to_string(N)+"_"+to_string(i)+".txt").c_str());
    
    
        
        in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

        if((in==NULL)||(out==NULL)){
            printf("Error:insufficient available memory\n");
        }
        else{
            for(int i=0; i<N; i++){
                in[i][0] = x[i];
                in[i][1] = 0;
            }
        }

        // -----------------------------------
        struct timespec sts,ets;
        timespec_get(&sts, TIME_UTC);
        // -----------计时部分的工作-----------

        fftw_plan  p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD,FFTW_ESTIMATE);

        fftw_execute(p); 
        fftw_destroy_plan(p);
        fftw_cleanup();

    // -----------------------------------
        timespec_get(&ets, TIME_UTC);
        time_t dsec=ets.tv_sec-sts.tv_sec;
        long dnsec=ets.tv_nsec-sts.tv_nsec;
        if (dnsec<0){
            dsec--;
            dnsec+=1000000000ll;
        }
        time += (dsec + dnsec * 1e-9);
        // -----------计时结束-----------------
    }
    time /=10;
    printf ("%lf\n",time);


    if(in!=NULL) fftw_free(in);
    if(out!=NULL) fftw_free(out);

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
    int num_fields_read ;
    for(int i = 0; i < N; i++)
    {
        num_fields_read = fscanf(fp, "%lf", &x[i]);
    }
    fclose(fp);
    return ;
}