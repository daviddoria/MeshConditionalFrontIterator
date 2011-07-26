#ifndef NORMALCONDITION_H
#define NORMALCONDITION_H

#include "Condition.h"

class NormalCondition : public Condition
{
public:
  NormalCondition();
  
  void SetMesh(vtkPolyData* m);
  bool IsSimilar(const int a, const int b);
  void SetAcceptableAngle(float angle);
  
private:
  float AcceptableAngle; // The angle (in degrees) that determines if a normal is similar enough to its neighbor.
};

#endif
