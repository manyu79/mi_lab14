#--------------------------------------------------------
# The CMakeLists.txt for:                       pHazardPath
# Author(s):                              
#--------------------------------------------------------

FILE(GLOB SRC *.cpp)

ADD_EXECUTABLE(pHazardPath ${SRC})

TARGET_LINK_LIBRARIES(pHazardPath
   ${MOOS_LIBRARIES}
   mbutil
   m
   pthread)

