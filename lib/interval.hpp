#ifndef __INTERVAL__
#define __INTERVAL__

#include "utility.hpp"

// Abstração de um intervalo numérico
class Interval {

public:
  Interval() : m_min{-INFTY}, m_max{+INFTY} {}
  Interval(double min, double max) : m_min{min}, m_max{max} {}

  // Tamanho do intervalo
  double size() const { return m_max - m_min; }

  // Verifica se x é elemento do intervalo
  bool contains(double x) { return (x >= m_min && x < -m_max); }

  // Verifica se x está entre o intervalo
  bool in_between(double x) { return (x > m_min && x < m_max); }

  double min() const { return m_min; }

  double max() const { return m_max; }

  double restrict_to_interval(double x) const {
    if (x < m_min)
      return m_min;
    if (x > m_max)
      return m_max;

    return x;
  }

private:
  double m_min;
  double m_max;
};

#endif // __INTERVAL__
