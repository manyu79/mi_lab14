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
#include "BHV_Front.h"
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

BHV_Front::BHV_Front(IvPDomain gdomain) : 
  IvPBehavior(gdomain)
{
  IvPBehavior::setParam("name", "Front");
  m_domain = subDomain(m_domain, "course,speed");


  // Default values for behavior state variables
  m_Kp  = 0;
  m_Kd  = 0;
  m_new_pos[0] = 0.0; 
  m_new_pos[1] = 0.0; 
  m_timer1 = 0.0;
  m_first_run = true; 
  m_t_hot = 20.0; 
  m_t_cold = 17.0; 

  addInfoVars("UCTD_SENSOR_REPORT");
}

//---------------------------------------------------------------
// Procedure: setParam - handle behavior configuration parameters

bool BHV_Front::setParam(string param, string val) 
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  double double_val = atof(val.c_str());
  if((param == "kp") && (isNumber(val))) {
    m_Kp = double_val;
    return(true);
  }
  else if((param == "kd") && (isNumber(val))) {
    m_Kd = double_val;
    return(true);
  }
  return(false);
}

//-----------------------------------------------------------
// Procedure: onIdleState

void BHV_Front::onIdleState() 
{
}

//-----------------------------------------------------------
// Procedure: onRunState

IvPFunction *BHV_Front::onRunState() 
{
  if(m_first_run){
    m_first_run = false; 
    m_timer1 = getBufferCurrTime(); 
  }
 
  /*
  while(!dx_delay(100.0,&m_timer1)){
    m_speed = 2.0; 
    m_head = 90+45;  
    IvPFunction *ipf = buildFunctionWithZAIC(); 
    return(ipf); 
  }
  */


  keepFront();
  m_speed = 2.0; 
  IvPFunction *ipf = buildFunctionWithZAIC(); 
  return(ipf); 
  /*
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
 
  }else if(dxyazdi_delay(m_durr,&m_timer1) && m_flag){
    if (dxyazdi_delay(m_durr+m_zig_duration,&m_timer2)) {

    }
    IvPFunction *ipf = buildFunctionWithZAIC();  
    return(ipf); 
  } 

  return(0);

  */
}

//-----------------------------------------------------------
// Procedure: buildFunctionWithZAIC

IvPFunction *BHV_Front::buildFunctionWithZAIC() 
{
  ZAIC_PEAK spd_zaic(m_domain, "speed");
  spd_zaic.setSummit(m_speed); 
  spd_zaic.setPeakWidth(0.5); 
  spd_zaic.setBaseWidth(1.0); 
  spd_zaic.setSummitDelta(0.8); 

  ZAIC_PEAK crs_zaic(m_domain, "course");
  crs_zaic.setSummit(m_head); 
  crs_zaic.setPeakWidth(0.0); 
  crs_zaic.setBaseWidth(180.0); 
  crs_zaic.setSummitDelta(0.0); 

  IvPFunction *spd_ipf = spd_zaic.extractIvPFunction(); 
  IvPFunction *crs_ipf = crs_zaic.extractIvPFunction(); 

  OF_Coupler coupler;
  IvPFunction *ivp_function = coupler.couple(crs_ipf, spd_ipf, 50, 50);

  ivp_function -> setPWT(100); 

  return(ivp_function);
}

//------------------------------------------------------
// updateSensorData();
void BHV_Front::updateSensorData(){
  bool ok1; 
  string str = getBufferStringVal("UCTD_SENSOR_REQUEST",ok1); 
  str = stripBlankEnds(str); 
  vector<string> vals = parseString(str,","); 
  for(vector<string>::size_type i=0; i<vals.size(); i++){
    string param = biteString(vals[i], '='); 
    string value = vals[i]; 
    if(param=="x"){
      m_new_pos[0]=strtod(value.c_str(),NULL); 
    }else if(param=="y"){
      m_new_pos[1]=strtod(value.c_str(),NULL); 
    }else if(param=="temp"){
      m_temp[0]=m_temp[1]; 
      m_temp[1]=strtod(value.c_str(),NULL); 
    }
  }
}

//---------------------------------------------------------
//dx_delay(); 
bool BHV_Front::dx_delay(double delay, double *init_time){
  if(getBufferCurrTime()-*init_time>=delay){
    //postWMessage("ballz"); 
    return true; 
  }else{
    postWMessage("nuts"); 
    return false; 
  }
}


//-------------------------------------------------------
//keepFront()
void BHV_Front::keepFront(){
  updateSensorData(); 
  if(m_temp[1]>=m_t_hot){
    m_head = 90.0; 
  }else if(m_temp[1]<=m_t_cold){
    m_head = 0.0; 
  }else {
    m_head = 90; 
    m_speed = .75; 
  }
  return; 
}
