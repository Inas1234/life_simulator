#include <iostream>
#include <ctime>
#include <cmath>
#include <stdlib.h>

#define WIDTH 50
#define HEIGHT 50

float grid[HEIGHT][WIDTH] = {0};
float grid_diff[HEIGHT][WIDTH] = {0};
char level[] = " .:;ox%#@";
#define level_count (sizeof(level) - 1)

float ra = 21;
float ri = ra/3;
float b1 = 0.278;
float b2 = 0.365;
float d1 = 0.267;
float d2 = 0.445;
float an = 0.028;
float am = 0.147;
float dt = 0.01f;
float getRand(){
    return (float)rand() / RAND_MAX;
}


void random_grid(){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            grid[i][j] = getRand();
        }
    }
}

int emod(int a, int b){
    return (a % b + b) % b;
}


void display_grid(float grid[HEIGHT][WIDTH]) {
   for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++)
        {
            int level_index = (int)(grid[i][j] * (level_count - 1));
            char c = level[level_index];
            std::fputc(c, stdout);
            std::fputc(c, stdout);
        }
        std::fputc('\n', stdout);
    }
}


float sigma1(float x, float a){
    return 1/(1 + expf(-(x-a)*4/am));
}

float sigma2(float x, float a, float b){
    return sigma1(x, a) * (1 - sigma1(x, b));
}

float sigmam(float x, float y, float m){
    return x*(1 - sigma1(m, 0.5f)) + y*sigma1(m, 0.5f);
}

float s(float n, float m){
    return sigma2(n, sigmam(b1, d1, m), sigmam(b2, d2, m));
}

void compute_grid_diff(){
    for (int cy = 0; cy < HEIGHT; cy++){
        for (int cx = 0; cx < WIDTH; cx++){
            float m = 0, M = 0;
            float n = 0, N = 0;

            for(int dy = -(ra -1); dy <= (ra - 1); ++dy){
                for(int dx = -(ra - 1); dx <= (ra - 1); ++dx){
                    int x = emod(cx + dx, WIDTH);
                    int y = emod(cy + dy, HEIGHT);
                    if(dx * dx + dy * dy <= ri * ri){
                        m += grid[y][x];
                        M += 1;
                    }
                    if(dx * dx + dy * dy <= ra * ra){
                        n += grid[y][x];
                        N += 1;
                    }
                }
            }
            m/=M; 
            n/=N;
            float q = s(n, m);
            grid_diff[cy][cx] = 2*q - 1;
        }
    }
    
}

void clamp(float *x, float l, float h){
    if(*x < l) *x = l;
    if(*x > h) *x = h;
}

int main(){
    srand(time(NULL));
    random_grid();
    
    
    display_grid(grid);
    compute_grid_diff();

    for (;;)
    {
        compute_grid_diff();
        for (size_t y = 0; y < HEIGHT; ++y){
            for (size_t x = 0; x < WIDTH; ++x){
                grid[y][x] += dt * grid_diff[y][x];
                clamp(&grid[y][x], 0, 1);
            }

        }
        display_grid(grid);
    }

    

    return 0;
}