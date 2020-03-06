#include<iostream>
#include<math.h>
#include"tools.h"

double distance(vect a, vect b)
{
    double diffx = a[0] - b[0];
    double diffy = a[1] - b[1];
    return sqrt(diffx*diffx + diffy*diffy) ;
}

double prod_scal(vect a, vect b)
{
  return a[0]*b[0] + a[1]*b[1];
}

void projection(vect &res, vect a, vect b)
{
  double k = prod_scal(a,b)/prod_scal(b,b);
  res[0] = k*b[0];
  res[1] = k*b[1];
}

double norme(vect a)
{
  return sqrt(a[0]*a[0] + a[1]*a[1]);
}

double dist_point_ligne(vect a,vect b, vect c)
{
  
  return;
}