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
#include "DistanceCondition.h"

int main (int argc, char *argv[])
{
  vtkSmartPointer<vtkSphereSource> sphereSource = 
    vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->SetThetaResolution(20);
  sphereSource->SetPhiResolution(20);
  sphereSource->Update();
  
  vtkSmartPointer<vtkPolyDataNormals> normalsFilter = 
    vtkSmartPointer<vtkPolyDataNormals>::New();
  normalsFilter->SetInputConnection(sphereSource->GetOutputPort());
  normalsFilter->Update();
  
  DistanceCondition* condition = new DistanceCondition;
  condition->SetAcceptableDistance(1.);
    
  vtkSmartPointer<vtkMeshConditionalFrontIterator> iterator = 
    vtkSmartPointer<vtkMeshConditionalFrontIterator>::New();
  iterator->SetCondition(condition);
  iterator->SetMesh(normalsFilter->GetOutput());
  iterator->SetStartVertex(0);
  iterator->Initialize();
  
  while(iterator->HasNext())
    {
    vtkIdType nextVertex = iterator->Next();
    std::cout << "Next vertex: " << nextVertex << std::endl;
    }
    
  return EXIT_SUCCESS;
}
