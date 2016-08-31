#ifndef MINI3D_GEOMETRY_H_
#define MINI3D_GEOMETRY_H_

#include <math.h>
#include <string.h>
#include "types.h"

#include <array>
#include <initializer_list>

namespace mini3d {

namespace math {

template <typename T, std::size_t NUM_OF_DIM>
class BaseVector {
public:
    BaseVector() {
        for (T &val : values_) {
            val = 0;
        }
    }

    BaseVector(const std::initializer_list<T> &values) {
        std::copy(values.begin(), values.end(), values_);
    }

    T get_value(std::size_t index) const { return values_[index]; }
    void set_value(std::size_t index, T value) { values_[index] = value; }

    void AddBy(const BaseVector<T, NUM_OF_DIM> &rhs) {
        if (this != &rhs) {
            for (std::size_t i = 0; i < NUM_OF_DIM; ++i) {
                values_[i] += rhs.get_value(i);
            }
        }
        return *this;
    }

    void SubBy(const BaseVector<T, NUM_OF_DIM> &rhs) {
        if (this != &rhs) {
            for (std::size_t i = 0; i < NUM_OF_DIM; ++i) {
                values_[i] -= rhs.get_value(i);
            }
        }
        return *this;
    }

    bool IsEqualTo(const BaseVector<T, NUM_OF_DIM> &rhs) const {
        for (std::size_t i = 0; i < NUM_OF_DIM; ++i) {
            if (rhs.get_value(i) != values_[i]) {
                return false;
            }
        }
        return true;
    }

protected:
    T values_[NUM_OF_DIM];
};

template <typename T, std::size_t NUM_OF_DIM>
class VectorImpl : public BaseVector<T, NUM_OF_DIM> {
public:
    VectorImpl(const std::initializer_list<T> &values)
        : BaseVector<T, NUM_OF_DIM>(std::move(values)) {}
};

template <typename T>
class VectorImpl<T, 2> : public BaseVector<T, 2> {
public:
    VectorImpl(const std::initializer_list<T> &values)
        : BaseVector<T, 2>(values) {}
    VectorImpl() : VectorImpl({0.0f, 0.0f}) {}

    T get_x() const { return this->get_value(0); }
    T get_y() const { return this->get_value(1); }

    void set_x(T val) { this->set_value(0, val); }
    void set_y(T val) { this->set_value(1, val); }

    T CalcLength() const {
        T sum = 0;
        auto &values = this->get_values();
        for (const T &val : values) {
            T val2 = val * val;
            sum += val2;
        }
        return sqrt(sum);
    }

    void Normalize() {
        T length = CalcLength();
        auto &values = this->get_values();
        for (T &val : values) {
            val /= length;
        }
    }
};

template <typename T>
class VectorImpl<T, 4> : public BaseVector<T, 4> {
public:
    VectorImpl(const std::initializer_list<T> &values)
        : BaseVector<T, 4>(values) {}
    VectorImpl() : VectorImpl({0.0f, 0.0f, 0.0f, 1.0f}) {}

    T get_x() const { return this->get_value(0); }
    T get_y() const { return this->get_value(1); }
    T get_z() const { return this->get_value(2); }
    T get_w() const { return this->get_value(3); }

    void set_x(T val) { this->set_value(0, val); }
    void set_y(T val) { this->set_value(1, val); }
    void set_z(T val) { this->set_value(2, val); }
    void set_w(T val) { this->set_value(3, val); }

    T IncX() { return ++this->values_[0]; }
    T IncY() { return ++this->values_[1]; }
    T IncZ() { return ++this->values_[2]; }
    T IncW() { return ++this->values_[3]; }

    T DecX() { return --this->values_[0]; }
    T DecY() { return --this->values_[1]; }
    T DecZ() { return --this->values_[2]; }
    T DecW() { return --this->values_[3]; }

    void Homogenize() {
        set_x(get_x() / get_w());
        set_y(get_y() / get_w());
        set_z(get_z() / get_w());
        set_w(1.0f);
    }
};

template <typename T, int NUM_OF_DIM>
VectorImpl<T, NUM_OF_DIM> operator+(const VectorImpl<T, NUM_OF_DIM> &lhs,
                                    const VectorImpl<T, NUM_OF_DIM> &rhs) {
    VectorImpl<T, NUM_OF_DIM> result;
    for (std::size_t i = 0; i < NUM_OF_DIM; ++i) {
        result.set_value(i, lhs.get_value(i) + rhs.get_value(i));
    }
    return result;
}

template <typename T, int NUM_OF_DIM>
VectorImpl<T, NUM_OF_DIM> operator-(const VectorImpl<T, NUM_OF_DIM> &lhs,
                                    const VectorImpl<T, NUM_OF_DIM> &rhs) {
    VectorImpl<T, NUM_OF_DIM> result;
    for (std::size_t i = 0; i < NUM_OF_DIM; ++i) {
        result.set_value(i, lhs.get_value(i) - rhs.get_value(i));
    }
    return result;
}

template <typename T, std::size_t NUM_OF_ROWS, std::size_t NUM_OF_COLS>
class BaseMatrix {
public:
    BaseMatrix() { memset(values_, 0, sizeof(values_)); }

    BaseMatrix(const std::initializer_list<T> &values) {
        T *p = values_;
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

    void Transfer(const math::VectorImpl<T, N> &offset) {
        for (std::size_t i = 0; i < N - 1; ++i) {
            this->set_value(i, N - 1, offset.get_value(i));
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

}  // namespace math

typedef math::VectorImpl<F32, 2> Vec2;
typedef math::VectorImpl<F32, 4> Vector;
typedef math::MatrixImpl<F32, 4, 4> Matrix;

struct Color {
    UI8 r = 0;
    UI8 g = 0;
    UI8 b = 0;

    Color() = default;
    Color(UI8 cr, UI8 cg, UI8 cb) {
        r = cr;
        g = cg;
        b = cb;
    }

    bool operator==(const Color &other) const {
        return (r == other.r) && (g == other.g) && (b == other.b);
    }

    bool operator!=(const Color &other) const {
        return (r != other.r) || (g != other.g) || (b != other.b);
    }

    static const Color ZERO;
    static const Color WHITE;
};

struct Vertex {
    Vector p;
    Color c;
};

// Vector operator*(const Matrix &m, const Vector &v);

}  // namespace mini3d

#endif  // MINI3D_GEOMETRY_H_
