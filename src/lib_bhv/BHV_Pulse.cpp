/*****************************************************************/
/*    NAME: M.Benjamin, H.Schmidt, J. Leonard                    */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: BHV_SimpleWaypoint.cpp                               */
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

#include <cstdlib>
#include <cmath>
#include "BHV_Pulse.h"
#include "MBUtils.h"
#include "AngleUtils.h"
#include "BuildUtils.h"
#include "ZAIC_PEAK.h"
#include "XYRangePulse.h"
#include "IvPBehavior.h"
#include <unistd.h>
#include <iostream>


using namespace std;


//-----------------------------------------------------------
// Procedure: Constructor

BHV_Pulse::BHV_Pulse(IvPDomain gdomain) : 
  IvPBehavior(gdomain)
{
  IvPBehavior::setParam("name", "pulse");
  m_domain = subDomain(m_domain, "course,speed");

  // All distances are in meters, all speed in meters per second
  // Default values for configuration parameters 
  m_pulse_duration  = 4; 
  m_radius = 20;

  // Default values for behavior state variables
  m_osx  = 0;
  m_osy  = 0;
  m_wpt  = 0; 
  m_wpt_old = 0; 
  m_call_time=0.0; 
  m_flag = false; 


  addInfoVars("WPT_INDEX", "nowarning");
  addInfoVars("NAV_X, NAV_Y");
}

//---------------------------------------------------------------
// Procedure: setParam - handle behavior configuration parameters

bool BHV_Pulse::setParam(string param, string val) 
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  double double_val = atof(val.c_str());
  if((param == "pulse_range")  && (isNumber(val))) {
    m_radius=double_val;
    return(true);
  }
  else if((param == "pulse_duration") && (isNumber(val))) {
    m_pulse_duration=double_val;
    return(true);
  }
  return(false);
}

//-----------------------------------------------------------
// Procedure: onIdleState

void BHV_Pulse::onIdleState() 
{
}

//-----------------------------------------------------------
// Procedure: postViewPoint

void BHV_Pulse::postViewPoint() 
{

  m_current_time = getBufferCurrTime(); 
  
  XYRangePulse pulse; 
  pulse.set_x(m_osx); 
  pulse.set_y(m_osy); 
  pulse.set_label("bhv_pulse"); 
  pulse.set_rad(m_radius); 
  pulse.set_duration(m_pulse_duration); 
  pulse.set_time(m_current_time); 
  pulse.set_color("edge","yellow"); 
  pulse.set_color("fill","yellow"); 

  string range_str=pulse.get_spec();  
  postMessage("VIEW_RANGE_PULSE", range_str);
  return; 
}

//-----------------------------------------------------------
// Procedure: onRunState

IvPFunction *BHV_Pulse::onRunState() 
{
  // Part 1: Get vehicle position from InfoBuffer and post a 
  // warning if problem is encountered
  //postWMessage("executing onRunState()"); 
  bool ok1, ok2,ok3;
  m_osx = getBufferDoubleVal("NAV_X", ok1);
  m_osy = getBufferDoubleVal("NAV_Y", ok2);
  if(!ok1 || !ok2) {
    postWMessage("No ownship X/Y info in info_buffer.");
    return(0);
  }

  m_wpt = getBufferDoubleVal("WPT_INDEX",ok3);

  if (abs(m_wpt-m_wpt_old)>.9){ 
    m_call_time=getBufferCurrTime(); 
    m_wpt_old=m_wpt; 
    m_flag=true; 
  }else if (dxyazdi_delay(5.0) && m_flag){
    postViewPoint();
    m_flag=false; 
  }


  return(0);
}

//----------------------------------------------------------
// doubleToString()
#if 0
string BHV_Pulse::doubleToString(double val, int digits)
  {
    char format[10] = "%.5f\0";
    if((digits>=0)&&(digits<=9))
      format[2] = digits+48;

    if(val > (double)(pow((double)(2),(double)(128))))
      format[3] = 'e';

    char buff[1000];
    sprintf(buff, format, val);
    string str = buff;
    return(str);
  }
#endif

//---------------------------------------------------------
//dxyazdi_delay(); 
bool BHV_Pulse::dxyazdi_delay(double delay){
  if(getBufferCurrTime()-m_call_time>=delay){
    return true; 
  }else{
    return false; 
  }
}
