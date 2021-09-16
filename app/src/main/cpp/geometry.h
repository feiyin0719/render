#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <cassert>
#include <iostream>

template<int n, typename T>
struct vec {
    vec() = default;

    T &operator[](const int i) {
        assert(i >= 0 && i < n);
        return data[i];
    }

    T operator[](const int i) const {
        assert(i >= 0 && i < n);
        return data[i];
    }

    T norm2() const { return (*this) * (*this); }

    T norm() const { return std::sqrt(norm2()); }

    T data[n] = {0};
};

template<int n, typename T>
T operator*(const vec<n, T> &lhs, const vec<n, T> &rhs) {
    T ret = 0;
    for (int i = n; i--; ret += lhs[i] * rhs[i]);
    return ret;
}

template<int n, typename T>
vec<n, T> operator+(const vec<n, T> &lhs, const vec<n, T> &rhs) {
    vec<n, T> ret = lhs;
    for (int i = n; i--; ret[i] += rhs[i]);
    return ret;
}

template<int n, typename T>
vec<n, T> operator-(const vec<n, T> &lhs, const vec<n, T> &rhs) {
    vec<n, T> ret = lhs;
    for (int i = n; i--; ret[i] -= rhs[i]);
    return ret;
}

template<int n, typename T>
vec<n, T> operator*(const T &rhs, const vec<n, T> &lhs) {
    vec<n, T> ret = lhs;
    for (int i = n; i--; ret[i] *= rhs);
    return ret;
}

template<int n, typename T>
vec<n, T> operator*(const vec<n, T> &lhs, const T &rhs) {
    vec<n, T> ret = lhs;
    for (int i = n; i--; ret[i] *= rhs);
    return ret;
}

template<int n, typename T>
vec<n, T> operator/(const vec<n, T> &lhs, const T &rhs) {
    vec<n, T> ret = lhs;
    for (int i = n; i--; ret[i] /= rhs);
    return ret;
}

template<int n1, int n2, typename T>
vec<n1, T> embed(const vec<n2, T> &v, T fill = 1) {
    vec<n1, T> ret;
    for (int i = n1; i--; ret[i] = (i < n2 ? v[i] : fill));
    return ret;
}

template<int n1, int n2, typename T>
vec<n1, T> proj(const vec<n2, T> &v) {
    vec<n1, T> ret;
    for (int i = n1; i--; ret[i] = v[i]);
    return ret;
}


template<int n, typename T>
std::ostream &operator<<(std::ostream &out, const vec<n, T> &v) {
    for (int i = 0; i < n; i++) out << v[i] << " ";
    return out;
}

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct vec<2, T> {
    vec() = default;

    vec(T X, T Y) : x(X), y(Y) {}

    T &operator[](const int i) {
        assert(i >= 0 && i < 2);
        return i == 0 ? x : y;
    }

    T operator[](const int i) const {
        assert(i >= 0 && i < 2);
        return i == 0 ? x : y;
    }

    T norm2() const { return (*this) * (*this); }

    T norm() const { return std::sqrt(norm2()); }

    vec &normalize() {
        *this = (*this) / norm();
        return *this;
    }

    T x{}, y{};
};

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct vec<3, T> {
    vec() = default;

    vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}

    T &operator[](const int i) {
        assert(i >= 0 && i < 3);
        return i == 0 ? x : (1 == i ? y : z);
    }

    T operator[](const int i) const {
        assert(i >= 0 && i < 3);
        return i == 0 ? x : (1 == i ? y : z);
    }

    T norm2() const { return (*this) * (*this); }

    T norm() const { return std::sqrt(norm2()); }

    vec &normalize() {
        *this = (*this) / norm();
        return *this;
    }

    T x{}, y{}, z{};
};

/////////////////////////////////////////////////////////////////////////////////

template<int n, typename T>
struct dt;

template<int nrows, int ncols, typename T>
struct mat {
    vec<ncols, T> rows[nrows] = {{}};

    mat() = default;

    vec<ncols, T> &operator[](const int idx) {
        assert(idx >= 0 && idx < nrows);
        return rows[idx];
    }

    const vec<ncols, T> &operator[](const int idx) const {
        assert(idx >= 0 && idx < nrows);
        return rows[idx];
    }

    vec<nrows, T> col(const int idx) const {
        assert(idx >= 0 && idx < ncols);
        vec<nrows, T> ret;
        for (int i = nrows; i--; ret[i] = rows[i][idx]);
        return ret;
    }

    void set_col(const int idx, const vec<nrows, T> &v) {
        assert(idx >= 0 && idx < ncols);
        for (int i = nrows; i--; rows[i][idx] = v[i]);
    }

