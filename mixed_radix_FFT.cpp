#include <iostream>
#include <complex>
#include <vector>
#include <cmath>

using namespace std;

int myfactor(int num) {
    if (num < 2) {
        return num;
    }
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) {
            return i;
        }
    }
    return num;
}

complex<double> myexp(double phi) {
    return cos(phi) + sin(phi) * complex<double>(0, 1);
}


vector<complex<double>> mydft(vector<complex<double>>& x) {
    int N = x.size();
    vector<complex<double>> x_1(N, 0.0);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            x_1[i] += x[j] * complex<double>(cos(-2 * M_PI * i * j / N), sin(-2 * M_PI * i * j / N));
        }
    }
    return x_1;
}

vector<complex<double>> mixed_radix_fft(vector<complex<double>>& x) {
    int N = x.size();
    int N1 = myfactor(N);
    if (N1 == N) {
        return mydft(x);
    }
    int N2 = N / N1;
    vector<vector<complex<double>>> temp(N1, vector<complex<double>>(N2));
    for (int n1 = 0; n1 < N1; n1++) {
        vector<complex<double>> inner_dft(N2);
        for (int n2 = 0; n2 < N2; n2++) {
            inner_dft[n2] = x[N1 * n2 + n1];
        }
        inner_dft = mixed_radix_fft(inner_dft);
        for (int k2 = 0; k2 < N2; k2++) {
            inner_dft[k2] *= myexp(-2 * M_PI / N * n1 * k2);
        }
        temp[n1] = inner_dft;
    }
    vector<vector<complex<double>>> temp2(N2, vector<complex<double>>(N1));
    for (int i = 0; i < N2; i++) {
        for (int j = 0; j < N1; j++) {
            temp2[i][j] = temp[j][i];
        }
    }
    vector<vector<complex<double>>> temp3(N2, vector<complex<double>>(N1));
    for (int i = 0; i < N2; i++) {
        temp3[i] = mixed_radix_fft(temp2[i]);
    }
    vector<complex<double>> temp4(N);
    for (int i = 0; i < N1; i++) {
        for (int j = 0; j < N2; j++) {
            temp4[i * N2 + j] = temp3[j][i];
        }
    }
    
    return temp4;
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
    
    vector<complex<double>> in;
    int N = atoi(argv[1]);
    in.reserve(N);
    
    
    
    FILE* fp; 
    fp = fopen(argv[2], "r");
    if(fp == NULL)
    {
        printf("Failed to open file!\n");
        return -1;
    }
    int num_fields_read;
    double temp;
    for(int i = 0; i < N; i++)
    {
        num_fields_read = fscanf(fp, "%lf", &temp);
        in.emplace_back(temp, 0);
    }
    fclose(fp);
    
    
    
    
    vector<complex<double>> ans;
    ans = mixed_radix_fft(in);
    
    
    
    
    fp = fopen(argv[3], "w");
    if(fp == NULL)
    {
        printf("Failed to open file!\n");
        return -1;
    }
    for(int i = 0; i < N; i++)
    {
        num_fields_read = fprintf(fp, "%lf ", ans[i].real());
        num_fields_read = fprintf(fp, "%lf ", ans[i].imag());
    }
    fclose(fp);
    
    return 0;
}
