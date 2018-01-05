ADD_LIBRARY(core INTERFACE)
TARGET_SOURCES(core INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/Assessment.h
  ${CMAKE_CURRENT_SOURCE_DIR}/BoundingBox.h
  ${CMAKE_CURRENT_SOURCE_DIR}/DocumentBoundingBox.h
  ${CMAKE_CURRENT_SOURCE_DIR}/DocumentBoundingBoxEventSet.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Event.h
  ${CMAKE_CURRENT_SOURCE_DIR}/EventSet.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Match.h
  ${CMAKE_CURRENT_SOURCE_DIR}/MatchError.h
  ${CMAKE_CURRENT_SOURCE_DIR}/MatchErrorCounts.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ScoredEvent.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ShapedEvent.h
  ${CMAKE_CURRENT_SOURCE_DIR}/TypeInfo.h)

IF(GTEST_FOUND AND GMOCK_FOUND AND WITH_TESTS)
  ADD_EXECUTABLE(AssessmentTest AssessmentTest.cc)
  TARGET_LINK_LIBRARIES(AssessmentTest
    core ${GMOCK_MAIN_LIBRARIES} ${COMMON_LIBRARIES})
  ADD_TEST(AssessmentTest AssessmentTest)

  ADD_EXECUTABLE(BoundingBoxTest BoundingBoxTest.cc)
  TARGET_LINK_LIBRARIES(BoundingBoxTest
    core ${GTEST_BOTH_LIBRARIES} ${COMMON_LIBRARIES})
  ADD_TEST(BoundingBoxTest BoundingBoxTest)

  ADD_EXECUTABLE(DocumentBoundingBoxTest DocumentBoundingBoxTest.cc)
  TARGET_LINK_LIBRARIES(DocumentBoundingBoxTest
    core ${GTEST_BOTH_LIBRARIES} ${COMMON_LIBRARIES})
  ADD_TEST(DocumentBoundingBoxTest DocumentBoundingBoxTest)

  ADD_EXECUTABLE(EventSetTest EventSetTest.cc)
  TARGET_LINK_LIBRARIES(EventSetTest
    core ${GTEST_BOTH_LIBRARIES} ${COMMON_LIBRARIES})
  ADD_TEST(EventSetTest EventSetTest)

  ADD_EXECUTABLE(EventTest EventTest.cc MockLocation.h)
  TARGET_LINK_LIBRARIES(EventTest
    core ${GMOCK_MAIN_LIBRARIES} ${COMMON_LIBRARIES})
  ADD_TEST(EventTest EventTest)

  ADD_EXECUTABLE(ShapedEventTest ShapedEventTest.cc MockLocation.h)
  TARGET_LINK_LIBRARIES(ShapedEventTest
    core ${GMOCK_MAIN_LIBRARIES} ${COMMON_LIBRARIES})
  ADD_TEST(ShapedEventTest ShapedEventTest)

  ADD_EXECUTABLE(DocumentBoundingBoxEventSetTest DocumentBoundingBoxEventSetTest.cc)
  TARGET_LINK_LIBRARIES(DocumentBoundingBoxEventSetTest
    core ${GTEST_BOTH_LIBRARIES} ${COMMON_LIBRARIES})
  ADD_TEST(DocumentBoundingBoxEventSetTest DocumentBoundingBoxEventSetTest)
ENDIF()
