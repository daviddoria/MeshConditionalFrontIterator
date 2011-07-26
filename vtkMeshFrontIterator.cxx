#include <vtkCell.h>
#include <vtkBitArray.h>
#include <vtkPolyData.h>
#include <vtkIdList.h>
#include <vtkObjectFactory.h>

#include "vtkMeshFrontIterator.h"
#include "Helpers.h"

#include <algorithm>

vtkStandardNewMacro(vtkMeshFrontIterator);

void vtkMeshFrontIterator::UniqueQueue::push_back(int val)
{
  vtkstd::deque<int>::iterator pos = vtkstd::find( this->q.begin(), this->q.end(), val);
  if ( pos == this->q.end())
    {
    this->q.push_back(val); 
    }
}

void vtkMeshFrontIterator::UniqueQueue::clear()
{
  this->q.clear();
}

void vtkMeshFrontIterator::UniqueQueue::pop_front()
{
  this->q.pop_front();
}

int vtkMeshFrontIterator::UniqueQueue::front()
{
  return this->q.front();
}

bool vtkMeshFrontIterator::UniqueQueue::empty()
{
  return this->q.empty();
}

vtkMeshFrontIterator::vtkMeshFrontIterator()
{
  this->Mesh = NULL;
  this->StartVertex = -1;
  this->VisitedArray = vtkSmartPointer<vtkBitArray>::New();
  this->VisitedArray->SetNumberOfComponents(1);
}


void vtkMeshFrontIterator::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "StartVertex: " << this->StartVertex << endl;
}

void vtkMeshFrontIterator::Initialize()
{
  this->NextId = -1;
  this->VertexQueue.clear();
  
  if (this->Mesh == NULL)
    {
    cout << "Error: Mesh is null" << endl;
    return;
    }
    
  if(this->StartVertex < 0 || this->StartVertex >= this->Mesh->GetNumberOfPoints())
    {
    cout << "Error: StartVertex is out of range" << endl;
    return;
    }
    
  //set all vertices to not visited
  this->VisitedArray->SetNumberOfValues(this->Mesh->GetNumberOfPoints());
  for(vtkIdType i = 0; i < this->VisitedArray->GetNumberOfTuples(); i++)
    {
    this->VisitedArray->SetValue(i, 0);
    }
  /*
  vtkSmartPointer<vtkIdList> connectedVertices = GetConnectedVertices(this->Mesh, this->StartVertex);
  
  if(connectedVertices->GetNumberOfIds() <= 0)
    {
    return;
    }
    
  cout << "Initializing" << endl;
  for(vtkIdType i = 0; i < connectedVertices->GetNumberOfIds(); i++)
    {
    cout << "Adding vertex " << connectedVertices->GetId(i) << " to the queue." << endl;
    this->VertexQueue.push_back(connectedVertices->GetId(i));
    }
  cout << endl;  
  
  this->VisitedArray->SetValue(this->StartVertex, 1);
    
  this->NextId = connectedVertices->GetId(0);
  */
    
  //this->NextId = this->StartVertex;
  this->VertexQueue.push_back(this->StartVertex);
  
  this->Modified();
}

void vtkMeshFrontIterator::SetMesh(vtkSmartPointer<vtkPolyData> mesh)
{
  this->Mesh = mesh;
  this->Modified();
}

void vtkMeshFrontIterator::SetStartVertex(vtkIdType vertex)
{
  this->StartVertex = vertex;
  this->Modified();
}

vtkIdType vtkMeshFrontIterator::Next()
{
  if(!this->HasNext())
    {
    this->NextId = -1;
    return -1;
    }
    
  if(this->VertexQueue.empty())
    {
    this->NextId = -1;
    return -1;
    }
    
  //get the next vertex from the queue
  this->NextId = this->VertexQueue.front();
  //cout << "Next vertex is " << this->NextId << endl;
  
  //mark this ID as visited
  this->VisitedArray->SetValue(this->NextId, 1);
  
  //remove this ID from the queue
  this->VertexQueue.pop_front();
    
  //add everything attached to this vertex to the queue (if it isn't already in the queue and it hasn't already been visited)
  vtkSmartPointer<vtkIdList> connectedVertices =
      vtkSmartPointer<vtkIdList>::New();
  GetConnectedVertices(this->Mesh, this->NextId, connectedVertices);
    
  for(vtkIdType i = 0; i < connectedVertices->GetNumberOfIds(); i++)
    {
    //cout << "Considering vertex " << connectedVertices->GetId(i) << endl;
    bool visited = this->VisitedArray->GetValue(connectedVertices->GetId(i));
    if(!visited)
      {
      //cout << "Adding vertex " << connectedVertices->GetId(i) << " to the queue." << endl;
      this->VertexQueue.push_back(connectedVertices->GetId(i));
      }
    else
      {
      //cout << "Vertex " << connectedVertices->GetId(i) << " has already been visited." << endl;
      }
    }
  
  cout << endl;
  
  return this->NextId;
}

bool vtkMeshFrontIterator::HasNext()
{
  if(this->VertexQueue.empty())
    {
    return false;
    }
  else
    {
    return true;
    }
}

#if 0
// assuming only edges are input ((all cells are vtkLines)
vtkSmartPointer<vtkIdList> GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, int id)
{
  vtkSmartPointer<vtkIdList> connectedVertices = 
      vtkSmartPointer<vtkIdList>::New();
  
  //get all cells that vertex 'id' is a part of
  vtkSmartPointer<vtkIdList> cellIdList = 
      vtkSmartPointer<vtkIdList>::New();
  mesh->GetPointCells(id, cellIdList);
  
  for(vtkIdType i = 0; i < cellIdList->GetNumberOfIds(); i++)
    {
    //cout << "cellId " << i << " : " << cellIdList->GetId(i) << endl;
    
    //get all points that are used by cells that vertex 'id' is a part of
    vtkSmartPointer<vtkIdList> pointIdList = 
      vtkSmartPointer<vtkIdList>::New();
    mesh->GetCellPoints(cellIdList->GetId(i), pointIdList);
    
    //cout << "End points are " << pointIdList->GetId(0) << " and " << pointIdList->GetId(1) << endl;
    
    
    if(pointIdList->GetId(0) != id)
      {
      //cout << "Connected to " << pointIdList->GetId(0) << endl;
      connectedVertices->InsertNextId(pointIdList->GetId(0));
      }
    else
      {
      //cout << "Connected to " << pointIdList->GetId(1) << endl;
      connectedVertices->InsertNextId(pointIdList->GetId(1));
      }
    }
    
    
  return connectedVertices;
}
#endif 
