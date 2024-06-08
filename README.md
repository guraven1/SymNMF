# SymNMF

## Introduction
This project implements a clustering algorithm based on symmetric Non-negative Matrix Factorization (symNMF). The implementation includes applying the algorithm to various datasets and comparing it with Kmeans clustering. This document provides an overview of the mathematical basis, algorithms, code structure, and implementation requirements.

### SymNMF Algorithm
The SymNMF algorithm is based on the research presented in [1]. Given a set of n points X = {x1, x2, ..., xN} ∈ Rd, the algorithm proceeds as follows:

1. Form the similarity matrix A from X (see Section 1.1).
2. Compute the Diagonal Degree Matrix (see Section 1.2).
3. Compute the normalized similarity W (see Section 1.3).
4. Find Hn×k that minimizes the objective function: min H≥0 ||kW − HHT||_F^2, where k is the number of clusters.

### Similarity Matrix
The similarity matrix A ∈ Rn×n is defined using the formula:
aij = exp (−kxi − xjk^2 / 2) if i ≠ j, and aii = 0

### Diagonal Degree Matrix
The degree matrix D is a diagonal matrix with degrees d1, ..., dn on the diagonal, where di is the sum of similarities between the vertex xi and all other vertices.

### Normalized Similarity Matrix
The normalized similarity matrix W is defined as W = D^(-1/2)AD^(-1/2).

### Algorithm for Optimizing H
The process of finding the decomposition matrix H involves initialization and iterative updates until convergence (see Section 1.4).

### Deriving a Clustering Solution
H represents an association matrix. To derive a hard clustering, the algorithm assigns each element to the cluster with the highest association score (see Section 1.5).

## Project Description
The project includes the following files and components:

### Python Program (symnmf.py)
- Implementation of user command-line argument parsing and processing.
- Initialization of H when the goal is symnmf.
- Interface with the C extension.

### C Program (symnmf.c)
- Implementation of the core algorithm in C.
- Reading user command-line arguments.
- Output of required matrices.

### Python C API (symnmfmodule.c)
- Definition of C extension functions for Python.

### C Header file (symnmf.h)
- Definition of function prototypes used in symnmfmodule.c and implemented in symnmf.c.

### analysis.py
- Comparison of SymNMF to Kmeans using silhouette score.

### Setup (setup.py)
- Build configuration for creating the .so file.

### Makefile
- Script for building the symnmf executable and handling dependencies.

## References
[1] Da Kuang, Chris Ding, and Haesun Park. Symmetric nonnegative matrix factorization for graph clustering. In Proceedings of the 2012 SIAM International Conference on Data Mining (SDM), Proceedings, pages 106–117. Society for Industrial and Applied Mathematics, April 2012.
