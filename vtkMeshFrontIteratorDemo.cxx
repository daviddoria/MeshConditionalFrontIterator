#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkUnsignedCharArray.h>
#include <vtkCommand.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkExtractEdges.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkLabeledDataMapper.h>
#include <vtkActor2D.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>

#include <sstream>

#include "vtkMeshFrontIterator.h"

// Define interaction style
class KeyPressInteractorStyle : public vtkInteractorStyleTrackballCamera
{
  public:
    static KeyPressInteractorStyle* New();
    vtkTypeMacro(KeyPressInteractorStyle, vtkInteractorStyleTrackballCamera);
 
    KeyPressInteractorStyle()
    {
      this->ColorArray = vtkSmartPointer<vtkUnsignedCharArray>::New();
      this->ColorArray->SetName("color");
      this->ColorArray->SetNumberOfComponents(3);
      
    }
    
    virtual void OnKeyPress() 
    {
      //get the keypress
      vtkRenderWindowInteractor *rwi = this->Interactor;
      std::string key = rwi->GetKeySym();
 
      //handle an arrow key
      if(key.compare("Right") == 0)
        {
        //cout << "The right arrow was pressed." << endl;
        if(this->Iterator->HasNext())
          {
          vtkIdType nextVertex = this->Iterator->Next();
  
          //cout << "Next vertex: " << nextVertex << endl << endl;
          unsigned char blue[3] = {0, 0, 255};
          vtkUnsignedCharArray::SafeDownCast(this->Mesh->GetPointData()->GetArray("color"))->SetTupleValue(nextVertex, blue);
          this->Mesh->Modified();
          this->GetInteractor()->GetRenderWindow()->Render();
  
          }
        }
  
      // forward events
      vtkInteractorStyleTrackballCamera::OnKeyPress();
    }
    
    
  void SetIterator(vtkSmartPointer<vtkMeshFrontIterator> iterator) {this->Iterator = iterator;}
  
  void SetMesh(vtkSmartPointer<vtkPolyData> mesh) 
  {
    this->Mesh = mesh; 
    
    unsigned char red[3] = {255,0,0};
    
    this->ColorArray->SetNumberOfTuples(this->Mesh->GetNumberOfPoints());
    
    for(vtkIdType i = 0; i < this->ColorArray->GetNumberOfTuples(); i++)
      {
      this->ColorArray->SetTupleValue(i, red);
      }
    this->Mesh->GetPointData()->SetScalars(this->ColorArray);
  }
  
  private:
    vtkSmartPointer<vtkMeshFrontIterator> Iterator;
    vtkSmartPointer<vtkPolyData> Mesh;
    vtkSmartPointer<vtkUnsignedCharArray> ColorArray;

 
};
//vtkCxxRevisionMacro(KeyPressInteractorStyle, "$Revision: 1.1 $");
vtkStandardNewMacro(KeyPressInteractorStyle);
 

class vtkTimerCallback : public vtkCommand
{
  public:
    static vtkTimerCallback *New()
    {
      vtkTimerCallback *cb = new vtkTimerCallback;
    
      return cb;
    }
 
    vtkTimerCallback()
    {
      this->ColorArray = vtkSmartPointer<vtkUnsignedCharArray>::New();
      this->ColorArray->SetName("color");
      this->ColorArray->SetNumberOfComponents(3);
      this->Counter = 0;
    }
    
    virtual void Execute(vtkObject *caller, unsigned long eventId,
                         void *callData)
    {
      if (eventId != vtkCommand::TimerEvent)
        {
        return;
        }
        
      if(this->Iterator->HasNext())
        {
        vtkIdType nextVertex = this->Iterator->Next();

        cout << "Next vertex: " << nextVertex << endl;
        unsigned char blue[3] = {0, 0, 255};
        
        vtkUnsignedCharArray::SafeDownCast(this->Mesh->GetPointData()->GetArray("color"))->SetTupleValue(nextVertex, blue);
        this->Mesh->Modified();
        this->Interactor->GetRenderWindow()->Render();

        //save the output
        vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = 
            vtkSmartPointer<vtkWindowToImageFilter>::New();
        windowToImageFilter->SetInput(this->Interactor->GetRenderWindow());
        //windowToImageFilter->SetMagnification(3); //set the resolution of the output image
        windowToImageFilter->Update();
        
        vtkSmartPointer<vtkPNGWriter> writer = 
            vtkSmartPointer<vtkPNGWriter>::New();
        std::stringstream ss;
        
        ss << std::setfill('0') << std::setw(2) << this->Counter << ".png";
        writer->SetFileName(ss.str().c_str());
        writer->SetInput(windowToImageFilter->GetOutput());
        writer->Write();
        
        this->Counter++;
        }

      }
    
