#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkXMLPolyDataWriter.h>

#include "vtkPolyDataKeepPoints.h"

int main (int argc, char *argv[])
{

  vtkSmartPointer<vtkSphereSource> sphereSource =
      vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->Update();
  
  {
  vtkSmartPointer<vtkXMLPolyDataWriter> writer =
    vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName("input.vtp");
  writer->SetInputConnection(sphereSource->GetOutputPort());
  writer->Write();
  }
  
  vtkSmartPointer<vtkPolyDataKeepPoints> keepPoints =
      vtkSmartPointer<vtkPolyDataKeepPoints>::New();
  keepPoints->SetInputConnection(sphereSource->GetOutputPort());
  //keep all but the 0th point
  //for(vtkIdType i = 1; i < sphereSource->GetOutput()->GetNumberOfPoints(); i++)
  for(vtkIdType i = 5; i < sphereSource->GetOutput()->GetNumberOfPoints(); i++)
    {
    keepPoints->AddPointToKeep(i);
    }
  keepPoints->Update();
  
  {
  vtkSmartPointer<vtkXMLPolyDataWriter> writer =
    vtkSmartPointer<vtkXMLPolyDataWriter>::New();
  writer->SetFileName("output.vtp");
  writer->SetInputConnection(keepPoints->GetOutputPort());
  writer->Write();
  }
  
  return EXIT_SUCCESS;
}
