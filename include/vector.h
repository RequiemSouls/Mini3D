#ifndef MINI3D_VECTOR_H_
#define MINI3D_VECTOR_H_

#include <stdlib.h>
#include <array>
#include <initializer_list>
namespace mini3d {

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
    : BaseVector<T, NUM_OF_DIM>(values) {}
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
        for (const T &val : this->values_) {
            T val2 = val * val;
            sum += val2;
        }
        return sqrt(sum);
    }

    void Normalize() {
        T length = CalcLength();
        for (T &val : this->values_) {
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

template <typename T, std::size_t NUM_OF_DIM>
    VectorImpl<T, NUM_OF_DIM> operator+(const VectorImpl<T, NUM_OF_DIM> &lhs,
                                        const VectorImpl<T, NUM_OF_DIM> &rhs) {
    VectorImpl<T, NUM_OF_DIM> result;
    for (std::size_t i = 0; i < NUM_OF_DIM; ++i) {
        result.set_value(i, lhs.get_value(i) + rhs.get_value(i));
    }
    return result;
}

template <typename T, std::size_t NUM_OF_DIM>
    VectorImpl<T, NUM_OF_DIM> operator-(const VectorImpl<T, NUM_OF_DIM> &lhs,
                                        const VectorImpl<T, NUM_OF_DIM> &rhs) {
    VectorImpl<T, NUM_OF_DIM> result;
    for (std::size_t i = 0; i < NUM_OF_DIM; ++i) {
        result.set_value(i, lhs.get_value(i) - rhs.get_value(i));
    }
    return result;
}

template <typename T, std::size_t NUM_OF_DIM>
    bool operator==(const VectorImpl<T, NUM_OF_DIM> &lhs,
                    const VectorImpl<T, NUM_OF_DIM> &rhs) {
    return lhs.IsEqualTo(rhs);
}

template <typename T, std::size_t NUM_OF_DIM>
    bool operator!=(const VectorImpl<T, NUM_OF_DIM> &lhs,
                    const VectorImpl<T, NUM_OF_DIM> &rhs) {
    return !lhs.IsEqualTo(rhs);
}

};
#endif