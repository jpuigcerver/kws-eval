ADD_LIBRARY(matcher INTERFACE)
TARGET_SOURCES(matcher INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/Matcher.h
  ${CMAKE_CURRENT_SOURCE_DIR}/SimpleMatcher.h)

IF(GTEST_FOUND AND GMOCK_FOUND AND WITH_TESTS)
  ADD_EXECUTABLE(SimpleMatcherTest
    SimpleMatcherTest.cc SimpleMatcher.h Matcher.h
    ../scorer/MockScorer.h ../core/DummyLocation.h)
  TARGET_LINK_LIBRARIES(SimpleMatcherTest
    ${GMOCK_MAIN_LIBRARIES} ${COMMON_LIBRARIES})
  ADD_TEST(SimpleMatcherTest SimpleMatcherTest)
ENDIF()
