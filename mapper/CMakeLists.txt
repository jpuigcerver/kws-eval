ADD_LIBRARY(mapper INTERFACE)
TARGET_SOURCES(mapper INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/IdentityMapper.h
  ${CMAKE_CURRENT_SOURCE_DIR}/Mapper.h
  ${CMAKE_CURRENT_SOURCE_DIR}/ScoredEventMapper.h
  ${CMAKE_CURRENT_SOURCE_DIR}/StringEventToIntMapper.h
  ${CMAKE_CURRENT_SOURCE_DIR}/StringToIntMapper.h)

IF(GTEST_FOUND AND GMOCK_FOUND AND WITH_TESTS)
  ADD_EXECUTABLE(ScoredEventMapperTest
    ScoredEventMapperTest.cc ScoredEventMapper.h Mapper.h)
  TARGET_LINK_LIBRARIES(ScoredEventMapperTest
    ${GTEST_LIBRARIES} ${GMOCK_BOTH_LIBRARIES} ${COMMON_LIBRARIES})
  ADD_TEST(ScoredEventMapperTest ScoredEventMapperTest)

  ADD_EXECUTABLE(StringEventToIntMapperTest
    StringEventToIntMapperTest.cc StringEventToIntMapper.h Mapper.h)
  TARGET_LINK_LIBRARIES(StringEventToIntMapperTest
    ${GTEST_BOTH_LIBRARIES} ${COMMON_LIBRARIES})
  ADD_TEST(StringEventToIntMapperTest StringEventToIntMapperTest)

  ADD_EXECUTABLE(StringToIntMapperTest
    StringToIntMapperTest.cc StringToIntMapper.h Mapper.h)
  TARGET_LINK_LIBRARIES(StringToIntMapperTest
    ${GTEST_BOTH_LIBRARIES} ${COMMON_LIBRARIES})
  ADD_TEST(StringToIntMapperTest StringToIntMapperTest)
ENDIF()
