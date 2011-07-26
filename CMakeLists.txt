cmake_minimum_required(VERSION 2.6)

PROJECT(MeshConditionalFrontIterator)

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

#### Executables ####
ADD_EXECUTABLE(vtkMeshFrontIteratorDemo vtkMeshFrontIteratorDemo.cxx vtkMeshFrontIterator.cxx Helpers.cpp)
target_link_libraries(vtkMeshFrontIteratorDemo ${ITK_LIBRARIES} ${VTK_LIBRARIES})

ADD_EXECUTABLE(vtkMeshConditionalFrontIteratorDemo vtkMeshConditionalFrontIteratorDemo.cxx vtkMeshConditionalFrontIterator.cxx vtkPolyDataKeepPoints.cxx Helpers.cpp)
target_link_libraries(vtkMeshConditionalFrontIteratorDemo ${ITK_LIBRARIES} ${VTK_LIBRARIES})

ADD_EXECUTABLE(vtkMeshNormalConditionFrontIteratorDemo vtkMeshNormalConditionFrontIteratorDemo.cxx vtkMeshNormalConditionFrontIterator.cxx vtkMeshConditionalFrontIterator.cxx vtkPolyDataKeepPoints.cxx Helpers.cpp)
target_link_libraries(vtkMeshNormalConditionFrontIteratorDemo ${ITK_LIBRARIES} ${VTK_LIBRARIES})

