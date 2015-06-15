#include <cmath>
#include <iostream>
#include <vector>

using std::string;
template<typename V> using vector = std::vector<V>;

struct vec_3d {
  double coords[3];

  vec_3d() {}
  vec_3d(double x, double y, double z) : coords {x, y, z} {}

  double &x () { return coords[0]; }
  double &y () { return coords[1]; }
  double &z () { return coords[2]; }

  double x () const { return coords[0]; }
  double y () const { return coords[1]; }
  double z () const { return coords[2]; }

  double len() const {
    return std::sqrt(len2());
  }

  double len2() const {
    return (coords[0] * coords[0] + coords[1] * coords[1] + coords[2] * coords[2]);
  }
};

vec_3d operator - (vec_3d a, vec_3d b) {
  return vec_3d(a.x() - b.x(), a.y() - b.y(), a.z() - b.z()); 
}

vec_3d operator * (vec_3d v, double d) {
  return vec_3d(v.x() * d, v.y() * d, v.z() * d);
}

double operator * (vec_3d a, vec_3d b) {
  return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

double _acos(double angle) {
  angle = std::min(1.0, std::max(-1.0, angle));
  return std::acos(angle);
}

int main() {
  std::cin.sync_with_stdio(false);

  vec_3d A, B, C;
  double r;

  std::cin >> A.x() >> A.y() >> A.z();
  std::cin >> B.x() >> B.y() >> B.z();
  std::cin >> C.x() >> C.y() >> C.z();
  std::cin >> r;

  auto AB = B - A;
  auto AC = C - A;

  if(AB.len() < 1e-7) {
    std::cout << AB.len();
    return 0;
  }

  double projA = AB * AC;
  if(projA < 0 || projA > AB.len2()) {
    std::cout << AB.len();
    return 0;
  }

  auto AP = AB * (projA / AB.len2());
  auto CP = AP - AC;

#ifdef TEST
  std::cout << "CP.len() = " << CP.len() << "\n";
#endif//#ifdef TEST

  if(CP.len() >= r) {
    std::cout << AB.len();
    return 0;
  }

  auto BC = C - B;

  auto distAC = AC.len();
  auto distBC = BC.len();

#ifdef TEST
  std::cout << "distAC = " << distAC << "\n";
  std::cout << "distBC = " << distBC << "\n";
#endif//#ifdef TEST

  auto tangA = std::sqrt(AC.len2() - r * r);
  auto tangB = std::sqrt(BC.len2() - r * r);

#ifdef TEST
  std::cout << "tangA = " << tangA << "\n";
  std::cout << "tangB = " << tangB << "\n";
#endif//#ifdef TEST

  auto angleA = _acos(r/AC.len());
  auto angleB = _acos(r/BC.len());

#ifdef TEST
  std::cout << "angleA = " << angleA << "\n";
  std::cout << "angleB = " << angleB << "\n";
#endif//#ifdef TEST

  auto CA = A - C;
  auto CB = B - C;

#ifdef TEST
  std::cout << "(CA * CB) = " << (CA * CB) << "\n";
  std::cout << "(CA * CB / CA.len()) = " << (CA * CB / CA.len()) << "\n";
  std::cout << "(CA * CB / CA.len() / CB.len()) = " << (CA * CB / CA.len() / CB.len()) << "\n";
#endif//#ifdef TEST

  auto angleACB  = _acos(CA * CB / CA.len() / CB.len());
#ifdef TEST
  std::cout << "angleACB = " << angleACB << "\n";
#endif//#ifdef TEST

  std::cout.setf(std::cout.fixed);
  std::cout.precision(2);

  std::cout << (tangA + tangB + r * (angleACB - angleA - angleB) );

  return 0;
}
