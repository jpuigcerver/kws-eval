ADD_EXECUTABLE(Icdar17KwsEval Icdar17KwsEval.cc GenericKwsEvalTool.h)
TARGET_LINK_LIBRARIES(Icdar17KwsEval
  cmd core reader scorer matcher ${COMMON_LIBRARIES})

ADD_EXECUTABLE(SimpleKwsEval SimpleKwsEval.cc GenericKwsEvalTool.h)
TARGET_LINK_LIBRARIES(SimpleKwsEval
  cmd core reader scorer mapper matcher ${COMMON_LIBRARIES})

INSTALL(
  TARGETS Icdar17KwsEval SimpleKwsEval
  RUNTIME DESTINATION bin)
