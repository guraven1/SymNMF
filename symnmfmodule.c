#include <Python.h>
#include "symnmf.h"

// Python wrapper function for updateH
static PyObject* py_updateH(PyObject* self, PyObject* args) {
    (void)self;
    PyObject* pyH;
    PyObject* pyW;
    int K;

    // Parse the arguments: H (2D list), W (2D list), K (int)
    if (!PyArg_ParseTuple(args, "OOi", &pyH, &pyW, &K)) {
        return NULL;
    }

    // Convert H and W from Python lists to C arrays
    double** H = (double**)malloc(V_D * sizeof(double*));
    double** W = (double**)malloc(V_D * sizeof(double*));
    for (int i = 0; i < V_D; i++) {
        PyObject* rowH = PyList_GetItem(pyH, i);
        PyObject* rowW = PyList_GetItem(pyW, i);

        H[i] = (double*)malloc(K * sizeof(double));
        W[i] = (double*)malloc(K * sizeof(double));

        for (int j = 0; j < K; j++) {
            H[i][j] = PyFloat_AsDouble(PyList_GetItem(rowH, j));
            W[i][j] = PyFloat_AsDouble(PyList_GetItem(rowW, j));
        }
    }

    // Call the updateH function
    updateH(H, W, K);

    // Convert the updated H back to a Python list of lists
    PyObject* pyUpdatedH = PyList_New(V_D);
    for (int i = 0; i < V_D; i++) {
        PyObject* row = PyList_New(K);
        for (int j = 0; j < K; j++) {
            PyList_SetItem(row, j, PyFloat_FromDouble(H[i][j]));
        }
        PyList_SetItem(pyUpdatedH, i, row);
    }

    // Free allocated memory for H and W
    for (int i = 0; i < V_D; i++) {
        free(H[i]);
        free(W[i]);
    }
    free(H);
    free(W);

    return pyUpdatedH;
}



// Python wrapper function for sym
static PyObject* py_sym(PyObject* self, PyObject* args) {
    (void)self;
    const char* filename;
    if (!PyArg_ParseTuple(args, "s", &filename)) {
        return NULL;
    }

    double **symMatrix = sym(filename);

    // Convert symMatrix to a Python list of lists
    PyObject* pySymMatrix = PyList_New(N);
    for (int i = 0; i < N; i++) {
        PyObject* row = PyList_New(N);
        for (int j = 0; j < N; j++) {
            PyList_SetItem(row, j, PyFloat_FromDouble(symMatrix[i][j]));
        }
        PyList_SetItem(pySymMatrix, i, row);
    }

    // Free allocated memory for symMatrix
    for (int i = 0; i < N; i++) {
        free(symMatrix[i]);
    }
    free(symMatrix);

    return pySymMatrix;
}

// Python wrapper function for ddg
static PyObject* py_ddg(PyObject* self, PyObject* args) {
    (void)self;
    const char* filename;
    if (!PyArg_ParseTuple(args, "s", &filename)) {
        return NULL;
    }

    double **ddgMatrix = ddg(filename);

    // Convert ddgMatrix to a Python list of lists
    PyObject* pyDdgMatrix = PyList_New(N);
    for (int i = 0; i < N; i++) {
        PyObject* row = PyList_New(N);
        for (int j = 0; j < N; j++) {
            PyList_SetItem(row, j, PyFloat_FromDouble(ddgMatrix[i][j]));
        }
        PyList_SetItem(pyDdgMatrix, i, row);
    }

    // Free allocated memory for ddgMatrix
    for (int i = 0; i < N; i++) {
        free(ddgMatrix[i]);
    }
    free(ddgMatrix);

    return pyDdgMatrix;
}

// Python wrapper function for norm
static PyObject* py_norm(PyObject* self, PyObject* args) {
    (void)self;
    const char* filename;
    if (!PyArg_ParseTuple(args, "s", &filename)) {
        return NULL;
    }

    double **normMatrix = norm(filename);

    // Convert normMatrix to a Python list of lists
    PyObject* pyNormMatrix = PyList_New(N);
    for (int i = 0; i < N; i++) {
        PyObject* row = PyList_New(N);
        for (int j = 0; j < N; j++) {
            PyList_SetItem(row, j, PyFloat_FromDouble(normMatrix[i][j]));
        }
        PyList_SetItem(pyNormMatrix, i, row);
    }

    // Free allocated memory for normMatrix
    for (int i = 0; i < N; i++) {
        free(normMatrix[i]);
    }
    free(normMatrix);

    return pyNormMatrix;
}

// Method definitions
static PyMethodDef symnmfMethods[] = {
    {"sym", py_sym, METH_VARARGS, "Calculate the similarity matrix."},
    {"ddg", py_ddg, METH_VARARGS, "Calculate the diagonal degree matrix."},
    {"norm", py_norm, METH_VARARGS, "Calculate the normalized Laplacian matrix."},
    {"updateH", py_updateH, METH_VARARGS, "Update H."},
    {NULL, NULL, 0, NULL} // Sentinel
};

static struct PyModuleDef symnmfModule = {
    PyModuleDef_HEAD_INIT,
    "symnmf",   // Module name
    NULL,       // Module documentation (optional)
    -1,         // Size of per-interpreter state of the module
    symnmfMethods,  // Method definitions
    NULL,       // m_slots (initialize to NULL)
    NULL,       // m_traverse (initialize to NULL)
    NULL,       // m_clear (initialize to NULL)
    NULL        // m_free (initialize to NULL)
};



// Module initialization
PyMODINIT_FUNC PyInit_symnmf(void) {
    return PyModule_Create(&symnmfModule);
}
