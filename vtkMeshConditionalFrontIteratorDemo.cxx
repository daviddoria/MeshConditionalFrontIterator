#include <vtkSmartPointer.h>
#include <vtkCellData.h>
#include <vtkPointSet.h>
#include <vtkIdFilter.h>
#include <vtkProperty.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkExtractGeometry.h>
#include <vtkIdTypeArray.h>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkPlanes.h>
#include <vtkAreaPicker.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkMath.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkExtractEdges.h>
#include <vtkObjectFactory.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>
#include <vtkPolyDataNormals.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkGraphToPolyData.h>
#include <vtkVertexGlyphFilter.h>

#include "vtkMeshConditionalFrontIterator.h"

// Define interaction style
class InteractorStyle : public vtkInteractorStyleRubberBandPick
{
  public:
    static InteractorStyle* New();
    vtkTypeRevisionMacro(InteractorStyle,vtkInteractorStyleRubberBandPick);
 
    InteractorStyle()
    {
      this->SeedPolyData = vtkSmartPointer<vtkPolyData>::New();
      this->SeedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
      this->SeedActor = vtkSmartPointer<vtkActor>::New();
        
      this->Mesh = vtkSmartPointer<vtkPolyData>::New();
    
      this->SelectedPoints = vtkSmartPointer<vtkPoints>::New();
      this->SelectedPolyData = vtkSmartPointer<vtkPolyData>::New();
      this->SelectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
      this->SelectedActor = vtkSmartPointer<vtkActor>::New(); 
    }
    
    virtual void OnLeftButtonUp();
    
    void SetMesh(vtkSmartPointer<vtkPolyData> mesh)
    {
      this->Mesh = mesh;
    }
    
  private:
    
    unsigned int SeedId;
    vtkSmartPointer<vtkPolyData> SeedPolyData;
    vtkSmartPointer<vtkDataSetMapper> SeedMapper;
    vtkSmartPointer<vtkActor> SeedActor;
        
    vtkSmartPointer<vtkPolyData> Mesh;
    
    vtkSmartPointer<vtkPoints> SelectedPoints;
    vtkSmartPointer<vtkPolyData> SelectedPolyData;
    vtkSmartPointer<vtkDataSetMapper> SelectedMapper;
    vtkSmartPointer<vtkActor> SelectedActor;

};
vtkCxxRevisionMacro(InteractorStyle, "$Revision: 1.1 $");
vtkStandardNewMacro(InteractorStyle);



class vtkMeshNormalConditionFrontIterator : public vtkMeshConditionalFrontIterator
{
  public:
    static vtkMeshNormalConditionFrontIterator* New();
    vtkTypeRevisionMacro(vtkMeshNormalConditionFrontIterator, vtkObject);
  
  private:
    bool Condition(const int a, const int b);
    
};
vtkCxxRevisionMacro(vtkMeshNormalConditionFrontIterator, "$Revision$");
vtkStandardNewMacro(vtkMeshNormalConditionFrontIterator);

bool vtkMeshNormalConditionFrontIterator::Condition(const int a, const int b)
{
  //cout << "Testing condition..." << endl;
  
  //get normals
  vtkSmartPointer<vtkFloatArray> normals = 
    //  vtkDoubleArray::SafeDownCast(this->Mesh->GetPointData()->GetArray("Normals"));
    vtkFloatArray::SafeDownCast(this->Mesh->GetPointData()->GetNormals());
  
  /*
  if(normals)
    { 
    cout << "There are " << normals->GetNumberOfTuples() << 
      " point normals." << endl;
    }
  else
    {
    cout << "Error: No point normals, can't evaluate condition." << endl;
    return false;
    }
  */
  double n1[3];
  normals->GetTuple(a, n1);
  vtkMath::Normalize(n1);
  //cout << "Point normal " << i << ": " << n[0] << " " << n[1] << " " << n[2] << endl;
  
  double n2[3];
  normals->GetTuple(b, n2);
  vtkMath::Normalize(n2);

  double angle = acos(vtkMath::Dot(n1,n2));
  
  if(angle > vtkMath::RadiansFromDegrees(5.0))
    {
    //cout << "normals vary by " << vtkMath::DegreesFromRadians(angle) << endl;
    return false;
    }
  else
    {
    return true;
    }
}
    
