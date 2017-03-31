IF(GTEST_FOUND AND WITH_TESTS)

ADD_EXECUTABLE(BoundingBoxTest BoundingBoxTest.cc)
TARGET_LINK_LIBRARIES(BoundingBoxTest ${GTEST_BOTH_LIBRARIES})
ADD_TEST(BoundingBoxTest BoundingBoxTest)

ADD_EXECUTABLE(DocumentBoundingBoxTest DocumentBoundingBoxTest.cc)
TARGET_LINK_LIBRARIES(DocumentBoundingBoxTest ${GTEST_BOTH_LIBRARIES})
ADD_TEST(DocumentBoundingBoxTest DocumentBoundingBoxTest)

ENDIF()
