#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkExtractEdges.h>

#include "vtkMeshFrontIterator.h"

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
  
  vtkSmartPointer<vtkMeshFrontIterator> meshFrontIterator = 
      vtkSmartPointer<vtkMeshFrontIterator>::New();
  meshFrontIterator->SetMesh(extractEdges->GetOutput());
  meshFrontIterator->SetStartVertex(0);
  meshFrontIterator->Initialize();
  
  while(meshFrontIterator->HasNext())
    {
    vtkIdType nextVertex = meshFrontIterator->Next();
    cout << "Next vertex: " << nextVertex << endl;
    }
    
  return EXIT_SUCCESS;
}
