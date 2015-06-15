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

  auto APa = AB * (projA / AB.len2());
  auto CPa = APa - AC;
  std::cout << "APa.len() = " << APa.len() << ", CPa.len() = " << CPa.len() << ", AB.len() = " << AB.len() << "\n";
  if(CPa.len() > r) {
    std::cout << AB.len();
    return 0;
  }
  std::cout << "(CPa * APa) = " << (CPa * APa) << "\n"; 
  std::cout << "CPa: " << CPa.x() << " " << CPa.y() << " " << CPa.z() << "\n";

  std::cout << "projA = " << projA << ", AB.len = " << AB.len() << "\n";

  auto BC = C - B;

  auto distAC = AC.len();
  auto distBC = BC.len();

  std::cout << "distAC = " << distAC << "\n";
  std::cout << "distBC = " << distBC << "\n";

  auto tangA = std::sqrt(distAC * distAC - r * r);
  auto tangB = std::sqrt(distBC * distBC - r * r);

  std::cout << "tangA = " << tangA << "\n";
  std::cout << "tangB = " << tangB << "\n";

  ;

  std::cout.setf(std::cout.fixed);
  std::cout.precision(2);

  std::cout << (tangA + tangB );

  return 0;
}
