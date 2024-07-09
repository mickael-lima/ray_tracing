#ifndef _VECTOR_3D_
#define _VECTOR_3D_

#include <array>
#include <cmath>
#include <ostream>

/*
Criaremos uma pequena biblioteca para representar um vetor tridimensional (um modo oportuno para representar raios de luz no
espaço) junto com algumas operações fundamentais, como soma, normal, vetor oposto, etc.
*/

// TODO: escrever testes exaustivos para todos os overloading dessa classe
class Vec3 {
    public:
        Vec3()
            : m_vector{0, 0, 0} {}

        explicit Vec3(double x, double y = 0, double z = 0)
            : m_vector{x, y, z} {}

        double x() const { return m_vector[0]; }
        double y() const { return m_vector[1]; }
        double z() const { return m_vector[2]; }

        // Para essa classe ser realmente útil, precisamos configurar algumas simbologias usando operator overloading
        // Vetor oposto (-v)
        Vec3 operator-() const {
            return Vec3(-x(), -y(), -z());
        }

        // NOTE: realmente necessário?
        double operator[](int index) const {
            return m_vector.at(index);
        }

        // NOTE: realmente necessário?
        double& operator[](int index) {
            return m_vector.at(index);
        }

        // Soma entre vetores
        Vec3& operator+=(const Vec3 &vector) {
            m_vector.at(0) += vector.x();
            m_vector.at(1) += vector.y();
            m_vector.at(2) += vector.z();

            return *this;
        }

        // Multiplicação entre um vetor um escalar real t
        Vec3& operator*=(double t) {
            m_vector.at(0) *= t;
            m_vector.at(1) *= t;
            m_vector.at(2) *= t;

            return *this;
        }

        // Norma do vetor tridimensional: d^2 = x^2 + y^2 + z^2
        double length() const {
            auto squared_len = (x() * x()) + (y() * y()) + (z() * z());
            return std::sqrt(squared_len);
        }

    private:
        std::array<double, 3> m_vector;

};

// Alguns utilitários fora da classe Vec3, o inline assegura que
// haja apenas uma instância de cada operador independente de
// quantas vezes o arquivo for incluído

// Printar um objeto Vec3 diretamente no std::cout
inline std::ostream& operator<<(std::ostream& out, const Vec3& vetor) {
    return out << vetor.x() << " " << vetor.y() << " " << vetor.z();
}

// Subtração entre vetores
inline Vec3 operator-(const Vec3 &vetor_u, const Vec3 &vetor_v) {
    return Vec3 {
    vetor_u.x() - vetor_v.x(),
    vetor_u.y() - vetor_v.y(),
    vetor_u.z() - vetor_v.z()};
}

// Adição entre vetores
inline Vec3 operator+(const Vec3 &vetor_u, const Vec3 &vetor_v) {
    return Vec3{
    vetor_u.x() + vetor_v.x(),
    vetor_u.y() + vetor_v.y(),
    vetor_u.z() + vetor_v.z()};
}

// Produto entre vetor e escalar, escalar e vetor, etc, todas as ordem
// de operação são cobertas. NOTE: as duas operaoes só fazem sentido se forem
// definidas nessa exata ordem, pois o operador* na ordem escalar * vetor usa
// da definição dada para ordem vetor * escalar.
inline Vec3 operator*(const Vec3 &vetor, double scalar) {
    return Vec3{scalar*vetor.x(), scalar*vetor.y(), scalar*vetor.z()};
}

inline Vec3 operator*(double scalar, const Vec3 &vetor) {
    return vetor * scalar;
}

// Isso ajuda na clareza matemática do código
inline Vec3 operator/(const Vec3 &vetor, double scalar) {
    return (1/scalar) * vetor;
}

inline Vec3 operator/(double scalar, const Vec3 &vetor) {
    return (1/scalar) * vetor;
}

// Produto escalar entre dois vetores
inline double operator*(const Vec3 &vetor_u, const Vec3 &vetor_v) {
    return vetor_u.x() * vetor_v.x()
        + vetor_u.y() * vetor_v.y()
        + vetor_u.z() * vetor_v.z();
}

// Produto vetorial. NOTE: esse método pode não ser necessariamente o mais
// performático, abre margem para otimização
inline Vec3 operator%(const Vec3 &vetor_u, const Vec3 &vetor_w) {
    return Vec3{vetor_u.y() * vetor_w.z() - vetor_u.z() * vetor_w.y(),
                vetor_u.z() * vetor_w.x() - vetor_u.x() * vetor_w.z(),
                vetor_u.x() * vetor_w.y() - vetor_u.y() * vetor_w.x()};
}

inline Vec3 vetor_unit(const Vec3 &vetor) {
    return vetor / vetor.length();
}

#endif // _VECTOR_3D_
