#include "ColorCondition.h"

#include <vtkFloatArray.h>
#include <vtkMath.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>

ColorCondition::ColorCondition()
{
  this->AcceptableDifference = 5.0;
}

void ColorCondition::SetAcceptableDifference(float difference)
{
  this->AcceptableDifference = difference;
}

void ColorCondition::SetMesh(vtkPolyData* m)
{
  Condition::SetMesh(m);
  
  // Check for colors
  vtkUnsignedCharArray* colors = 
    vtkUnsignedCharArray::SafeDownCast(this->Mesh->GetPointData()->GetScalars());
  if(!colors)
    { 
    std::cout << "Error: No point colors, can't evaluate condition." << std::endl;
    exit(-1);
    }
}

bool ColorCondition::IsSimilar(const int a, const int b)
{
  //get normals
  vtkUnsignedCharArray* colors = 
    vtkUnsignedCharArray::SafeDownCast(this->Mesh->GetPointData()->GetScalars());
  
  unsigned char color1[3];
  colors->GetTupleValue(a, color1);
  
  unsigned char color2[3];
  colors->GetTupleValue(b, color2);

  double difference = 0; // What should this be??
  
  if(difference > this->AcceptableDifference)
    {
    //cout << "colors vary by " << endl;
    return false;
    }
  else
    {
    return true;
    }
}
