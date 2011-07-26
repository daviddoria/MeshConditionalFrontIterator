#include "vtkMeshConditionalFrontIterator.h"
#include "vtkMeshFrontIterator.h" // for GetConnectedVertices only

#include "vtkPolyDataKeepPoints.h"

#include "vtkBitArray.h"
#include "vtkPolyData.h"
#include "vtkIdList.h"
#include "vtkObjectFactory.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"


#include <vtkstd/algorithm>

vtkCxxRevisionMacro(vtkMeshConditionalFrontIterator, "$Revision$");
vtkStandardNewMacro(vtkMeshConditionalFrontIterator);

void vtkMeshConditionalFrontIterator::GetSelectedRegion(vtkSmartPointer<vtkPolyData> selectedRegion)
{
  std::cout << "this->Mesh has " << this->Mesh->GetNumberOfPoints() << " points." << std::endl;
  
  this->Initialize();
  
  vtkSmartPointer<vtkPolyDataKeepPoints> keepPoints =
    vtkSmartPointer<vtkPolyDataKeepPoints>::New();
  keepPoints->SetInputConnection(this->Mesh->GetProducerPort());
  
  while(this->HasNext())
    {
    vtkIdType nextVertex = this->Next();
    keepPoints->AddPointToKeep(nextVertex);
    }
    
  keepPoints->Update();
  
  selectedRegion->ShallowCopy(keepPoints->GetOutput());
}

bool vtkMeshConditionalFrontIterator::Condition(const int a, const int b)
{
  //In this class, the condition is always true. Subclasses can provide
  //real condition functions
  return true;
}

void vtkMeshConditionalFrontIterator::UniqueQueue::push_back(int val)
{
  vtkstd::deque<int>::iterator pos = vtkstd::find( this->q.begin(), this->q.end(), val);
  if ( pos == this->q.end())
    {
    this->q.push_back(val); 
    }
}

void vtkMeshConditionalFrontIterator::UniqueQueue::clear()
{
  this->q.clear();
}

void vtkMeshConditionalFrontIterator::UniqueQueue::pop_front()
{
  this->q.pop_front();
}

int vtkMeshConditionalFrontIterator::UniqueQueue::front()
{
  return this->q.front();
}

bool vtkMeshConditionalFrontIterator::UniqueQueue::empty()
{
  return this->q.empty();
}

vtkMeshConditionalFrontIterator::vtkMeshConditionalFrontIterator()
{
  this->Mesh = NULL;
  this->StartVertex = -1;
  this->VisitedArray = vtkSmartPointer<vtkBitArray>::New();
  this->VisitedArray->SetNumberOfComponents(1);
}


void vtkMeshConditionalFrontIterator::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "StartVertex: " << this->StartVertex << endl;
}

void vtkMeshConditionalFrontIterator::Initialize()
{
  this->NextId = -1;
  this->VertexQueue.clear();
  
  if (this->Mesh == NULL)
    {
    std::cout << "Error: Mesh is null" << std::endl;
    return;
    }
    
  if(this->StartVertex < 0 || this->StartVertex >= this->Mesh->GetNumberOfPoints())
    {
    std::cout << "Error: StartVertex is out of range" << std::endl;
    return;
    }
    
  //set all vertices to not visited
  this->VisitedArray->SetNumberOfValues(this->Mesh->GetNumberOfPoints());
  for(vtkIdType i = 0; i < this->VisitedArray->GetNumberOfTuples(); i++)
    {
    this->VisitedArray->SetValue(i, 0);
    }

  this->VertexQueue.push_back(this->StartVertex);
  
  this->Modified();
}

void vtkMeshConditionalFrontIterator::SetMesh(vtkSmartPointer<vtkPolyData> mesh)
{
  this->Mesh = mesh;
  //this->VisitedArray->SetNumberOfValues(this->Mesh->GetNumberOfPoints());
  
  //check for normals
  vtkSmartPointer<vtkFloatArray> normals = 
    vtkFloatArray::SafeDownCast(this->Mesh->GetPointData()->GetNormals());
  if(!normals)
    { 
    cout << "Error: No point normals, can't evaluate condition." << endl;
    exit(-1);
    }
  
  this->Modified();
}

void vtkMeshConditionalFrontIterator::SetStartVertex(vtkIdType vertex)
{
  this->StartVertex = vertex;
  this->Modified();
}

vtkIdType vtkMeshConditionalFrontIterator::Next()
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
  //void GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, int seed, vtkSmartPointer<vtkIdList> ids);
    
  //cout << "There are " << connectedVertices->GetNumberOfIds() << " connected vertices" << endl;
  
  if(connectedVertices->GetNumberOfIds() <= 0)
    {
    return this->NextId;
    }
    
  for(vtkIdType i = 0; i < connectedVertices->GetNumberOfIds(); i++)
    {
    //cout << "Considering vertex " << connectedVertices->GetId(i) << endl;
    //cout << "VisitedArray length is " << this->VisitedArray->GetNumberOfTuples() << endl;

    bool visited = this->VisitedArray->GetValue(connectedVertices->GetId(i));
    if(!visited)
      {
      //std::cout << "Inside testing condition..." << std::endl;
      if(this->Condition(this->NextId, connectedVertices->GetId(i)))
        {
        //std::cout << "Adding vertex " << connectedVertices->GetId(i) << " to the queue." << std::endl;
        this->VertexQueue.push_back(connectedVertices->GetId(i));
        }
      else
        {
        //std::cout << "Failed condition test." << std::endl;
        }
      }
    else
      {
      //std::cout << "Vertex " << connectedVertices->GetId(i) << " has already been visited." << std::endl;
      }
    }
  
  //std::cout << std::endl;
  
  return this->NextId;
}

bool vtkMeshConditionalFrontIterator::HasNext()
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

/*
void GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, int id, vtkSmartPointer<vtkIdList> connectedVertices)
{
    
  //get all cells that vertex 'id' is a part of
  vtkSmartPointer<vtkIdList> cellIdList = 
      vtkSmartPointer<vtkIdList>::New();
  mesh->GetPointCells(id, cellIdList);
  
  for(vtkIdType i = 0; i < cellIdList->GetNumberOfIds(); i++)
    {
    //cout << "cellId " << i << " : " << cellIdList->GetId(i) << endl;
    
    vtkSmartPointer<vtkIdList> pointIdList = 
      vtkSmartPointer<vtkIdList>::New();
    mesh->GetCellPoints(cellIdList->GetId(i), pointIdList);
    
    if(pointIdList->GetNumberOfIds() != 2)
      {
      cout << "The cell is not a vtkLine! Cannot continue!" << endl;
      exit(-1);
      }
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
    
}
*/
