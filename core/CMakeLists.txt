IF(GTEST_FOUND AND GMOCK_FOUND AND WITH_TESTS)

ADD_EXECUTABLE(AssessmentTest AssessmentTest.cc)
TARGET_LINK_LIBRARIES(AssessmentTest
  ${GMOCK_MAIN_LIBRARIES} ${COMMON_LIBRARIES})
ADD_TEST(AssessmentTest AssessmentTest)

ADD_EXECUTABLE(BoundingBoxTest BoundingBoxTest.cc)
TARGET_LINK_LIBRARIES(BoundingBoxTest
  ${GTEST_BOTH_LIBRARIES} ${COMMON_LIBRARIES})
ADD_TEST(BoundingBoxTest BoundingBoxTest)

ADD_EXECUTABLE(DocumentBoundingBoxTest DocumentBoundingBoxTest.cc)
TARGET_LINK_LIBRARIES(DocumentBoundingBoxTest
  ${GTEST_BOTH_LIBRARIES} ${COMMON_LIBRARIES})
ADD_TEST(DocumentBoundingBoxTest DocumentBoundingBoxTest)

ADD_EXECUTABLE(EventTest EventTest.cc)
TARGET_LINK_LIBRARIES(EventTest
  ${GMOCK_MAIN_LIBRARIES} ${COMMON_LIBRARIES})
ADD_TEST(EventTest EventTest)

ADD_EXECUTABLE(DocumentBoundingBoxEventSetTest DocumentBoundingBoxEventSetTest.cc)
TARGET_LINK_LIBRARIES(DocumentBoundingBoxEventSetTest
        ${GTEST_BOTH_LIBRARIES} ${COMMON_LIBRARIES})
ADD_TEST(DocumentBoundingBoxEventSetTest DocumentBoundingBoxEventSetTest)

ADD_EXECUTABLE(EventSetTest EventSetTest.cc)
TARGET_LINK_LIBRARIES(EventSetTest
        ${GTEST_BOTH_LIBRARIES} ${COMMON_LIBRARIES})
ADD_TEST(EventSetTest EventSetTest)

ENDIF()
