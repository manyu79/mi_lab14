#--------------------------------------------------------
# The CMakeLists.txt for:                  pFrontEstimate
# Author(s):                               Henrik Schmidt
#--------------------------------------------------------

FILE(GLOB SRC *.cpp)

ADD_EXECUTABLE(pFrntEstimate ${SRC})
   
TARGET_LINK_LIBRARIES(pFrntEstimate
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread
   henrik_util
)