int main (int argc, char *argv[])
{
  
  //ensure a filename was specified
  if ( argc != 2 )
    {
    cout << "Required arguments: InputFilename" << endl;
    return EXIT_FAILURE;
    }
 
  //get the filename from the command line
  vtkstd::string inputFilename = argv[1];
  
  vtkSmartPointer<vtkXMLPolyDataReader> reader = 
      vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader->SetFileName(inputFilename.c_str());
  reader->Update();
  
  vtkSmartPointer<vtkPolyData> input = 
      vtkSmartPointer<vtkPolyData>::New();
  input->ShallowCopy(reader->GetOutput());
  //input->GetPointData()->SetActiveScalars(NULL);
  input->GetCellData()->SetActiveScalars(NULL);
  
  vtkSmartPointer<vtkIdFilter> idFilter = 
      vtkSmartPointer<vtkIdFilter>::New();
  idFilter->SetInputConnection(input->GetProducerPort());
  idFilter->Update();
  
  //create a mapper and actor
  vtkSmartPointer<vtkPolyDataMapper> mapper = 
      vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(input->GetProducerPort());
 
  vtkSmartPointer<vtkActor> actor = 
      vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  //actor->GetProperty()->SetColor(1.0, 0.0, 0.0); //(R,G,B)
  
  // a renderer and render window
  vtkSmartPointer<vtkRenderer> renderer = 
      vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow = 
      vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
 
  // an interactor
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = 
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
 
  // add the actors to the scene
  renderer->AddActor(actor);
  renderer->SetBackground(1,1,1); // Background color white
  renderWindow->Render();
  renderWindow->SetSize(800,800);
  
  vtkSmartPointer<InteractorStyle> style = 
      vtkSmartPointer<InteractorStyle>::New();
  style->SetMesh(vtkPolyData::SafeDownCast(idFilter->GetOutput()));
  
  renderWindowInteractor->SetInteractorStyle( style );
 
  vtkSmartPointer<vtkAreaPicker> areaPicker = 
    vtkSmartPointer<vtkAreaPicker>::New();
  
  renderWindowInteractor->SetPicker(areaPicker);
  
  // begin mouse interaction
  renderWindowInteractor->Start();
    
  return EXIT_SUCCESS;
}


