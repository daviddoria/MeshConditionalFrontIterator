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
#include "ColorCondition.h"

int main (int argc, char *argv[])
{
  vtkSmartPointer<vtkSphereSource> sphereSource = 
      vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->Update();
  
  ColorCondition* condition = new ColorCondition;
  condition->SetAcceptableDifference(15.);
  
  vtkSmartPointer<vtkMeshConditionalFrontIterator> iterator = 
      vtkSmartPointer<vtkMeshConditionalFrontIterator>::New();
  iterator->SetCondition(condition);
  iterator->SetMesh(sphereSource->GetOutput());
  iterator->SetStartVertex(0);
  iterator->Initialize();
  
  while(iterator->HasNext())
    {
    vtkIdType nextVertex = iterator->Next();
    std::cout << "Next vertex: " << nextVertex << std::endl;
    }
    
  return EXIT_SUCCESS;
}
