#ifndef CONDITION_H
#define CONDITION_H

/*
 * This is an abstract class that defines the interface to perform a comparison between attributes of two vertices
 */

class vtkPolyData;

class Condition
{
public:
  Condition() {this->Mesh = 0;}
  
  // Compare the attributes of vertices a and b. Return true if they "pass the test".
  virtual bool IsSimilar(const int a, const int b) = 0;
  
  void SetMesh(vtkPolyData* m) {this->Mesh = m;}
  
protected:
  vtkPolyData* Mesh;
};

#endif
