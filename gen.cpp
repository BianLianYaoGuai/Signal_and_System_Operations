#include <iostream>
#include <string>
#include <cstring>
#include <time.h>
#include <cmath>
using namespace std;


int main(int argc, char *argv[]){
    if(argc<2){
        cout<<"参数过少！"<<endl;
        return -1;
    }
    if(argc>2){
        cout<<"参数过多！"<<endl;
        return -1;
    }
    int m = atoi(argv[1]);
    int N = pow(2, m);
    double* x = new double[N];
    string str="./array/array_"+to_string(N)+"_";
    FILE* fp;
    for(int i=0;i<10;i++){
        time_t t;
        srand((unsigned)time(&t));
        for (int i = 0; i < N; i++) {
            x[i] = (double)rand() / (double)RAND_MAX; // 生成0到1之间的随机double数
        }
        cout<<"完成第"<<i+1<<"个向量的生成！"<<endl;
        fp = fopen((str+to_string(i)+".txt").c_str(), "w");
        if(fp == NULL)
        {
            cout<<"Failed to open file!"<<endl;
            return -1;
        }
        for(int i = 0; i < N; i++)
        {
                fprintf(fp, "%lf ", x[i]);
        }
        fclose(fp);
        cout<<"完成第"<<i+1<<"个向量的存储！"<<endl;
    }
    return 0;
}