  void SetIterator(vtkSmartPointer<vtkMeshFrontIterator> iterator) {this->Iterator = iterator;}
  void SetInteractor(vtkSmartPointer<vtkRenderWindowInteractor> interactor) {this->Interactor = interactor;}
  
  void SetMesh(vtkSmartPointer<vtkPolyData> mesh) 
  {
    this->Mesh = mesh; 
    
    unsigned char red[3] = {255,0,0};
    
    this->ColorArray->SetNumberOfTuples(this->Mesh->GetNumberOfPoints());
    
    for(vtkIdType i = 0; i < this->ColorArray->GetNumberOfTuples(); i++)
      {
      this->ColorArray->SetTupleValue(i, red);
      }
    this->Mesh->GetPointData()->SetScalars(this->ColorArray);
  }
  
  private:
    unsigned int Counter;
    vtkSmartPointer<vtkMeshFrontIterator> Iterator;
    vtkSmartPointer<vtkPolyData> Mesh;
    vtkSmartPointer<vtkUnsignedCharArray> ColorArray;
    vtkSmartPointer<vtkRenderWindowInteractor> Interactor;
};

int main (int argc, char *argv[])
{
  
  vtkSmartPointer<vtkPolyData> input =
    vtkSmartPointer<vtkPolyData>::New();
  
  // Use a sphere for a demo
  vtkSmartPointer<vtkSphereSource> sphereSource = 
      vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->Update();
  input->ShallowCopy(sphereSource->GetOutput());

  
  /*
  input->ShallowCopy(reader->GetOutput());
  
  
  vtkSmartPointer<vtkExtractEdges> extractEdges = 
      vtkSmartPointer<vtkExtractEdges>::New();
  extractEdges->SetInputConnection(input->GetProducerPort());
  extractEdges->Update();
  
  vtkSmartPointer<vtkPolyData> edges = 
      vtkSmartPointer<vtkPolyData>::New();
  edges->ShallowCopy(extractEdges->GetOutput());
    cout << "There are " << edges->GetNumberOfPoints() << " points." << endl;
  */

  
  vtkSmartPointer<vtkMeshFrontIterator> meshFrontIterator = 
      vtkSmartPointer<vtkMeshFrontIterator>::New();
  //meshFrontIterator->SetMesh(edges);
  meshFrontIterator->SetMesh(input);
  meshFrontIterator->SetStartVertex(12);
  meshFrontIterator->Initialize();
      
  //Create a mapper and actor
  vtkSmartPointer<vtkPolyDataMapper> mapper = 
      vtkSmartPointer<vtkPolyDataMapper>::New();
//  mapper->SetInputConnection(edges->GetProducerPort());
    mapper->SetInputConnection(input->GetProducerPort());
   
  vtkSmartPointer<vtkActor> actor = 
      vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
  actor->GetProperty()->SetLineWidth(10);
  
  //Create a renderer, render window, and interactor
  vtkSmartPointer<vtkRenderer> renderer = 
      vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow = 
      vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = 
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  /*
  //one step at a time using the interactor style
  vtkSmartPointer<KeyPressInteractorStyle> style = 
      vtkSmartPointer<KeyPressInteractorStyle>::New();
  style->SetIterator(meshFrontIterator);
  style->SetMesh(edges);
  renderWindowInteractor->SetInteractorStyle( style );
  */
  
  renderWindowInteractor->SetRenderWindow(renderWindow);
 
  //Add the actor to the scene
  renderer->AddActor(actor);
  //actor->GetProperty()->SetRepresentationToPoints();
  actor->GetProperty()->SetPointSize(6);
    
  // Animation using a timer
  vtkSmartPointer<vtkTimerCallback> cb = 
    vtkSmartPointer<vtkTimerCallback>::New();
  cb->SetIterator(meshFrontIterator);
//  cb->SetMesh(edges);
  cb->SetMesh(input);
  cb->SetInteractor(renderWindowInteractor);
  renderWindowInteractor->Initialize();
  int timerId = renderWindowInteractor->CreateRepeatingTimer(500);
  renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb);
  
  //Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();
  
  return EXIT_SUCCESS;
}
