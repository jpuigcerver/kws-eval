IF(GTEST_FOUND AND GMOCK_FOUND AND WITH_TESTS)

ADD_EXECUTABLE(IntersectionOverHypothesisAreaScorerTest
	IntersectionOverHypothesisAreaScorerTest.cc)
TARGET_LINK_LIBRARIES(IntersectionOverHypothesisAreaScorerTest
  ${GMOCK_MAIN_LIBRARIES} ${COMMON_LIBRARIES})
ADD_TEST(IntersectionOverHypothesisAreaScorerTest
	IntersectionOverHypothesisAreaScorerTest)

ADD_EXECUTABLE(SoftBoundingBoxScorerTest
        SoftBoundingBoxScorerTest.cc)
TARGET_LINK_LIBRARIES(SoftBoundingBoxScorerTest
  ${GMOCK_MAIN_LIBRARIES} ${COMMON_LIBRARIES})
ADD_TEST(SoftBoundingBoxScorerTest
	SoftBoundingBoxScorerTest)

ADD_EXECUTABLE(IntersectionOverUnionAreaScorerTest
	IntersectionOverUnionAreaScorerTest.cc)
TARGET_LINK_LIBRARIES(IntersectionOverUnionAreaScorerTest
  ${GMOCK_MAIN_LIBRARIES} ${COMMON_LIBRARIES})
ADD_TEST(IntersectionOverUnionAreaScorerTest
	IntersectionOverUnionAreaScorerTest)

ENDIF()
