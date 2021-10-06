#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>

template <class T>
class Matrix    //Матрицы
{
public:
    Matrix(int row, int col, T** array = NULL)
    {
        rows = row;
        cols = col;
        data = new T*[rows];
        for (size_t i = 0; i < rows; i++)
        {
            data[i] = new T[cols];
            if (array != NULL)
            {
                for(size_t j = 0; j < cols; j++)
                    data[i][j] = array[i][j];
            }
        }
    }
    Matrix(T array[4][4])
    {
        rows = 4;
        cols = 4;
        data = new T*[4];
        for (size_t i = 0; i < 4; i++)
        {
            data[i] = new T[4];
            for(size_t j = 0; j < 4; j++)
                data[i][j] = array[i][j];
        }
        print();
    }
    void SetFromStaticArray(T array[4][4])
    {
        for (size_t i = 0; i < 4; i++)
        {
            for(size_t j = 0; j < 4; j++)
                data[i][j] = array[i][j];
        }
    }

    ~Matrix()
    {
        for (size_t i = 0; i < rows; i++)
        {
            delete[] data[i];
        }
        delete[] data;
    }
    Matrix* operator* (const Matrix& b) const;
    T* operator [](size_t row); //Получить строку матрицы
    Matrix& operator*=(const Matrix& b)
    {
       Matrix<T>* new_m = (*this) * b;  //��������� ���������
       this->swap(new_m);
       delete new_m;
       return *this;
    }
    void swap(Matrix<T>* other)
    {
        size_t temp_rows = rows;
        size_t temp_cols = cols;
        T** temp_data = data;
        rows = other->rows;
        cols = other->cols;
        data = other->data;
        other->rows = temp_rows;
        other->cols = temp_cols;
        other->data = temp_data;
    }

    void print();
    //ToPoint3D
    size_t rows;
    size_t cols;
    T** data;
};

template <class T>
Matrix<T>* Matrix<T>::operator* (const Matrix& b) const  //Умножение матриц
{
    if (cols != b.rows)
    {
        qDebug()<<"DIFF SIZES!";
        return NULL;
    }
    Matrix<double>* result = new Matrix<double>(rows, b.cols);
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < b.cols; j++)
        {
            T sum = 0;
            for (size_t k = 0; k < cols; k++)
            {
                sum += data[i][k] * b.data[k][j];
            }
            result->data[i][j] = sum;
        }
    }
    return result;
}

template <class T>
T* Matrix<T>::operator [](size_t row) //Получить строку матрицы
{
    return data[row];
}

template <class T>
void Matrix<T>::print()
{
    for (size_t i = 0; i < rows; i++)
    {
        for(size_t j = 0; j < cols; j++)
            qDebug() <<"\t" << data[i][j];
        qDebug()<<"\n";
    }
}



#endif // MATRIX_H
