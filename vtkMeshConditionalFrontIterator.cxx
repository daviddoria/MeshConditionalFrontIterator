#include "vtkMeshConditionalFrontIterator.h"
#include "Helpers.h"
#include "vtkPolyDataKeepPoints.h"

#include <vtkBitArray.h>
#include <vtkIdList.h>
#include <vtkIntArray.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>

#include <algorithm>

vtkStandardNewMacro(vtkMeshConditionalFrontIterator);

void vtkMeshConditionalFrontIterator::GetSelectedRegion(vtkPolyData* selectedRegion)
{
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

void vtkMeshConditionalFrontIterator::MarkSelectedRegion()
{
  vtkSmartPointer<vtkIntArray> regionLabels = vtkSmartPointer<vtkIntArray>::New();
  regionLabels->SetNumberOfComponents(1);
  regionLabels->SetNumberOfValues(this->Mesh->GetNumberOfPoints());
  regionLabels->SetName("RegionLabels");
  
  // Mark all points as not belonging to the region
  for(unsigned int i = 0; i < this->Mesh->GetNumberOfPoints(); ++i)
    {
    regionLabels->SetValue(i, 0);
    }
  
  // Mark points that are reached as belonging to the region
  this->Initialize();
  
  while(this->HasNext())
    {
    vtkIdType nextVertex = this->Next();
    regionLabels->SetValue(nextVertex, 1);
    }
    
  this->Mesh->GetPointData()->AddArray(regionLabels);
}

void vtkMeshConditionalFrontIterator::SetCondition(Condition* c)
{
  this->Conditions.clear();
  AddCondition(c);
}

void vtkMeshConditionalFrontIterator::AddCondition(Condition* c)
{
  this->Conditions.push_back(c);
}

void vtkMeshConditionalFrontIterator::UniqueQueue::push_back(int val)
{
  vtkstd::deque<int>::iterator pos = std::find( this->q.begin(), this->q.end(), val);
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

vtkMeshConditionalFrontIterator::~vtkMeshConditionalFrontIterator()
{

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
  
  // If the condition has already been specified, forward the mesh to it.
  // If not, the user will manually have to give the mesh to the condition.
  for(unsigned int i = 0; i < this->Conditions.size(); ++i)
    {
    this->Conditions[i]->SetMesh(mesh);
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
    
  for(vtkIdType vertexID = 0; vertexID < connectedVertices->GetNumberOfIds(); vertexID++)
    {
    //cout << "Considering vertex " << connectedVertices->GetId(i) << endl;
    //cout << "VisitedArray length is " << this->VisitedArray->GetNumberOfTuples() << endl;

    bool visited = this->VisitedArray->GetValue(connectedVertices->GetId(vertexID));
    if(!visited)
      {
      //std::cout << "Inside testing condition..." << std::endl;
      bool pass = true;
    
      // Test all of the conditions
      for(unsigned int conditionID = 0; conditionID < this->Conditions.size(); ++conditionID)
	{
	if(!this->Conditions[conditionID]->IsSimilar(this->NextId, connectedVertices->GetId(vertexID)))
	  {
	  pass = false;
	  break;
	  }
	}
      if(pass)
	{
	//std::cout << "Adding vertex " << connectedVertices->GetId(i) << " to the queue." << std::endl;
	this->VertexQueue.push_back(connectedVertices->GetId(vertexID));
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
