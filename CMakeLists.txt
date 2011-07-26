cmake_minimum_required(VERSION 2.6)

PROJECT(MeshConditionalFrontIterator)

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

#### Executables ####
ADD_EXECUTABLE(vtkMeshFrontIteratorDemo vtkMeshFrontIteratorDemo.cxx vtkMeshFrontIterator.cxx Helpers.cpp)
target_link_libraries(vtkMeshFrontIteratorDemo ${ITK_LIBRARIES} ${VTK_LIBRARIES})

# ADD_EXECUTABLE(vtkMeshConditionalFrontIteratorDemo 
# vtkMeshConditionalFrontIteratorDemo.cxx 
# vtkMeshConditionalFrontIterator.cxx 
# vtkPolyDataKeepPoints.cxx 
# Helpers.cpp
# NormalCondition.cpp)
# target_link_libraries(vtkMeshConditionalFrontIteratorDemo ${ITK_LIBRARIES} ${VTK_LIBRARIES})

ADD_EXECUTABLE(vtkMeshConditionalFrontIteratorDefaultExample
vtkMeshConditionalFrontIteratorDefaultExample.cxx 
vtkMeshConditionalFrontIterator.cxx 
vtkPolyDataKeepPoints.cxx 
Helpers.cpp)
target_link_libraries(vtkMeshConditionalFrontIteratorDefaultExample ${ITK_LIBRARIES} ${VTK_LIBRARIES})

ADD_EXECUTABLE(vtkMeshConditionalFrontIteratorNormalExample
vtkMeshConditionalFrontIteratorNormalExample.cxx 
vtkMeshConditionalFrontIterator.cxx 
vtkPolyDataKeepPoints.cxx 
Helpers.cpp
NormalCondition.cpp)
target_link_libraries(vtkMeshConditionalFrontIteratorNormalExample ${ITK_LIBRARIES} ${VTK_LIBRARIES})


ADD_EXECUTABLE(vtkMeshConditionalFrontIteratorColorExample
vtkMeshConditionalFrontIteratorColorExample.cxx 
vtkMeshConditionalFrontIterator.cxx 
vtkPolyDataKeepPoints.cxx 
Helpers.cpp
ColorCondition.cpp)
target_link_libraries(vtkMeshConditionalFrontIteratorColorExample ${ITK_LIBRARIES} ${VTK_LIBRARIES})



ADD_EXECUTABLE(vtkMeshConditionalFrontIteratorDistanceExample
vtkMeshConditionalFrontIteratorDistanceExample.cxx 
vtkMeshConditionalFrontIterator.cxx 
vtkPolyDataKeepPoints.cxx 
Helpers.cpp
DistanceCondition.cpp)
target_link_libraries(vtkMeshConditionalFrontIteratorDistanceExample ${ITK_LIBRARIES} ${VTK_LIBRARIES})
