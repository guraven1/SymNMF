#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <Python.h>
#include "symnmf.h"

int N = 0;
int V_D = 0;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("An Error Has Occurred\n");
        exit(1);
    }
    
    const char* goal = argv[1];
    const char* filename = argv[2];

    if (strcmp(goal, "sym") == 0) {
        print_mat(sym(filename));
    }
    if (strcmp(goal, "ddg") == 0) {
        print_mat(ddg(filename));
    }
    if (strcmp(goal, "norm") == 0) {
        print_mat(norm(filename));
    }
}

void print_mat(double **mat) {
    int i,j;
    for (i = 0; i < N; i++) {
        for (j=0; j < N; j++) {
            printf("%.4f", mat[i][j]);
            if (j < N -1) {
                printf(",");
            }
            else {
                printf("\n");
            }
        }
    }
}

double frobeniusNorm(double** A, double** B, int rows, int cols) {
    double sum = 0.0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double diff = A[i][j] - B[i][j];
            sum += diff * diff;
        }
    }
    return sum;
}

double squaredEuclideanDistance(double* a, double* b, int d) {
    double distanceSquared = 0.0;
    for (int i = 0; i < d; i++) {
        double diff = a[i] - b[i];
        distanceSquared += pow(diff, 2.0);
    }
    return distanceSquared;
}


double **data_creation(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("An Error Has Occurrd\n");
        exit(1);
    }
    double **data = calloc(1, sizeof(double *));
    int m = 0;
    size_t read;
    size_t m_one = -1;
    char *line = NULL;
    size_t row_size = 0;


    while ((read = getline(&line, &row_size, file)) != m_one) {
        char *ptr;
        double value;
        size_t num_count;
        double *row = calloc(1, sizeof(double));
        row_size = 0;
        num_count = 0;
        ptr = line;

        while (*ptr != '\n') {
            int scanned = sscanf(ptr, "%lf", &value);
            if (scanned == -1) {
                break;
            }
            if (scanned == 1) {
                if (num_count >= row_size) {
                    row_size = num_count + 1;
                    row = realloc(row, row_size * sizeof(double));
                    if (row == NULL) {
                        free(row);
                    }
                }
                row[num_count] = value;
                num_count++;
                
            }

            while ((*ptr >= '0' && *ptr <= '9') || *ptr == '.' || *ptr == '-') {
                ptr++;

                
            }
            if (*ptr == ','){
                ptr++;
            }
        }


        if (num_count > 0) {
            data = realloc(data, (m + 1) * sizeof(double *));
            data[m] = row;
            m++;


        }
        else {
            free(row);
        }
    }
    free(line);
    N = m;
    V_D = row_size;
    return data;
    }

double **sym(const char* filename) {
    double **data = data_creation(filename);
    // Allocate memory for the result matrix
    double** result = (double**)malloc(N * sizeof(double*));
    for (int i = 0; i < N; i++) {
        result[i] = (double*)malloc(N * sizeof(double));
    }
    // Calculate the similarity matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i != j) {
                double distanceSquared = squaredEuclideanDistance(data[i], data[j], V_D);
                result[i][j] = exp(-distanceSquared / 2.0);
            } else {
                result[i][j] = 0.0;
            }
        }
    }

    // Free allocated memory for the data array
    for (int i = 0; i < N; i++) {
        free(data[i]);
    }
    free(data);

    return result;
}

double **ddg(const char* filename) {
    double **sym_matrix = sym(filename);

    // Allocate memory for the diagonal degree matrix (DDG)
    double** ddg_matrix = (double**)malloc(N * sizeof(double*));
    for (int i = 0; i < N; i++) {
        ddg_matrix[i] = (double*)calloc(N, sizeof(double));
    }

    // Calculate row sums and put them on the diagonal of the DDG
    for (int i = 0; i < N; i++) {
        double row_sum = 0.0;
        for (int j = 0; j < N; j++) {
            row_sum += sym_matrix[i][j];
        }
        ddg_matrix[i][i] = row_sum;
    }

    // Free allocated memory for the similarity matrix
    for (int i = 0; i < N; i++) {
        free(sym_matrix[i]);
    }
    free(sym_matrix);

    return ddg_matrix;
}

