#include "../lib/vector3d.hpp"

Vec3& Vec3::operator+=(const Vec3& vector) {
    m_vector.at(0) += vector.x();
    m_vector.at(1) += vector.y();
    m_vector.at(2) += vector.z();

    return *this;
}

Vec3& Vec3::operator*=(double t) {
    m_vector.at(0) *= t;
    m_vector.at(1) *= t;
    m_vector.at(2) *= t;

    return *this;
}

double Vec3::squared_length() const {
    return (x() * x()) + (y() * y()) + (z() * z());
}

double Vec3::length() const {
    return std::sqrt(squared_length());
}

std::ostream& operator<<(std::ostream& out, const Vec3& vetor) {
    return out << vetor.x() << " " << vetor.y() << " " << vetor.z();
}

Vec3 operator-(const Vec3& vetor_u, const Vec3& vetor_v) {
    return Vec3 {
        vetor_u.x() - vetor_v.x(),
        vetor_u.y() - vetor_v.y(),
        vetor_u.z() - vetor_v.z() 
    };
}

Vec3 operator+(const Vec3& vetor_u, const Vec3& vetor_v) {
    return Vec3 {
        vetor_u.x() + vetor_v.x(),
        vetor_u.y() + vetor_v.y(),
        vetor_u.z() + vetor_v.z() 
    };
}

Vec3 operator*(const Vec3& vetor, double scalar) {
    return Vec3{ scalar * vetor.x(), scalar * vetor.y(), scalar * vetor.z() };
}

Vec3 operator*(double scalar, const Vec3& vetor) {
    return vetor * scalar;
}

Vec3 operator/(const Vec3& vetor, double scalar) {
    return (1 / scalar) * vetor;
}

Vec3 operator/(double scalar, const Vec3& vetor) {
    return vetor / scalar;
}

double operator*(const Vec3& vetor_u, const Vec3& vetor_v) {
    return vetor_u.x() * vetor_v.x()
         + vetor_u.y() * vetor_v.y()
         + vetor_u.z() * vetor_v.z();
}

Vec3 operator%(const Vec3& vetor_u, const Vec3& vetor_w) {
    return Vec3{ vetor_u.y() * vetor_w.z() - vetor_u.z() * vetor_w.y(),
                vetor_u.z() * vetor_w.x() - vetor_u.x() * vetor_w.z(),
                vetor_u.x() * vetor_w.y() - vetor_u.y() * vetor_w.x() };
}

