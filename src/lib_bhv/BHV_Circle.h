/*****************************************************************/
/*    NAME: M.Benjamin, H.Schmidt, J. Leonard                    */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: BHV_Circle.ch                                */
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
 
#ifndef BHV_CIRCLE_HEADER
#define BHV_CIRCLE_HEADER

#include <string>
#include "IvPBehavior.h"
#include "XYPoint.h"

class BHV_Circle : public IvPBehavior {
public:
  BHV_Circle(IvPDomain);
  ~BHV_Circle() {};
  
  bool         setParam(std::string, std::string);
  void         onIdleState();
  IvPFunction* onRunState();

protected:
  void         postPulse(bool viewable=true);
  void         updateState();
  IvPFunction* buildCourseWithZAIC(double ang);

protected: // Configuration parameters
  double  m_radius;
  double  m_arc;
  double  m_priority_wt;
  bool    m_send;
  double  m_raderr;
  double  m_angerr;

protected: // State variables
  double   m_osx;
  double   m_osy;
  double   m_oshead;
  double   m_now;
  double   m_cenx;
  double   m_ceny;
  double   m_startang;
  double   m_presc_ang;
  double   m_range;
  double   m_ang;

};

#ifdef WIN32
	// Windows needs to explicitly specify functions to export from a dll
   #define IVP_EXPORT_FUNCTION __declspec(dllexport) 
#else
   #define IVP_EXPORT_FUNCTION
#endif

extern "C" {
  IVP_EXPORT_FUNCTION IvPBehavior * createBehavior(std::string name, IvPDomain domain) 
  {return new BHV_Circle(domain);}
}
#endif
