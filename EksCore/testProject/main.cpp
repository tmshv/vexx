#include "QDebug"
#include "Eigen/Core"

int main( )
  {
  Eigen::VectorXf a(3);
  Eigen::VectorXf b(a);
  Eigen::VectorXf c(b);

  a = Eigen::Vector3f(1, 2, 3);
  b = Eigen::Vector3f(3, 2, 1);

  c = a + b;

  Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic> x(1,3);
  Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic> y(3,1);

  x = y;

  Eigen::Vector3f d = a;

  c = a;

  Eigen::Vector3f e = a;

  Eigen::ArrayXf ddd = c;

  ddd = a;

  return EXIT_SUCCESS;
  }
