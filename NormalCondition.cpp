#include "NormalCondition.h"

#include <vtkFloatArray.h>
#include <vtkMath.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>

NormalCondition::NormalCondition()
{
  this->AcceptableAngle = 5.0;
}

void NormalCondition::SetAcceptableAngle(float angle)
{
  this->AcceptableAngle = angle;
}

void NormalCondition::SetMesh(vtkPolyData* m)
{
  Condition::SetMesh(m);
  
  //check for normals
  vtkFloatArray* normals = 
    vtkFloatArray::SafeDownCast(this->Mesh->GetPointData()->GetNormals());
  if(!normals)
    { 
    std::cout << "Error: No point normals, can't evaluate condition." << std::endl;
    exit(-1);
    }
}

bool NormalCondition::IsSimilar(const int a, const int b)
{
  //get normals
  vtkFloatArray* normals = 
    vtkFloatArray::SafeDownCast(this->Mesh->GetPointData()->GetNormals());
  
  double n1[3];
  normals->GetTuple(a, n1);
  vtkMath::Normalize(n1);
  //cout << "Point normal " << i << ": " << n[0] << " " << n[1] << " " << n[2] << endl;
  
  double n2[3];
  normals->GetTuple(b, n2);
  vtkMath::Normalize(n2);

  double angle = vtkMath::DegreesFromRadians(acos(vtkMath::Dot(n1,n2)));
  
  if(angle > this->AcceptableAngle)
    {
    //cout << "normals vary by " << vtkMath::DegreesFromRadians(angle) << endl;
    return false;
    }
  else
    {
    return true;
    }
}
