#ifndef __vtkMeshNormalConditionFrontIterator_h
#define __vtkMeshNormalConditionFrontIterator_h

#include "vtkMeshConditionalFrontIterator.h"

class vtkMeshNormalConditionFrontIterator : public vtkMeshConditionalFrontIterator
{
  public:
    static vtkMeshNormalConditionFrontIterator* New();
    vtkTypeMacro(vtkMeshNormalConditionFrontIterator, vtkMeshConditionalFrontIterator);
  
    vtkSetMacro(AcceptableAngle, float);
    
  protected:
    vtkMeshNormalConditionFrontIterator();
    ~vtkMeshNormalConditionFrontIterator() {}
    
    bool Condition(const int a, const int b);
    
  private:
    vtkMeshNormalConditionFrontIterator(const vtkMeshNormalConditionFrontIterator &);  // Not implemented.
    void operator=(const vtkMeshNormalConditionFrontIterator &);        // Not implemented.
    
    float AcceptableAngle; //specified in degrees
};

#endif
