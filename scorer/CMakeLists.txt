ADD_LIBRARY(scorer INTERFACE)
TARGET_SOURCES(scorer INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/IntersectionOverHypothesisAreaScorer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/IntersectionOverUnionAreaScorer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Scorer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/SoftBoundingBoxScorer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/TrivialScorer.h)

IF(GTEST_FOUND AND GMOCK_FOUND AND WITH_TESTS)
  ADD_EXECUTABLE(IntersectionOverHypothesisAreaScorerTest
    IntersectionOverHypothesisAreaScorerTest.cc
    IntersectionOverHypothesisAreaScorer.h Scorer.h ../core/MockEvent.h)
  TARGET_LINK_LIBRARIES(IntersectionOverHypothesisAreaScorerTest
    core ${COMMON_LIBRARIES} ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})
  ADD_TEST(IntersectionOverHypothesisAreaScorerTest
    IntersectionOverHypothesisAreaScorerTest)

  ADD_EXECUTABLE(SoftBoundingBoxScorerTest
    SoftBoundingBoxScorerTest.cc SoftBoundingBoxScorer.h Scorer.h
    ../core/MockEvent.h)
  TARGET_LINK_LIBRARIES(SoftBoundingBoxScorerTest
    core ${COMMON_LIBRARIES} ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})
  ADD_TEST(SoftBoundingBoxScorerTest
    SoftBoundingBoxScorerTest)

  ADD_EXECUTABLE(IntersectionOverUnionAreaScorerTest
    IntersectionOverUnionAreaScorerTest.cc IntersectionOverUnionAreaScorer.h
    Scorer.h ../core/MockEvent.h)
  TARGET_LINK_LIBRARIES(IntersectionOverUnionAreaScorerTest
    core ${COMMON_LIBRARIES} ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})
  ADD_TEST(IntersectionOverUnionAreaScorerTest
    IntersectionOverUnionAreaScorerTest)
ENDIF()
