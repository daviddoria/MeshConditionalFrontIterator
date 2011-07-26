cmake_minimum_required(VERSION 2.6)

PROJECT(vtkMeshConditionalFrontIterator)
ENABLE_TESTING()

INCLUDE_DIRECTORIES(/media/portable/VTK_Additions/)

FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

ADD_EXECUTABLE(vtkMeshConditionalFrontIteratorExample ../vtkMeshConditionalFrontIteratorExample.cxx
../vtkMeshConditionalFrontIterator.cxx
../vtkMeshFrontIterator.cxx)
TARGET_LINK_LIBRARIES(vtkMeshConditionalFrontIteratorExample vtkHybrid vtkInfovis)

ADD_EXECUTABLE(vtkMeshConditionalFrontIteratorDefaultExample ../vtkMeshConditionalFrontIteratorDefaultExample.cxx
../vtkMeshConditionalFrontIterator.cxx
../vtkMeshFrontIterator.cxx)
TARGET_LINK_LIBRARIES(vtkMeshConditionalFrontIteratorDefaultExample vtkHybrid vtkInfovis)

ADD_EXECUTABLE(vtkMeshConditionalFrontIteratorDemo ../vtkMeshConditionalFrontIteratorDemo.cxx
../vtkMeshConditionalFrontIterator.cxx
../vtkMeshFrontIterator.cxx
#/media/portable/VTK_Additions/vtkRiemannianGraphFilter.cxx
#/media/portable/VTK_Additions/vtkPointSetNormalEstimation.cxx
#/media/portable/VTK_Additions/vtkEuclideanMinimumSpanningTree.cxx
#/media/portable/VTK_Additions/vtkMutableDirectedGraphToMutableUndirectedGraph.cxx
#/media/portable/VTK_Additions/vtkFullyConnectedGraphFilter.cxx
#/media/portable/VTK_Additions/vtkEuclideanWeightUndirectedFilter.cxx
#/media/portable/VTK_Additions/vtkEuclideanWeightDirectedFilter.cxx
#/media/portable/VTK_Additions/vtkTreeToMutableDirectedGraph.cxx
#/media/portable/VTK_Additions/vtkMeshToGraph.cxx

)
TARGET_LINK_LIBRARIES(vtkMeshConditionalFrontIteratorDemo vtkHybrid vtkInfovis)

ADD_EXECUTABLE(vtkMeshConditionalFrontIteratorTest ../vtkMeshConditionalFrontIteratorTest.cxx
../vtkMeshConditionalFrontIterator.cxx
../vtkMeshFrontIterator.cxx)
TARGET_LINK_LIBRARIES(vtkMeshConditionalFrontIteratorTest vtkHybrid vtkInfovis)
ADD_TEST(vtkMeshConditionalFrontIteratorTest vtkMeshConditionalFrontIteratorTest)