void InteractorStyle::OnLeftButtonUp() 
{
  
  // forward events
  vtkInteractorStyleRubberBandPick::OnLeftButtonUp();
  
  //if we are not in selection/rubber band mode, don't do anything
  if(this->CurrentMode == 0)
    {
    return;
    }
  
    cout << "Computing connected region..." << endl;
  
    
  //cout << "There are " << this->Points->GetNumberOfPoints() << " points." << endl;
  
  //get the selection that just happened
  vtkPlanes* frustum = static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

    
  cout << "Selecting from " << this->Mesh->GetNumberOfPoints() << " points." << endl;
  //cout << "Selecting from " << this->Mesh->GetNumberOfCells() << " cells." << endl;
    
  vtkSmartPointer<vtkExtractGeometry> extractGeometry = 
      vtkSmartPointer<vtkExtractGeometry>::New();
  extractGeometry->SetImplicitFunction(frustum);
  extractGeometry->SetInput(this->Mesh);
  extractGeometry->Update();
  
  vtkSmartPointer<vtkIdTypeArray> ids = 
      vtkIdTypeArray::SafeDownCast(extractGeometry->GetOutput()->GetPointData()->GetArray("vtkIdFilter_Ids"));
  
  if(!ids)
    {
    cout << "Nothing selected (or data set does not have vtkIdFilters_Ids array, run vtkIdFilter first)!" << endl;
    return;
    }
    
  if(ids->GetNumberOfTuples() <= 0)
    {
    cout << "There are " << ids->GetNumberOfTuples() << " ids." << endl;
    cout << "Nothing selected (or data set does not have vtkIdFilters_Ids array, run vtkIdFilter first)!" << endl;
    this->SeedMapper->SetInputConnection(NULL);
    this->SelectedMapper->SetInputConnection(NULL);
    this->GetInteractor()->GetRenderWindow()->Render();
    return;
    }
    
  //the first id in the selection is chosen to be the seed
  this->SeedId = ids->GetValue(0);
  
  //create an object with this seed point
  vtkSmartPointer<vtkPoints> seedPoint = 
      vtkSmartPointer<vtkPoints>::New();
  double p[3];
  this->Mesh->GetPoint(this->SeedId, p);
  seedPoint->InsertNextPoint(p);
  this->SeedPolyData->SetPoints(seedPoint);
  
  vtkSmartPointer<vtkVertexGlyphFilter> seedVertexFilter = 
      vtkSmartPointer<vtkVertexGlyphFilter>::New();
  seedVertexFilter->SetInputConnection(this->SeedPolyData->GetProducerPort());
  seedVertexFilter->Update();
  
  //this->SeedMapper->SetInputConnection(this->SeedPolyData->GetProducerPort());
  this->SeedMapper->SetInputConnection(seedVertexFilter->GetOutputPort());
  this->SeedActor->SetMapper(this->SeedMapper);
  this->SeedActor->GetProperty()->SetColor(1.0, 0.0, 0.0);
  //this->SeedActor->GetProperty()->SetLineWidth(3.0);
  this->SeedActor->GetProperty()->SetPointSize(10.0);
  
  this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(this->SeedActor);
  
  this->GetInteractor()->GetRenderWindow()->Render();
  this->HighlightProp(NULL);

    
//  cout << "There are " << extractEdges->GetOutput()->GetNumberOfPoints() << " points." << endl;
  this->SelectedPoints->SetNumberOfPoints(0);
  
  if(this->SeedId >= 0)
    {

    vtkSmartPointer<vtkMeshNormalConditionFrontIterator> iterator = 
        vtkSmartPointer<vtkMeshNormalConditionFrontIterator>::New();
    iterator->SetMesh(this->Mesh);
    //iterator->SetMesh(vtkPolyData::SafeDownCast(this->Points));
    iterator->SetStartVertex(this->SeedId);
    iterator->Initialize();
    
    while(iterator->HasNext())
      {
      vtkIdType nextVertex = iterator->Next();
      this->SelectedPoints->InsertNextPoint(this->Mesh->GetPoint(nextVertex));
      //cout << "Next vertex: " << nextVertex << endl;
      }
    }
  
  this->SelectedPolyData->SetPoints(this->SelectedPoints);
  cout << "Selected " << this->SelectedPolyData->GetNumberOfPoints() << " points." << endl;
  
  vtkSmartPointer<vtkVertexGlyphFilter> selectedVertexFilter = 
      vtkSmartPointer<vtkVertexGlyphFilter>::New();
  selectedVertexFilter->SetInputConnection(this->SelectedPolyData->GetProducerPort());
  selectedVertexFilter->Update();
    
  //this->SelectedMapper->SetInputConnection(this->SelectedPolyData->GetProducerPort());
  this->SelectedMapper->SetInputConnection(selectedVertexFilter->GetOutputPort());
  this->SelectedActor->SetMapper(this->SelectedMapper);
  this->SelectedActor->GetProperty()->SetColor(0.0, 1.0, 0.0);
  //this->SelectedActor->GetProperty()->SetLineWidth(7.0);
  this->SelectedActor->GetProperty()->SetPointSize(7.0);
  
  this->GetInteractor()->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(this->SelectedActor);
  
  this->GetInteractor()->GetRenderWindow()->Render();
  this->HighlightProp(NULL);
  
}
