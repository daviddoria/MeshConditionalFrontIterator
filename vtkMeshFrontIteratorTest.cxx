#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkExtractEdges.h>

#if VTK_MAJOR_VERSION>5 || (VTK_MAJOR_VERSION==5 && VTK_MINOR_VERSION>4)
#include <vtkstd/limits>
#else
#include <limits>
#endif

#include "vtkMeshFrontIterator.h"

template<class A>
bool fuzzyCompare(A a, A b) 
{
  return fabs(a - b) < vtkstd::numeric_limits<A>::epsilon();
}

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
  
  int ids[50] = {0,1,2,8,18,19,3,26,4,5,6,7,46,20,21,27,30,31,34,38,42,47,22,23,28,32,35,36,39,43,48,24,25,29,33,37,40,41,44,49,9,11,12,13,14,15,45,16,17,10};

  unsigned int counter = 0;
  while(meshFrontIterator->HasNext())
    {
    vtkIdType nextVertex = meshFrontIterator->Next();
    if(ids[counter] != nextVertex)
      {
      return EXIT_FAILURE;
      }
    counter++;
    }
    
    return EXIT_SUCCESS;
  
    
  return EXIT_SUCCESS;
}
