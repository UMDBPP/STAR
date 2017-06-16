#ifndef VECTOR_H
#define VECTOR_H

template <typename T> class Vector3 {
    public:
        Vector3() {
            x = 0;
            y = 0;
            z = 0;
        }
        Vector3(T _x, T _y, T _z) {
            x = _x;
            y = _y;
            z = _z;
        }
        ~Vector3() { }

        void operator=(const Vector3<T>& _newVec) {
            x = _newVec.x;
            y = _newVec.y;
            z = _newVec.z;
        }
        void operator+=(const Vector3<T>& _rhs) {
            x += _rhs.x;
            y += _rhs.y;
            z += _rhs.z;
        }
        void operator-=(const Vector3<T>& _rhs) {
            x -= _rhs.x;
            y -= _rhs.y;
            z -= _rhs.z;
        }
        template <typename U> void operator*=(U _rhs) {
            x *= _rhs;
            y *= _rhs;
            z *= _rhs;
        }
        template <typename U> void operator/=(U _rhs) {
            x /= _rhs;
            y /= _rhs;
            z /= _rhs;
        }

        T dot(const Vector3<T>& _rhs) {
            return (x * _rhs.x) + (y * _rhs.y) + (z * _rhs.z);
        }
        Vector3<T> cross(const Vector3<T>& _rhs) {
            Vector3<T> product_;
            product_.x = (y * _rhs.z) - (z * _rhs.y);
            product_.y = (z * _rhs.x) - (x * _rhs.z);
            product_.z = (x * _rhs.y) - (y * _rhs.x);
            return product_;
        }

        T x;
        T y;
        T z;
};

template <typename T> class Vector4 {
    public:
        Vector4() {
            x = 0;
            y = 0;
            z = 0;
            s = 0;
        }
        Vector4(T _x, T _y, T _z, T _s) {
            x = _x;
            y = _y;
            z = _z;
            s = _s;
        }
        ~Vector4() { }

        void operator=(const Vector4<T>& _newVec) {
            x = _newVec.x;
            y = _newVec.y;
            z = _newVec.z;
            s = _newVec.s;
        }
        void operator+=(const Vector4<T>& _rhs) {
            x += _rhs.x;
            y += _rhs.y;
            z += _rhs.z;
            s += _rhs.s;
        }
        void operator-=(const Vector4<T>& _rhs) {
            x -= _rhs.x;
            y -= _rhs.y;
            z -= _rhs.z;
            s -= _rhs.s;
        }
        template <typename U> void operator*=(U _rhs) {
            x *= _rhs;
            y *= _rhs;
            z *= _rhs;
            s *= _rhs;
        }
        template <typename U> void operator/=(U _rhs) {
            x /= _rhs;
            y /= _rhs;
            z /= _rhs;
            s /= _rhs;
        }

        T dot(const Vector3<T>& _rhs) {
            return (x * _rhs.x) + (y * _rhs.y) + (z * _rhs.z) + (s * _rhs.s);
        }

        T x;
        T y;
        T z;
        T s;
};

template <typename T> Vector3<T> operator+(Vector3<T> _lhs, const Vector3<T>& _rhs) {
    _lhs += _rhs;
    return _lhs;
}

template <typename T> Vector4<T> operator+(Vector4<T> _lhs, const Vector4<T>& _rhs) {
    _lhs += _rhs;
    return _lhs;
}

template <typename T> Vector3<T> operator-(Vector3<T> _lhs, const Vector3<T>& _rhs) {
    _lhs -= _rhs;
    return _lhs;
}

template <typename T> Vector4<T> operator-(Vector4<T> _lhs, const Vector4<T>& _rhs) {
    _lhs -= _rhs;
    return _lhs;
}

template <typename T> Vector3<T> operator*(Vector3<T> _lhs, const T& _rhs) {
    _lhs *= _rhs;
    return _lhs;
}

template <typename T> Vector3<T> operator*(const T& _lhs, Vector3<T> _rhs) {
    _rhs *= _lhs;
    return _rhs;
}

template <typename T> Vector3<T> operator/(Vector3<T> _lhs, const T& _rhs) {
    _lhs /= _rhs;
    return _lhs;
}

template <typename T> Vector4<T> operator*(Vector4<T> _lhs, const T& _rhs) {
    _lhs *= _rhs;
    return _lhs;
}

template <typename T> Vector4<T> operator*(const T& _lhs, Vector4<T> _rhs) {
    _rhs *= _lhs;
    return _rhs;
}

template <typename T> Vector4<T> operator/(Vector4<T> _lhs, const T& _rhs) {
    _lhs /= _rhs;
    return _lhs;
}

#endif // VECTOR_H
