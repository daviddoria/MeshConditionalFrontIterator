#include "vtkMeshNormalConditionFrontIterator.h"

#include <vtkObjectFactory.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>

#include <vtkMath.h>

vtkStandardNewMacro(vtkMeshNormalConditionFrontIterator);

vtkMeshNormalConditionFrontIterator::vtkMeshNormalConditionFrontIterator()
{
  AcceptableAngle = 5.0;
}

bool vtkMeshNormalConditionFrontIterator::Condition(const int a, const int b)
{
  //cout << "Testing condition..." << endl;
  
  //get normals
  vtkSmartPointer<vtkFloatArray> normals = 
    //  vtkDoubleArray::SafeDownCast(this->Mesh->GetPointData()->GetArray("Normals"));
    vtkFloatArray::SafeDownCast(this->Mesh->GetPointData()->GetNormals());
  
  /*
  if(normals)
    { 
    cout << "There are " << normals->GetNumberOfTuples() << 
      " point normals." << endl;
    }
  else
    {
    cout << "Error: No point normals, can't evaluate condition." << endl;
    return false;
    }
  */
  double n1[3];
  normals->GetTuple(a, n1);
  vtkMath::Normalize(n1);
  //cout << "Point normal " << i << ": " << n[0] << " " << n[1] << " " << n[2] << endl;
  
  double n2[3];
  normals->GetTuple(b, n2);
  vtkMath::Normalize(n2);

  double angle = acos(vtkMath::Dot(n1,n2));
  
  if(angle > vtkMath::RadiansFromDegrees(this->AcceptableAngle))
    {
    //cout << "normals vary by " << vtkMath::DegreesFromRadians(angle) << endl;
    return false;
    }
  else
    {
    return true;
    }
}
