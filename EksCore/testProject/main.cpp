#include "QDebug"
#include "Eigen/Core"

int main( )
  {
  Eigen::VectorXf a(3);
  Eigen::VectorXf b(a);
  Eigen::VectorXf c(b);

  a = Eigen::Vector4f(1, 2, 3, 4);
  b = Eigen::Vector4f(3, 2, 1, 0);

  c = a + b;

  /*Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic> x(1,3);
  Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic> y(3,1);

  x = y;

  x.resize(4,4);
  x.resize(2,3);

  y = x;

  y = a;

  Eigen::Vector3f d = a.block<3, 1>(0, 0);

  c = a;

  Eigen::Vector3f e = a.block<3, 1>(0, 0);*/

  Eigen::ArrayXf ddd = c;

  ddd = a;

  return EXIT_SUCCESS;
  }
