#ifndef MINI3D_MATRIX_H_
#define MINI3D_MATRIX_H_

#include "vector.h"
#include <string.h>
#include <math.h>
#include "config.h"
#include "SDL.h"

MINI_NS_BEGIN

template <typename T, std::size_t NUM_OF_ROWS, std::size_t NUM_OF_COLS>
    class BaseMatrix {
public:
    BaseMatrix() { memset(values_, 0, sizeof(values_)); }

    BaseMatrix(const std::initializer_list<T> &values) {
        T *p = reinterpret_cast<T *>(values_);
        std::copy(values.begin(), values.end(), p);
    }

    T get_value(std::size_t row, std::size_t col) const {
        return values_[row][col];
    }

    void set_value(std::size_t row, std::size_t col, T val) {
        values_[row][col] = val;
    }

    bool IsEqualTo(const BaseMatrix<T, NUM_OF_ROWS, NUM_OF_COLS> &rhs) const {
        for (std::size_t row = 0; row < NUM_OF_ROWS; ++row) {
            for (std::size_t col = 0; col < NUM_OF_COLS; ++col) {
                if (values_[row][col] != rhs.get_value(row, col)) {
                    return false;
                }
            }
        }
        return true;
    }

private:
    T values_[NUM_OF_ROWS][NUM_OF_COLS];
};

template <typename T, std::size_t NUM_OF_ROWS, std::size_t NUM_OF_COLS>
    class MatrixImpl : public BaseMatrix<T, NUM_OF_ROWS, NUM_OF_COLS> {
public:
    MatrixImpl() {}
MatrixImpl(const std::initializer_list<T> &values)
    : BaseMatrix<T, NUM_OF_ROWS, NUM_OF_COLS>(values) {}
};

template <typename T, std::size_t N>
    class MatrixImpl<T, N, N> : public BaseMatrix<T, N, N> {
public:
    MatrixImpl() {}
MatrixImpl(const std::initializer_list<T> &values)
    : BaseMatrix<T, N, N>(values) {}

    static MatrixImpl<T, N, N> IDENTITY() {
        MatrixImpl<T, N, N> result;
        for (std::size_t i = 0; i < N; ++i) {
            result.set_value(i, i, 1.0f);
        }
        return result;
    }

    void Transfer(const VectorImpl<T, N> &offset) {
        for (std::size_t i = 0; i < N - 1; ++i) {
            this->set_value(i, N - 1, offset.get_value(i));
        }
    }

    // | x*x+(1-x*x)*cos,   x*y*(1-cos)-z*sin, x*z*(1-cos)+y*sin, 0|
    // | x*y*(1-cos)+z*sin, y*y+(1-y*y)*cos,   y*z*(1-cos)-x*sin, 0|
    // | x*z*(1-cos)-y*sin, y*z*(1-cos)+x*sin, z*z+(1-z*z)*cos,   0|
    // | 0,                 0,                 0,                 1|
    void Rotate(const VectorImpl<T, 4> &vec, float delta) {
        float c = cos(delta);
        float s = sin(delta);

        T x = vec.get_x();
        T y = vec.get_y();
        T z = vec.get_z();
        T x2 = x*x;
        T y2 = y*y;
        T z2 = z*z;

        this->set_value(0, 0, x2 + (1 - x2) * c);
        this->set_value(0, 1, x * y * (1 - c) - z * s);
        this->set_value(0, 2, x * z * (1 - c) + y * s);

        this->set_value(1, 0, x * y * (1 - c) + z * s);
        this->set_value(1, 1, y2 + (1 - y2) * c);
        this->set_value(1, 2, y * z * (1 - c) - x * s);

        this->set_value(2, 0, x * z * (1 - c) - y * s);
        this->set_value(2, 1, y * z * (1 - c) + x * s);
        this->set_value(2, 2, z2 + (1 - z2) * c);
    }

    void Scale(const VectorImpl<T, N> &scale) {
        for (std::size_t i = 0; i < N - 1; ++i) {
            this->set_value(i, i, scale.get_value(i));
        }
    }
};

template <typename T, std::size_t NUM_OF_DIM>
    auto MakeMatrix(const VectorImpl<T, NUM_OF_DIM> &vec)
    -> MatrixImpl<T, NUM_OF_DIM, 1> {
    MatrixImpl<T, NUM_OF_DIM, 1> result;
    for (std::size_t i = 0; i < NUM_OF_DIM; ++i) {
        result.set_value(i, 0, vec.get_value(i));
    }
    return result;
}

template <typename T, std::size_t ROW1, std::size_t COL1, std::size_t COL2>
    auto operator*(const MatrixImpl<T, ROW1, COL1> &lhs,
                    const MatrixImpl<T, COL1, COL2> &rhs)
    -> MatrixImpl<T, ROW1, COL2> {
    MatrixImpl<T, ROW1, COL2> result;
    for (std::size_t row = 0; row < ROW1; ++row) {
        for (std::size_t col = 0; col < COL2; ++col) {
            T val = 0.0f;
            for (std::size_t i = 0; i < ROW1; ++i) {
                val += lhs.get_value(row, i) * rhs.get_value(i, col);
            }
            result.set_value(row, col, val);
        }
    }
    return result;
}

template <typename T, std::size_t ROW, std::size_t COL>
    auto operator*(const MatrixImpl<T, ROW, COL> &lhs,
                    const VectorImpl<T, COL> &rhs) -> VectorImpl<T, COL> {
    MatrixImpl<T, COL, 1> matrix = lhs * MakeMatrix(rhs);
    VectorImpl<T, COL> result;
    for (std::size_t i = 0; i < COL; ++i) {
        result.set_value(i, matrix.get_value(i, 0));
    }
    return result;
}

template <typename T, std::size_t ROW, std::size_t COL>
    bool operator==(const MatrixImpl<T, ROW, COL> &lhs,
                    const MatrixImpl<T, ROW, COL> &rhs) {
    return lhs.IsEqualTo(rhs);
}

template <typename T, std::size_t ROW, std::size_t COL>
    bool operator!=(const MatrixImpl<T, ROW, COL> &lhs,
                    const MatrixImpl<T, ROW, COL> &rhs) {
    return !lhs.IsEqualTo(rhs);
}

MINI_NS_END
#endif  // MINI3D_MATRIX_H_
