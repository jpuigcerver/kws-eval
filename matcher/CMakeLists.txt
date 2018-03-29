ADD_LIBRARY(matcher INTERFACE)
TARGET_SOURCES(matcher INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/Matcher.h
  ${CMAKE_CURRENT_SOURCE_DIR}/SimpleMatcher.h)

IF(GTEST_FOUND AND GMOCK_FOUND AND WITH_TESTS)
  ADD_EXECUTABLE(SimpleMatcherTest
    SimpleMatcherTest.cc SimpleMatcher.h Matcher.h
    ../scorer/MockScorer.h ../core/DummyLocation.h)
  TARGET_LINK_LIBRARIES(SimpleMatcherTest
    ${COMMON_LIBRARIES} ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})
  ADD_TEST(SimpleMatcherTest SimpleMatcherTest)
ENDIF()
