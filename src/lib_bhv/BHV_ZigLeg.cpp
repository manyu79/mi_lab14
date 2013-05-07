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
#include "BHV_ZigLeg.h"
#include "MBUtils.h"
#include "AngleUtils.h"
#include "BuildUtils.h"
#include "ZAIC_PEAK.h"
#include "OF_Coupler.h"
#include "OF_Reflector.h"
#include "XYRangePulse.h"



using namespace std;

//-----------------------------------------------------------
// Procedure: Constructor

BHV_ZigLeg::BHV_ZigLeg(IvPDomain gdomain) : 
  IvPBehavior(gdomain)
{
  IvPBehavior::setParam("name", "pulse");
  m_domain = subDomain(m_domain, "course,speed");

  // All distances are in meters, all speed in meters per second
  // Default values for configuration parameters 

  m_zig_duration = 10; 
  m_angle = 45; 
  m_durr = 12; 

  // Default values for behavior state variables
  m_osx  = 0;
  m_osy  = 0;
  m_wpt  = 0; 
  m_wpt_old = 0; 
  m_current_time = 0;
  m_flag = false; 
  m_timer1 = 0.0; 
  m_timer2 = 0.0; 

  addInfoVars("NAV_X, NAV_Y");
  addInfoVars("WPT_INDEX", "nowarning"); 
  addInfoVars("NAV_HEADING"); 
}

//---------------------------------------------------------------
// Procedure: setParam - handle behavior configuration parameters

bool BHV_ZigLeg::setParam(string param, string val) 
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  double double_val = atof(val.c_str());
  if((param == "zig_duration") && (isNumber(val))) {
    m_zig_duration=double_val;
    return(true);
  }
  else if((param == "zig_angle") && (isNumber(val))) {
    m_angle=double_val;
    return(true);
  }
  return(false);
}

//-----------------------------------------------------------
// Procedure: onIdleState

void BHV_ZigLeg::onIdleState() 
{
}

//-----------------------------------------------------------
// Procedure: onRunState

IvPFunction *BHV_ZigLeg::onRunState() 
{
  // Part 1: Get vehicle position from InfoBuffer and post a 
  // warning if problem is encountered
  bool ok1, ok2,ok3;
  m_osx = getBufferDoubleVal("NAV_X", ok1);
  m_osy = getBufferDoubleVal("NAV_Y", ok2);
  if(!ok1 || !ok2) {
    postWMessage("No ownship X/Y info in info_buffer.");
    return(0);
  }
  m_wpt = getBufferDoubleVal("WPT_INDEX",ok3);
  if (abs(m_wpt-m_wpt_old)>.9 && !m_flag) {
    m_heading = getBufferDoubleVal("NAV_HEADING", ok1);  
    postWMessage(doubleToString(m_heading)); 
    m_timer1=getBufferCurrTime(); 
    m_timer2 = getBufferCurrTime();
    m_wpt_old=m_wpt;
    m_flag=true; 
  }else if(dxyazdi_delay(m_durr,&m_timer1) && m_flag){
    if (dxyazdi_delay(m_durr+m_zig_duration,&m_timer2)) {
      m_flag = false; 
    }
    IvPFunction *ipf = buildFunctionWithZAIC(); 
    ipf -> setPWT(100); 
    return(ipf); 
  } 

  return(0);
}

//-----------------------------------------------------------
// Procedure: buildFunctionWithZAIC

IvPFunction *BHV_ZigLeg::buildFunctionWithZAIC() 
{
  ZAIC_PEAK spd_zaic(m_domain, "speed");
  spd_zaic.setSummit(2.0); 
  spd_zaic.setPeakWidth(0.5); 
  spd_zaic.setBaseWidth(1.0); 
  spd_zaic.setSummitDelta(0.8); 

  bool ok1; 
  ZAIC_PEAK crs_zaic(m_domain, "course");
  crs_zaic.setSummit(m_heading-m_angle); 
  crs_zaic.setPeakWidth(0.0); 
  crs_zaic.setBaseWidth(180.0); 
  crs_zaic.setSummitDelta(0.0); 

  IvPFunction *spd_ipf = spd_zaic.extractIvPFunction(); 
  IvPFunction *crs_ipf = crs_zaic.extractIvPFunction(); 

  OF_Coupler coupler;
  IvPFunction *ivp_function = coupler.couple(crs_ipf, spd_ipf, 50, 50);

  return(ivp_function);
}

//---------------------------------------------------------
//dxyazdi_delay(); 
bool BHV_ZigLeg::dxyazdi_delay(double delay, double *init_time){
  if(getBufferCurrTime()-*init_time>=delay){
    return true; 
  }else{
    return false; 
  }
}
