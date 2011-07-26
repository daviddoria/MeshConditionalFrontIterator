// .NAME vtkMeshConditionalFrontIterator
//
// .SECTION Description
// vtkMeshConditionalFrontIterator traverses a mesh by constructing a queue of all edges that
// are attached to the seed point. It moves over each of these edges, adding all of 
// the edges that are attached to the end vertex to the queue if the pass the condition test.

// After setting up the iterator, the normal mode of operation is to
// set up a <code>while(iter->HasNext())</code> loop, with the statement
// <code>vtkIdType vertex = iter->Next()</code> inside the loop.

// The input to this filter must have normals and consist of only edges (i.e. first run
// vtkExtractEdges)

#ifndef __vtkMeshConditionalFrontIterator_h
#define __vtkMeshConditionalFrontIterator_h

#include "vtkObject.h"
#include "vtkSmartPointer.h"

#include "Condition.h"

#include <deque>
#include <vector>

class vtkPolyData;
class vtkIdList;
class vtkBitArray;

class vtkMeshConditionalFrontIterator : public vtkObject
{
  
public:
  static vtkMeshConditionalFrontIterator* New();
  vtkTypeMacro(vtkMeshConditionalFrontIterator, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set the mesh to iterate over.
  void SetMesh(vtkSmartPointer<vtkPolyData> mesh);

  // Description:
  // Extract the region which has been reached during the propagation.
  void GetSelectedRegion(vtkPolyData* mesh);
  
  // Description:
  // Mark the region which has been reached during the propagation by adding an array to the input mesh.
  void MarkSelectedRegion();
  
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

  // Description:
  // Set the condition to use.
  void SetCondition(Condition* c);
  
  // Description:
  // Set one of many conditions to use.
  void AddCondition(Condition* c);
  
protected:
  vtkMeshConditionalFrontIterator();
  ~vtkMeshConditionalFrontIterator();

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
  
  std::vector<Condition*> Conditions;
  
  vtkSmartPointer<vtkPolyData> Mesh;
  
  vtkIdType StartVertex;
  vtkIdType NextId;
  
  UniqueQueue VertexQueue;
  
  vtkSmartPointer<vtkBitArray> VisitedArray;
  
  vtkMeshConditionalFrontIterator(const vtkMeshConditionalFrontIterator &);  // Not implemented.
  void operator=(const vtkMeshConditionalFrontIterator &);        // Not implemented.
};

//helpers
//void GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, int id, vtkSmartPointer<vtkIdList> connectedVertices);
    
#endif
