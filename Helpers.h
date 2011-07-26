#ifndef HELPERS_H
#define HELPERS_H

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkIdList.h>

void GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, int seed, vtkSmartPointer<vtkIdList> ids);

#endif
