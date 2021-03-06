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
  std::vector<std::vector<std::vector<int> > > genTracklines(int swath);
  bool traverseTrackline(int); 
  double getDist(std::vector<int>, std::vector<double>); 

protected:
  int m_offset; 
  int m_start_offset; 
  int m_num_time_repeat; 

  int tl[2]; 
  int tr[2];
  int ll[2]; 
  int lr[2];  


};

#endif 
