/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: HazardPath.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef HazardPath_HEADER
#define HazardPath_HEADER

#include "MOOS/libMOOS/MOOSLib.h"

class HazardPath : public CMOOSApp
{
 public:
  HazardPath();
  virtual ~HazardPath();

  bool OnNewMail(MOOSMSG_LIST &NewMail);
  bool Iterate();
  bool OnConnectToServer();
  bool OnStartUp();
  void RegisterVariables();
  void genLawnMower(int, int); 
  std::string intToString(int); 

protected:
  int m_offset; 
  int m_start_offset; 
};

#endif 
