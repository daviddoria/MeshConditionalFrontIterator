#include "vtkPolyDataKeepPoints.h"

#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

#include "vtkPolyData.h"
#include "vtkSmartPointer.h"

#include <vtkExtractSelection.h>
#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkIdTypeArray.h>
#include <vtkDataSetSurfaceFilter.h>


//for testing only
#include <vtkXMLUnstructuredGridWriter.h>

vtkCxxRevisionMacro(vtkPolyDataKeepPoints, "$Revision: 1.70 $");
vtkStandardNewMacro(vtkPolyDataKeepPoints);

vtkPolyDataKeepPoints::vtkPolyDataKeepPoints()
{
  this->KeepArray =
    vtkSmartPointer<vtkIdTypeArray>::New();
  this->KeepArray->SetNumberOfComponents(1);
}

// FIX ME !!! This is currently not used even if it is set!
void vtkPolyDataKeepPoints::SetKeepArray(vtkSmartPointer<vtkIdTypeArray> arr)
{
  this->KeepArray->DeepCopy(arr);
}

void vtkPolyDataKeepPoints::AddPointToKeep(vtkIdType id)
{
  this->PointsToKeep.insert(id);
}

int vtkPolyDataKeepPoints::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  // get the input and ouptut
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkPolyData *filterInput = vtkPolyData::SafeDownCast(
      inInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkSmartPointer<vtkPolyData> input =
    vtkSmartPointer<vtkPolyData>::New();
  input->ShallowCopy(filterInput);
      
  vtkInformation* outInfo = outputVector->GetInformationObject(0);
  vtkPolyData* output = vtkPolyData::SafeDownCast(
		  outInfo->Get(vtkDataObject::DATA_OBJECT()));

  this->KeepArray->Reset();
  for(std::set<vtkIdType>::iterator it = this->PointsToKeep.begin(); it != this->PointsToKeep.end(); it++)
    {
    this->KeepArray->InsertNextValue(*it);
    }

  //std::cout << "Keeping " << this->KeepArray->GetNumberOfTuples() << " points." << std::endl;
  
  vtkSmartPointer<vtkSelectionNode> selectionNode =
      vtkSmartPointer<vtkSelectionNode>::New();
  selectionNode->SetFieldType(vtkSelectionNode::POINT);
  selectionNode->SetContentType(vtkSelectionNode::INDICES);
  selectionNode->SetSelectionList(this->KeepArray);
  //selectionNode->GetProperties()->Set(vtkSelectionNode::INVERSE(), 1); //invert the selection
  //selectionNode->GetProperties()->Set(vtkSelectionNode::CONTAINING_CELLS(), 1); //keep cells that use remaining points
  
  vtkSmartPointer<vtkSelection> selection =
      vtkSmartPointer<vtkSelection>::New();
  selection->AddNode(selectionNode);

  vtkSmartPointer<vtkExtractSelection> extractSelection =
      vtkSmartPointer<vtkExtractSelection>::New();
  extractSelection->SetInput(0, input);
  extractSelection->SetInput(1, selection);
  extractSelection->Update();

  vtkDataSet* ds = vtkDataSet::SafeDownCast (extractSelection->GetOutput());
  
  {
  vtkSmartPointer<vtkXMLUnstructuredGridWriter> writer =
    vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
  writer->SetFileName("ug.vtu");
  writer->SetInputConnection(ds->GetProducerPort());
  writer->Write();
  }
  
  //we want the output type to match the input type (vtkPolyData)
  vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter =
    vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
  surfaceFilter->SetInputConnection(ds->GetProducerPort());
  surfaceFilter->Update();
  
  output->ShallowCopy(surfaceFilter->GetOutput());
  
  return 1;
}
