/*****************************************************************/
/*    NAME: M.Benjamin, H.Schmidt, J. Leonard                    */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: BHV_SimpleWaypoint.ch                                */
/*    DATE: July 1st 2008  (For purposes of simple illustration) */
/*                                                               */
/* This program is free software; you can redistribute it and/or */
/* modify it under the terms of the GNU General Public License   */
/* as published by the Free Software Foundation; either version  */
/* 2 of the License, or (at your option) any later version.      */
/*                                                               */
/* This program is distributed in the hope that it will be       */
/* useful, but WITHOUT ANY WARRANTY; without even the implied    */
/* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR       */
/* PURPOSE. See the GNU General Public License for more details. */
/*                                                               */
/* You should have received a copy of the GNU General Public     */
/* License along with this program; if not, write to the Free    */
/* Software Foundation, Inc., 59 Temple Place - Suite 330,       */
/* Boston, MA 02111-1307, USA.                                   */
/*****************************************************************/
 
#ifndef BHV_FRONT_HEADER
#define BHV_FRONT_HEADER

#include <string>
#include "IvPBehavior.h"


class BHV_Front : public IvPBehavior {
public:
  BHV_Front(IvPDomain);
  ~BHV_Front() {};
  
  bool         setParam(std::string, std::string);
  void         onIdleState();
  IvPFunction* onRunState();

protected:
  void         updateSensorData();  
  IvPFunction* buildFunctionWithZAIC();
  bool         dx_delay(double, double *);
  void         keepFront(); 
  double       min_dhead(double, double); 

protected: // Configuration parameters
  double m_Kp; 
  double m_Kd; 

protected: // State variables
  double   m_speed;
  double   m_head;
  double   m_t_hot; 
  double   m_t_cold; 
  double   m_t_avg;  
  double   m_temp[2];
  double   m_old_pos[2]; 
  double   m_new_pos[2]; 
  bool     m_first_run; 
  double   m_dir[4];

  double   m_timer1; 
};

#ifdef WIN32s
	// Windows needs to explicitly specify functions to export from a dll
   #define IVP_EXPORT_FUNCTION __declspec(dllexport) 
#else
   #define IVP_EXPORT_FUNCTION
#endif

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_Front(domain);}
}
#endif









