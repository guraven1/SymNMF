#ifndef SYM_NMF_H
#define SYM_NMF_H
#define BETA 0.5
#define TOLERANCE 1e-4
#define MAX_ITER 300
#define PY_SSIZE_T_CLEAN

extern int N;
extern int V_D;

// Function prototypes for symmnfmodule.c
double **data_creation(const char *filename);
double squaredEuclideanDistance(double *a, double *b, int d);
double **sym(const char *filename);
double **ddg(const char *filename);
double **norm(const char *filename);
void updateH(double** H, double** W, int K);
double frobeniusNorm(double** A, double** B, int rows, int cols);
void print_mat(double **mat);

#endif // SYM_NMF_H
