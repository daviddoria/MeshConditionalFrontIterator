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
  
  vtkSmartPointer<vtkExtractEdges> extractEdges = 
      vtkSmartPointer<vtkExtractEdges>::New();
  extractEdges->SetInputConnection(sphereSource->GetOutputPort());
  extractEdges->Update();
  
  cout << "There are " << extractEdges->GetOutput()->GetNumberOfPoints() << " points." << endl;
  
  vtkSmartPointer<vtkMeshConditionalFrontIterator> meshConditionalFrontIterator = 
      vtkSmartPointer<vtkMeshConditionalFrontIterator>::New();
  meshConditionalFrontIterator->SetMesh(extractEdges->GetOutput());
  meshConditionalFrontIterator->SetStartVertex(0);
  meshConditionalFrontIterator->Initialize();
  
  while(meshConditionalFrontIterator->HasNext())
    {
    vtkIdType nextVertex = meshConditionalFrontIterator->Next();
    cout << "Next vertex: " << nextVertex << endl;
    }
    
  return EXIT_SUCCESS;
}
