#ifndef __vtkPolyDataKeepPoints_h
#define __vtkPolyDataKeepPoints_h

#include "vtkPolyDataAlgorithm.h"
#include "vtkSmartPointer.h"

#include <set>
class vtkIdTypeArray;

class vtkPolyDataKeepPoints : public vtkPolyDataAlgorithm
{
public:
  
  vtkTypeRevisionMacro(vtkPolyDataKeepPoints,vtkPolyDataAlgorithm);
  
  static vtkPolyDataKeepPoints *New();

  void AddPointToKeep(vtkIdType id);

  void SetKeepArray(vtkSmartPointer<vtkIdTypeArray> arr);
  
protected:
  vtkPolyDataKeepPoints();
  ~vtkPolyDataKeepPoints(){}
  
  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

private:
  vtkPolyDataKeepPoints(const vtkPolyDataKeepPoints&);  // Not implemented.
  void operator=(const vtkPolyDataKeepPoints&);  // Not implemented.

  std::set<vtkIdType> PointsToKeep;

  vtkSmartPointer<vtkIdTypeArray> KeepArray;
  
};

#endif