    static mat<nrows, ncols, T> identity() {
        mat<nrows, ncols, T> ret;
        for (int i = nrows; i--;)
            for (int j = ncols; j--; ret[i][j] = (i == j));
        return ret;
    }

    T det() const {
        return dt<ncols, T>::det(*this);
    }

    mat<nrows - 1, ncols - 1, T> get_minor(const int row, const int col) const {
        mat<nrows - 1, ncols - 1, T> ret;
        for (int i = nrows - 1; i--;)
            for (int j = ncols - 1; j--; ret[i][j] = rows[i < row ? i : i + 1][j < col ? j : j +
                                                                                             1]);
        return ret;
    }

    T cofactor(const int row, const int col) const {
        return get_minor(row, col).det() * ((row + col) % 2 ? -1 : 1);
    }

    mat<nrows, ncols, T> adjugate() const {
        mat<nrows, ncols, T> ret;
        for (int i = nrows; i--;)
            for (int j = ncols; j--; ret[i][j] = cofactor(i, j));
        return ret;
    }

    mat<nrows, ncols, T> invert_transpose() const {
        mat<nrows, ncols, T> ret = adjugate();
        return ret / (ret[0] * rows[0]);
    }

    mat<nrows, ncols, T> invert() const {
        return invert_transpose().transpose();
    }

    mat<ncols, nrows, T> transpose() const {
        mat<ncols, nrows, T> ret;
        for (int i = ncols; i--; ret[i] = this->col(i));
        return ret;
    }
};

/////////////////////////////////////////////////////////////////////////////////

template<int nrows, int ncols, typename T>
vec<nrows, T> operator*(const mat<nrows, ncols, T> &lhs, const vec<ncols, T> &rhs) {
    vec<nrows, T> ret;
    for (int i = nrows; i--; ret[i] = lhs[i] * rhs);
    return ret;
}

template<int R1, int C1, int C2, typename T>
mat<R1, C2, T> operator*(const mat<R1, C1, T> &lhs, const mat<C1, C2, T> &rhs) {
    mat<R1, C2, T> result;
    for (int i = R1; i--;)
        for (int j = C2; j--; result[i][j] = lhs[i] * rhs.col(j));
    return result;
}

template<int nrows, int ncols, typename T>
mat<nrows, ncols, T> operator*(const mat<nrows, ncols, T> &lhs, const T &val) {
    mat<nrows, ncols, T> result;
    for (int i = nrows; i--; result[i] = lhs[i] * val);
    return result;
}

template<int nrows, int ncols, typename T>
mat<nrows, ncols, T> operator/(const mat<nrows, ncols, T> &lhs, const T &val) {
    mat<nrows, ncols, T> result;
    for (int i = nrows; i--; result[i] = lhs[i] / val);
    return result;
}

template<int nrows, int ncols, typename T>
mat<nrows, ncols, T> operator+(const mat<nrows, ncols, T> &lhs, const mat<nrows, ncols, T> &rhs) {
    mat<nrows, ncols, T> result;
    for (int i = nrows; i--;)
        for (int j = ncols; j--; result[i][j] = lhs[i][j] + rhs[i][j]);
    return result;
}

template<int nrows, int ncols, typename T>
mat<nrows, ncols, T> operator-(const mat<nrows, ncols, T> &lhs, const mat<nrows, ncols, T> &rhs) {
    mat<nrows, ncols, T> result;
    for (int i = nrows; i--;)
        for (int j = ncols; j--; result[i][j] = lhs[i][j] - rhs[i][j]);
    return result;
}

template<int nrows, int ncols, typename T>
std::ostream &operator<<(std::ostream &out, const mat<nrows, ncols, T> &m) {
    for (int i = 0; i < nrows; i++) out << m[i] << std::endl;
    return out;
}

/////////////////////////////////////////////////////////////////////////////////

template<int n, typename T>
struct dt {
    static T det(const mat<n, n, T> &src) {
        T ret = 0;
        for (int i = n; i--; ret += src[0][i] * src.cofactor(0, i));
        return ret;
    }
};

template<typename T>
struct dt<1, T> {
    static T det(const mat<1, 1, T> &src) {
        return src[0][0];
    }
};

/////////////////////////////////////////////////////////////////////////////////

typedef vec<2, int> vec2;
typedef vec<3, int> vec3;
typedef vec<4, int> vec4;

typedef vec<2, float> vec2f;
typedef vec<3, float> vec3f;
typedef vec<4, float> vec4f;

template<typename T>
vec<3, T> cross(const vec<3, T> &v1, const vec<3, T> &v2) {
    return vec<3, T>{v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                     v1.x * v2.y - v1.y * v2.x};
}

#endif //__GEOMETRY_H__

