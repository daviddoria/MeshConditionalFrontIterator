#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkExtractEdges.h>

#include "vtkMeshConditionalFrontIterator.h"

int main (int argc, char *argv[])
{
  vtkSmartPointer<vtkSphereSource> sphereSource = 
      vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->Update();
  
  vtkSmartPointer<vtkMeshConditionalFrontIterator> meshConditionalFrontIterator = 
      vtkSmartPointer<vtkMeshConditionalFrontIterator>::New();
  meshConditionalFrontIterator->SetMesh(sphereSource->GetOutput());
  meshConditionalFrontIterator->SetStartVertex(0);
  meshConditionalFrontIterator->Initialize();
  
  while(meshConditionalFrontIterator->HasNext())
    {
    vtkIdType nextVertex = meshConditionalFrontIterator->Next();
    std::cout << "Next vertex: " << nextVertex << std::endl;
    }
    
  return EXIT_SUCCESS;
}
