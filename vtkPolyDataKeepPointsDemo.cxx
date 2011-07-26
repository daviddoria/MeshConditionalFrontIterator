#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>

#include "vtkPolyDataKeepPoints.h"

int main (int argc, char *argv[])
{
  
  vtkSmartPointer<vtkSphereSource> sphereSource = 
      vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->Update();
  
  std::cout << "Input has " << sphereSource->GetOutput()->GetNumberOfPoints() << " points." << std::endl;
  
  vtkSmartPointer<vtkPolyDataKeepPoints> keepPoints =
      vtkSmartPointer<vtkPolyDataKeepPoints>::New();
  keepPoints->SetInputConnection(sphereSource->GetOutputPort());
  for(vtkIdType i = 5; i < sphereSource->GetOutput()->GetNumberOfPoints(); i++)
    {
    keepPoints->AddPointToKeep(i);
    }

  keepPoints->Update();

  std::cout << "Output has " << keepPoints->GetOutput()->GetNumberOfPoints() << " points." << std::endl;
  
  return EXIT_SUCCESS;
}
