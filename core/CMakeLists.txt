IF(GTEST_FOUND AND GMOCK_FOUND AND WITH_TESTS)

ADD_EXECUTABLE(BoundingBoxTest BoundingBoxTest.cc)
TARGET_LINK_LIBRARIES(BoundingBoxTest
  ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
ADD_TEST(BoundingBoxTest BoundingBoxTest)

ADD_EXECUTABLE(DocumentBoundingBoxTest DocumentBoundingBoxTest.cc)
TARGET_LINK_LIBRARIES(DocumentBoundingBoxTest
  ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
ADD_TEST(DocumentBoundingBoxTest DocumentBoundingBoxTest)

ADD_EXECUTABLE(EventTest EventTest.cc)
TARGET_LINK_LIBRARIES(EventTest
  ${GMOCK_MAIN_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
ADD_TEST(EventTest EventTest)

ENDIF()