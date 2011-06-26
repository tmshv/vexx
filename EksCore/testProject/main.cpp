#include "QDebug"
#include "Eigen/Core"

int main( )
  {
  Eigen::VectorXf a(3);
  Eigen::VectorXf b(a);
  Eigen::VectorXf c(b);

  c(2) = 1.0f;

  //Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic> x(1,3);
  //Eigen::Matrix<float,Eigen::Dynamic,Eigen::Dynamic> y(3,1);

  //x = y;

  //Eigen::Vector3f d = a;

  c = a;

  Eigen::Vector3f e = a;

  Eigen::ArrayXf ddd = c;

  ddd = a;

  return EXIT_SUCCESS;
  }
