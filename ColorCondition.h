#ifndef COLORCONDITION_H
#define COLORCONDITION_H

#include "Condition.h"

class ColorCondition : public Condition
{
public:
  ColorCondition();
  
  void SetMesh(vtkPolyData* m);
  bool IsSimilar(const int a, const int b);
  void SetAcceptableDifference(float difference);
  
private:
  float AcceptableDifference; // The difference between colors that determines if two vertices are similar enough.
};

#endif
