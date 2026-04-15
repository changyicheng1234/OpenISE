#include <iostream>

class Matrix {
private:
    double *data;
    int nRows, nColumns;
public:
    Matrix(const int r, const int c) : nRows(r), nColumns(c) {
        data = new double[nRows * nColumns];
        for (int i = 0; i < nRows * nColumns; ++i) data[i] = 0;
        //memset(data, 0, sizeof(double) * nRows * nColumns);
    }

    Matrix(const Matrix &m) : nRows(m.nRows), nColumns(m.nColumns) {
        data = new double[nRows * nColumns];
        //memcpy(data, m.data, m.nRows * m.nColumns);
        for (int i = 0; i < nRows * nColumns; ++i) data[i] = m.data[i];
    }

    ~Matrix() {
        delete[] data;
    }
    
    int getRow() {return nRows;}
    int getCol() {return nColumns;}

    Matrix operator=(const Matrix& m) {
        if (this == &m)
            return *this;

        delete[] data;

        nRows = m.nRows;
        nColumns = m.nColumns;

        data = new double[nRows * nColumns];
        //memcpy(data, m.data, m.nRows * m.nColumns);
        for (int i = 0; i < nRows * nColumns; ++i) data[i] = m.data[i];

        return *this;
    }

    int nElements() {return nRows * nColumns;}

    int size() { return (sizeof(double) * nColumns * nRows); }

    bool set(int r, int c, double val) {
        if (r >= nRows || c >= nColumns) return false;
        data[r*nColumns+c]=val;
        return true;
    }
    bool get(int r = -1, int c = -1) {
        if (r >= nRows || c >= nColumns) return false;
        if (r == -1) {
            if (c == -1) {
                for (int i = 0; i < nRows; ++i) {
                    for (int j = 0; j < nColumns; ++j) printf("%lf ", data[i * nColumns + j]);
                    printf("\n");
                }
            }
            else {
                for (int i = 0; i < nRows; ++i) printf("%lf ", data[i * nColumns + c]);
                    printf("\n");
            }
        }
        else {
            if (c == -1) {
                for (int j = 0; j < nColumns; ++j) printf("%lf ", data[r * nColumns + j]);
                printf("\n");
            }
            else {
                printf("%lf\n", data[r * nColumns + c]);
            }
        }
        return true;
    }
    bool add(const Matrix &m) {
        if (nRows != m.nRows || nColumns != m.nColumns) return false;
        for (int i = 0; i < nRows*nColumns; ++i) data[i] += m.data[i];
        return true;
    }
    bool sub(const Matrix &m) {
        if (nRows != m.nRows || nColumns != m.nColumns) return false;
        for (int i = 0; i < nRows*nColumns; ++i) data[i] -= m.data[i];
        return true;
    }
    Matrix operator+(const Matrix &m)const {
        if (nRows != m.nRows || nColumns != m.nColumns) {throw "Matrix size do not match!";}
        Matrix result(*this);
        for (int i = 0; i < nRows*nColumns; ++i) result.data[i] += m.data[i];
        return result;
    }
    Matrix operator-(const Matrix &m)const {
        if (nRows != m.nRows || nColumns != m.nColumns) {throw "Matrix size do not match!";}
        Matrix result(*this);
        for (int i = 0; i < nRows*nColumns; ++i) result.data[i] -= m.data[i];
        return result;
    }
};


int main() {
    Matrix a(3, 5);
    printf("A = \n%d  %d\n", a.getRow(), a.getCol());
    a.get();
    printf("\n");
    a.set(0, 0, 2);
    printf("A[0] = \n");
    a.get(0);
    printf("\n");
    printf("A[][0] = \n");
    a.get(-1, 0);
    printf("\n");

    Matrix b(a);
    printf("B = \n%d  %d\n", b.getRow(), b.getCol());
    b.get();
    printf("\n");

    Matrix c(2, 2);
    c = a;
    printf("C = \n%d  %d\n", c.getRow(), c.getCol());
    c.get();
    printf("\n");

    a.set(2, 3, 1);
    printf("A = \n%d  %d\n", a.getRow(), a.getCol());
    a.get();
    printf("\n");
    c = a + b + a + b;
    printf("C = \n");
    c.get();
    printf("\n");

    printf("A = \n");
    a.get();
    printf("\n");

    return 0;
}