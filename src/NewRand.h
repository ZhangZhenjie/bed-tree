#include <math.h>

#define PI 3.1415926535


double surand()
{
  return( (double) rand()/RAND_MAX );
}

double urand(double low, double high)
{
  return(low+(high-low)*surand());
}

double genexp(double lambda)
{
  double u,x;
  u=surand();
  x=(-1/lambda)*log(u);
  return(x);
}

double gennor()
{
  double theta,rsq,x;
  theta=urand(0,2*PI);
  rsq=genexp(0.5);
  x=sqrt(rsq)*cos(theta);
  return(x);
}
