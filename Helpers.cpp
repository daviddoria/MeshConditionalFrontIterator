#include "Helpers.h"

#include <vtkCell.h>

void GetConnectedVertices(vtkSmartPointer<vtkPolyData> mesh, int seed, vtkSmartPointer<vtkIdList> connectedVertices)
{
  
  //get all cells that vertex 'seed' is a part of
  vtkSmartPointer<vtkIdList> cellIdList = 
      vtkSmartPointer<vtkIdList>::New();
  mesh->GetPointCells(seed, cellIdList);
  
  //loop through all the cells that use the seed point
  for(vtkIdType i = 0; i < cellIdList->GetNumberOfIds(); i++)
    {

    vtkCell* cell = mesh->GetCell(cellIdList->GetId(i));
    //cout << "The cell has " << cell->GetNumberOfEdges() << " edges." << endl;
    
    for(vtkIdType e = 0; e < cell->GetNumberOfEdges(); e++)
      {
      vtkCell* edge = cell->GetEdge(e);

      vtkIdList* pointIdList = edge->GetPointIds();
      /*
      for(vtkIdType p = 0; p < pointIdList->GetNumberOfIds(); p++)
        {
        cout << "Edge " << i << " uses point " << pointIdList->GetId(p) << endl;
        }
        */
      if(pointIdList->GetId(0) == seed || pointIdList->GetId(1) == seed)
        {
        if(pointIdList->GetId(0) == seed)
          {
          connectedVertices->InsertNextId(pointIdList->GetId(1));
          }
        else
          {
          connectedVertices->InsertNextId(pointIdList->GetId(0));
          }
        }
      }
    

    }

}
