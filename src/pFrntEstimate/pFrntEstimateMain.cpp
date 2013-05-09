
#include "FrntEstimate.h"
int main(int argc , char * argv[])
{
  const char * sMissionFile = "Mission.moos";

  if(argc > 1) {
    sMissionFile = argv[1];
  }
  
  CFrntEstimate MSim;
  
  MSim.Run("pFrntEstimate", sMissionFile);
  
  return 0;
}
