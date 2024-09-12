#include "../lib/vector3d.hpp"
#include "../lib/utility.hpp"

#include <gtest/gtest.h>
#include <array>
#include <cmath>
#include <string_view>

TEST(ClasseVetores3D, ConstrutorSemArgumentos) {
    Vec3 vetor{};
    std::array<double, 3> vetor_nulo{0, 0, 0};

    EXPECT_EQ(vetor.x(), vetor_nulo.at(0));
    EXPECT_EQ(vetor.y(), vetor_nulo.at(1));
    EXPECT_EQ(vetor.z(), vetor_nulo.at(2));
}

TEST(ClasseVetores3D, ConstrutorCom1Argumento) {
    Vec3 vetor{1};
    std::array<double, 3> vetor_esperado{1, 0, 0};

    EXPECT_EQ(vetor.x(), vetor_esperado.at(0));
    EXPECT_EQ(vetor.y(), vetor_esperado.at(1));
    EXPECT_EQ(vetor.z(), vetor_esperado.at(2));
}

TEST(ClasseVetores3D, ConstrutorCom2Argumento) {
    Vec3 vetor{1, 2};
    std::array<double, 3> vetor_esperado{1, 2, 0};

    EXPECT_EQ(vetor.x(), vetor_esperado.at(0));
    EXPECT_EQ(vetor.y(), vetor_esperado.at(1));
    EXPECT_EQ(vetor.z(), vetor_esperado.at(2));
}

TEST(ClasseVetores3D, ConstrutorCom3Argumento) {
    Vec3 vetor{1, 2, 3};
    std::array<double, 3> vetor_esperado{1, 2, 3};

    EXPECT_EQ(vetor.x(), vetor_esperado.at(0));
    EXPECT_EQ(vetor.y(), vetor_esperado.at(1));
    EXPECT_EQ(vetor.z(), vetor_esperado.at(2));
}

TEST(ClasseVetores3D, VetorInverso) {
    Vec3 vetor{1, 2, 3};
    Vec3 vetor_inverso{-1, -2, -3};
    Vec3 vetor_inv_por_operador = -vetor;

    EXPECT_EQ(vetor_inverso.x(), vetor_inv_por_operador.x());
    EXPECT_EQ(vetor_inverso.y(), vetor_inv_por_operador.y());
    EXPECT_EQ(vetor_inverso.z(), vetor_inv_por_operador.z());
}

TEST(ClasseVetores3D, SomaEntreVetores) {
    Vec3 vetor_u{2};
    Vec3 vetor_w{1,3};
    Vec3 vetor_resultante{};

    vetor_resultante += vetor_u;
    vetor_resultante += vetor_w;

    EXPECT_EQ(vetor_resultante.x(), 3);
    EXPECT_EQ(vetor_resultante.y(), 3);
    EXPECT_EQ(vetor_resultante.z(), 0);
}

TEST(ClasseVetores3D, ProdutoEntreVetorEEscalar) {
    double escalar{0.5};
    Vec3 vetor_u{2, 4, 6};

    vetor_u *= escalar;

    EXPECT_EQ(vetor_u.x(), 1);
    EXPECT_EQ(vetor_u.y(), 2);
    EXPECT_EQ(vetor_u.z(), 3);
}

TEST(ClasseVetores3D, Norma) {
    Vec3 vetor{3, 4, 5};
    auto norma = std::sqrt(50);

    EXPECT_EQ(norma, vetor.length());
}

TEST(OperadorVetores3D, Subtracao) {
    Vec3 vetor_u{1302, 1211, -1};
    Vec3 vetor_w{2, 11, -1};

    Vec3 resultado{vetor_u - vetor_w};

    EXPECT_EQ(resultado.x(), 1300);
    EXPECT_EQ(resultado.y(), 1200);
    EXPECT_EQ(resultado.z(), 0);
}

TEST(OperadorVetores3D, Soma) {
    Vec3 vetor_u{-10, -50, 2};
    Vec3 vetor_w{10, 50, -2};

    Vec3 resultado{vetor_u + vetor_w};

    EXPECT_EQ(resultado.x(), 0);
    EXPECT_EQ(resultado.y(), 0);
    EXPECT_EQ(resultado.z(), 0);
}

TEST(OperadorVetores3D, ProdutoEscalar) {
    Vec3 vetor_u{1, 2, 3};
    Vec3 vetor_w{4, 5, 6};

    double resultado{vetor_u * vetor_w};

    EXPECT_EQ(resultado, 32);
}

TEST(OperadorVetores3D, MultiplicacaoEntreVetorEscalar) {
    Vec3 vetor_u{1024, 64, 4};
    double scalar{0.25};

    Vec3 resultado1 = scalar * vetor_u;
    Vec3 resultado2 = vetor_u * scalar;

    Vec3 resultado3 = vetor_u / scalar;

    EXPECT_EQ(resultado1.x(), 256);
    EXPECT_EQ(resultado1.y(), 16);
    EXPECT_EQ(resultado1.z(), 1);

    EXPECT_EQ(resultado2.x(), 256);
    EXPECT_EQ(resultado2.y(), 16);
    EXPECT_EQ(resultado2.z(), 1);

    EXPECT_EQ(resultado3.x(), 4096);
    EXPECT_EQ(resultado3.y(), 256);
    EXPECT_EQ(resultado3.z(), 16);
}

TEST(OperadorVetores3D, ProdutoVetorial) {
    Vec3 vetor_u{1, 2, 3};
    Vec3 vetor_w{1, 5, 7};

    Vec3 resultado{vetor_u % vetor_w};

    EXPECT_EQ(resultado.x(), -1);
    EXPECT_EQ(resultado.y(), -4);
    EXPECT_EQ(resultado.z(), 3);
}

TEST(OperadorVetores3D, VetorUnitario) {
    Vec3 vetor{2, -4, 1};
    Vec3 resultado{vetor.unit()};

    EXPECT_EQ(resultado.x(), 2 / std::sqrt(21));
    EXPECT_EQ(resultado.y(), -(4 / std::sqrt(21)));
    EXPECT_EQ(resultado.z(), 1 / std::sqrt(21));
}

TEST(OperadorVetores3D, VetorRandom) {
    Vec3 vetor1 = Utility::random_vec();
    Vec3 vetor2 = Utility::random_vec();

    EXPECT_NE(vetor1.x(), vetor2.x());
    EXPECT_NE(vetor1.y(), vetor2.y());
    EXPECT_NE(vetor1.z(), vetor2.z());

    Vec3 vetor3 = Utility::random_vec(-1, 1);
    Vec3 vetor4 = Utility::random_vec(-1, 1);

    EXPECT_NE(vetor3.x(), vetor4.x());
    EXPECT_NE(vetor3.y(), vetor4.y());
    EXPECT_NE(vetor3.z(), vetor4.z());
}
