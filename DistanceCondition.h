#ifndef DISTANCECONDITION_H
#define DISTANCECONDITION_H

#include "Condition.h"

class DistanceCondition : public Condition
{
public:
  DistanceCondition();
  
  bool IsSimilar(const int a, const int b);
  void SetAcceptableDistance(float distance);
  
private:
  float AcceptableDistance; // The distance that determines if a point is close enough to its neighbor.
};

#endif