double **norm(const char* filename) {
   
    double **sym_matrix = sym(filename);
    double **ddg_matrix = ddg(filename);

    // Allocate memory for the normalized Laplacian matrix (norm matrix)
    double** norm_matrix = (double**)malloc(N * sizeof(double*));
    for (int i = 0; i < N; i++) {
        norm_matrix[i] = (double*)calloc(N, sizeof(double));
    }

    double** result = (double**)malloc(N * sizeof(double*));
    for (int i = 0; i < N; i++) {
        result[i] = (double*)calloc(N, sizeof(double));
    }

    // Compute the inverse square root of the diagonal degree matrix
    double** ddg_inv_sqrt = (double**)malloc(N * sizeof(double*));
    for (int i = 0; i < N; i++) {
        ddg_inv_sqrt[i] = (double*)malloc(N * sizeof(double));
        for (int j = 0; j < N; j++) {
            ddg_inv_sqrt[i][j] = (i == j) ? 1.0 / sqrt(ddg_matrix[i][j]) : 0.0;
        }
    }

    // Multiply ddg_inv_sqrt with sym_matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double value = 0.0;
            for (int k = 0; k < N; k++) {
                value += ddg_inv_sqrt[i][k] * sym_matrix[k][j];
            }
            norm_matrix[i][j] = value;
        }
    }
    // Multiply the result again with the inverse square root of the diagonal degree matrix
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double value = 0.0;
            for (int k = 0; k < N; k++) {
                value += norm_matrix[i][k] * ddg_inv_sqrt[k][j];
            }
            result[i][j] = value;
        }
    }

    // Free allocated memory for ddg_inv_sqrt and the similarity matrix and the diagonal degree matrixf and the norm_matrix
    for (int i = 0; i < N; i++) {
        free(ddg_inv_sqrt[i]);
        free(sym_matrix[i]);
        free(ddg_matrix[i]);
        free(norm_matrix[i]);
    }

    free(ddg_inv_sqrt);
    free(sym_matrix);
    free(ddg_matrix);
    free(norm_matrix);

    return result;
}

void updateH(double** H, double** W, int K) {
    // Allocate memory for the intermediate matrix WH
    double** WH = (double**)malloc(V_D * sizeof(double*));
    for (int i = 0; i < V_D; i++) {
        WH[i] = (double*)calloc(K, sizeof(double));
    }

    double** H_prev = (double**)malloc(V_D * sizeof(double*));
    for (int i = 0; i < V_D; i++) {
        H_prev[i] = (double*)malloc(K * sizeof(double));
    }

    int iteration = 0;
    double frobeniusNormSquaredDiff;

    do {
        // Store the current H values in H_prev
        for (int i = 0; i < V_D; i++) {
            for (int j = 0; j < K; j++) {
                H_prev[i][j] = H[i][j];
            }
        }

        // Calculate the WH matrix
        for (int i = 0; i < V_D; i++) {
            for (int j = 0; j < K; j++) {
                WH[i][j] = 0.0;
                for (int k = 0; k < K; k++) {
                    WH[i][j] += W[i][k] * H[i][k];
                }
            }
        }

        // Update H based on the given formula
        for (int i = 0; i < V_D; i++) {
            for (int j = 0; j < K; j++) {
                double numerator = H[i][j] * WH[i][j];
                double denominator = 0.0;
                for (int k = 0; k < V_D; k++) {
                    denominator += H[k][j] * WH[k][j];
                }
                H[i][j] = H[i][j] * (1.0 - BETA) + BETA * (W[i][j] * numerator / denominator);
            }
        }

        // Calculate Frobenius norm squared of the difference between H and H_prev
        frobeniusNormSquaredDiff = frobeniusNorm(H, H_prev, V_D, K);

        iteration++;
    } while (frobeniusNormSquaredDiff >= (TOLERANCE * TOLERANCE) && iteration < MAX_ITER);

    // Free allocated memory
    for (int i = 0; i < V_D; i++) {
        free(WH[i]);
        free(H_prev[i]);
    }
    free(WH);
    free(H_prev);
}




