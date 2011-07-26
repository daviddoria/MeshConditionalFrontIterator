// .NAME vtkMeshFrontIterator - depth first seedgeh iterator through a vtkGraph
//
// .SECTION Description
// vtkMeshFrontIterator traverses a mesh by constructing a queue of all edges that
// are attached to the seed point. It moves over each of these edges, adding all of 
// the edges that are attached to the end vertex to the queue.

// After setting up the iterator, the normal mode of operation is to
// set up a <code>while(iter->HasNext())</code> loop, with the statement
// <code>vtkIdType vertex = iter->Next()</code> inside the loop.


#ifndef __vtkMeshFrontIterator_h
#define __vtkMeshFrontIterator_h

#include "vtkObject.h"
#include "vtkSmartPointer.h"

#include <vtkstd/deque>

class vtkPolyData;
class vtkIdList;
class vtkBitArray;

class VTK_FILTERING_EXPORT vtkMeshFrontIterator : public vtkObject
{
  
public:
  static vtkMeshFrontIterator* New();
  vtkTypeMacro(vtkMeshFrontIterator, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set the mesh to iterate over.
  void SetMesh(vtkSmartPointer<vtkPolyData> mesh);

  // Description:
  // The start vertex of the iterator.
  void SetStartVertex(vtkIdType vertex);
  vtkGetMacro(StartVertex, vtkIdType);

  // Description:
  // The next vertex to visit.
  vtkIdType Next();

  // Description:
  // Return false when all vertices have been visited.
  bool HasNext();
  
  // Description:
  // Prepare the iterator.
  void Initialize();

protected:
  vtkMeshFrontIterator();
  ~vtkMeshFrontIterator() {}

private:
  class UniqueQueue
  {
    public:
      void push_back(int);
      void pop_front();
      int front();
      bool empty();
      void clear();
    private:
      vtkstd::deque<int> q;
  };
  
  vtkSmartPointer<vtkPolyData> Mesh;
  
  vtkIdType StartVertex;
  vtkIdType NextId;
  
  UniqueQueue VertexQueue;
  
  vtkSmartPointer<vtkBitArray> VisitedArray;
  
  vtkMeshFrontIterator(const vtkMeshFrontIterator &);  // Not implemented.
  void operator=(const vtkMeshFrontIterator &);        // Not implemented.
};


    
#endif
