#ifndef _VECTOR_3D_
#define _VECTOR_3D_

#include <array>
#include <cmath>
#include <ostream>


/*
Criaremos uma pequena biblioteca para representar um vetor tridimensional (um modo oportuno para representar raios de luz no
espaço) junto com algumas operações fundamentais, como soma, normal, vetor oposto, etc.
*/

class Vec3 {
    public:
        explicit Vec3(double x = 0, double y = 0, double z = 0)
            : m_vector{x, y, z} {}

        double x() const { return m_vector[0]; }
        double y() const { return m_vector[1]; }
        double z() const { return m_vector[2]; }

        // Vetor oposto (-v)
        Vec3 operator-() const { return Vec3(-x(), -y(), -z()); }

        // Soma entre vetores
        Vec3& operator+=(const Vec3& vector);

        // Multiplicação entre um vetor um escalar real t
        Vec3& operator*=(double t);

        // Retorna o módulo do vetor ao quadrado, para evitar custos de processamentos
        // relacionados a extrair a raíz quadrada em situações que não é necessário fazê-la
        double squared_length() const;

        // Norma do vetor tridimensional: d = sqrt(x^2 + y^2 + z^2)
        double length() const;

        // Majoritariamente, usado para printar o objecto Vec3 na forma X Y Z 
        friend std::ostream& operator<<(std::ostream& out, const Vec3& vetor);

        // Subtração entre vetores
        friend Vec3 operator-(const Vec3& vetor_u, const Vec3& vetor_v);

        // Soma entre vetores
        friend Vec3 operator+(const Vec3& vetor_u, const Vec3& vetor_v);

        // Produto entre vetor e escalar, escalar e vetor, etc, todas as ordem
        // de operação são cobertas.
        friend Vec3 operator*(const Vec3& vetor, double scalar);
        friend Vec3 operator*(double scalar, const Vec3& vetor);

        // Em teoria, apenas com os operadores acima é possível fazer divisão de
        // escalar por vetor, porém definir esse operador ajudará a manter uma 
        // sintaxe mais natural ao manipular objetos Vec3
        friend Vec3 operator/(const Vec3& vetor, double scalar);
        friend Vec3 operator/(double scalar, const Vec3& vetor);

        // Produto escalar
        friend double operator*(const Vec3& vetor_u, const Vec3& vetor_v);

        // Produto vetorial. NOTE: esse método pode não ser necessariamente o mais
        // performático, abre margem para otimização
        friend Vec3 operator%(const Vec3& vetor_u, const Vec3& vetor_w);

        // Vetor unitário
        Vec3 unit() const { return *this / this->length(); };

        // Retorna se o vetor está próximo do valor 0 em suas componentes
        bool near_zero() const;

      private:
        std::array<double, 3> m_vector;
};


// Representar pontos e cores como vetores pode não ser a melhor abordagem. TODO: Refatorar
using Point3 = Vec3;

#endif // _VECTOR_3D_
