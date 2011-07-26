#include "DistanceCondition.h"

#include <vtkFloatArray.h>
#include <vtkMath.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>

DistanceCondition::DistanceCondition()
{
  this->AcceptableDistance = 5.0;
}

void DistanceCondition::SetAcceptableDistance(float distance)
{
  this->AcceptableDistance = distance;
}

bool DistanceCondition::IsSimilar(const int a, const int b)
{
  double p1[3];
  this->Mesh->GetPoint(a, p1);
  
  double p2[3];
  this->Mesh->GetPoint(b, p2);

  double distance = sqrt(vtkMath::Distance2BetweenPoints(p1, p2));

  if(distance > this->AcceptableDistance)
    {
    return false;
    }
  else
    {
    return true;
    }
}
