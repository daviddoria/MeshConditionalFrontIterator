#include <vtkSmartPointer.h>
#include <vtkMath.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkExtractEdges.h>
#include <vtkObjectFactory.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkPolyDataNormals.h>

#include "vtkMeshConditionalFrontIterator.h"

class vtkMeshNormalConditionFrontIterator : public vtkMeshConditionalFrontIterator
{
  public:
    static vtkMeshNormalConditionFrontIterator* New();
    vtkTypeRevisionMacro(vtkMeshNormalConditionFrontIterator, vtkObject);
  
  private:
    bool Condition(const int a, const int b)
    {
      //cout << "Testing condition..." << endl;
      
      //get normals
      vtkSmartPointer<vtkFloatArray> normals = 
        //  vtkDoubleArray::SafeDownCast(this->Mesh->GetPointData()->GetArray("Normals"));
        vtkFloatArray::SafeDownCast(this->Mesh->GetPointData()->GetNormals());
      
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
        
      double n1[3];
      normals->GetTuple(a, n1);
      vtkMath::Normalize(n1);
      //cout << "Point normal " << i << ": " << n[0] << " " << n[1] << " " << n[2] << endl;
      
      double n2[3];
      normals->GetTuple(b, n2);
      vtkMath::Normalize(n2);
  
      double angle = acos(vtkMath::Dot(n1,n2));
      
      if(angle > vtkMath::RadiansFromDegrees(5.0))
        {
        cout << "normals vary by " << vtkMath::DegreesFromRadians(angle) << endl;
        return false;
        }
      else
        {
        return true;
        }
     }
};
vtkCxxRevisionMacro(vtkMeshNormalConditionFrontIterator, "$Revision$");
vtkStandardNewMacro(vtkMeshNormalConditionFrontIterator);

int main (int argc, char *argv[])
{
  
  vtkSmartPointer<vtkSphereSource> sphereSource = 
      vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->Update();
  
  vtkSmartPointer<vtkPolyDataNormals> normalsFilter = 
      vtkSmartPointer<vtkPolyDataNormals>::New();
  normalsFilter->SetInputConnection(sphereSource->GetOutputPort());
  normalsFilter->Update();
  
 
  vtkSmartPointer<vtkExtractEdges> extractEdges = 
      vtkSmartPointer<vtkExtractEdges>::New();
  //extractEdges->SetInputConnection(sphereSource->GetOutputPort());
  extractEdges->SetInputConnection(normalsFilter->GetOutputPort());
  extractEdges->Update();
  
  cout << "There are " << extractEdges->GetOutput()->GetNumberOfPoints() << " points." << endl;
  
  vtkSmartPointer<vtkMeshNormalConditionFrontIterator> iterator = 
      vtkSmartPointer<vtkMeshNormalConditionFrontIterator>::New();
  iterator->SetMesh(extractEdges->GetOutput());
  iterator->SetStartVertex(0);
  iterator->Initialize();
  
  while(iterator->HasNext())
    {
    vtkIdType nextVertex = iterator->Next();
    cout << "Next vertex: " << nextVertex << endl;
    }
    
  return EXIT_SUCCESS;
}
