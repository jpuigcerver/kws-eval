IF(GTEST_FOUND AND GMOCK_FOUND AND WITH_TESTS)

ADD_EXECUTABLE(ThresholdBoundingBoxScorerTest
        ThresholdBoundingBoxScorerTest.cc)
TARGET_LINK_LIBRARIES(ThresholdBoundingBoxScorerTest
  ${GMOCK_MAIN_LIBRARIES} ${COMMON_LIBRARIES})
ADD_TEST(ThresholdBoundingBoxScorerTest
        ThresholdBoundingBoxScorerTest)

ADD_EXECUTABLE(SoftBoundingBoxScorerTest
        SoftBoundingBoxScorerTest.cc)
TARGET_LINK_LIBRARIES(SoftBoundingBoxScorerTest
  ${GMOCK_MAIN_LIBRARIES} ${COMMON_LIBRARIES})
ADD_TEST(SoftBoundingBoxScorerTest
	SoftBoundingBoxScorerTest)

ENDIF()
