#include <stdio.h>

float d(float(f)(float), float x){
    float dx = 0.001;
    return (f(x + dx) - f(x)) / (dx); 
}
float newton(float(f)(float), int N){
    float x = 1;
    for(int i = 0; i < N; i++){
        x = x - f(x)/d(f,x);
    }
    return x;
}
float fx(float x){
    return x * x - 10.f;
}
int main(int argc, char* argv[]){
    float x = newton(fx, 5);
    printf("got x = %f, f(x) = %f", x, fx(x));
}