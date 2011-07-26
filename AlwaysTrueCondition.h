#ifndef AlwaysTrueCondition_H
#define AlwaysTrueCondition_H

#include "Condition.h"

class AlwaysTrueCondition : public Condition
{
  bool IsSimilar(const int a, const int b) {return true;}
};

#endif
