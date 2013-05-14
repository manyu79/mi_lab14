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
#include <iostream>



using namespace std;

//-----------------------------------------------------------
// Procedure: Constructor

BHV_Front::BHV_Front(IvPDomain gdomain) : 
  IvPBehavior(gdomain)
{
  IvPBehavior::setParam("name", "Front");
  m_domain = subDomain(m_domain, "course,speed");


  // Default values for behavior state variables
  m_new_pos[0] = 0.0; 
  m_new_pos[1] = 0.0; 
  m_t_hot = 25; 
  m_t_cold = 20; 
  m_t_avg  = (m_t_hot+m_t_cold)/2.0; 
  m_speed = 2.0; 
  m_head = 0.0; 
  m_x_max = 160;
  m_x_min = -90; 
  m_first_run = true; 
  m_iter = 0; 
 
  addInfoVars("UCTD_MSMNT_REPORT");
  addInfoVars("T_NORTH, T_SOUTH"); 
}

//---------------------------------------------------------------
// Procedure: setParam - handle behavior configuration parameters

bool BHV_Front::setParam(string param, string val) 
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  double double_val = atof(val.c_str());
  if((param == "angle") && (isNumber(val))) {
    m_ang = double_val;
    m_dir[0]=m_ang; 
    m_dir[1]=180-m_ang; 
    m_dir[2]=m_ang+180;
    m_dir[3]=360-m_ang; 
    return(true);
  }
  else if((param == "offset") && (isNumber(val))) {
    m_off = double_val;
    return(true);
  }else if((param == "x_max") && (isNumber(val))) {
    m_x_max = double_val;
    return(true);
  }else if((param == "x_min") && (isNumber(val))) {
    m_x_min = double_val;
    return(true);
  }else if((param == "vname")) {
    m_name = stripBlankEnds(val);
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
    getTemps(); 
    m_iter++; 
    if(m_iter>100){
    m_first_run = false;
    } 
  }

  //getTemps(); 
  keepFront();
  m_speed = 2.0; 
  IvPFunction *ipf = buildFunctionWithZAIC(); 
  return(ipf); 
}

//-----------------------------------------------------------
// Procedure: buildFunctionWithZAIC

IvPFunction *BHV_Front::buildFunctionWithZAIC() 
{
  ZAIC_PEAK spd_zaic(m_domain, "speed");
  spd_zaic.setSummit(m_speed); 
  spd_zaic.setPeakWidth(0.25); 
  spd_zaic.setBaseWidth(.5); 
  spd_zaic.setSummitDelta(0.8); 

  ZAIC_PEAK crs_zaic(m_domain, "course");
  crs_zaic.setSummit(m_head); 
  crs_zaic.setPeakWidth(0.0); 
  crs_zaic.setBaseWidth(90.0); 
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
  bool vee_name = false; 
  string str = getBufferStringVal("UCTD_MSMNT_REPORT",ok1); 
  str = stripBlankEnds(str); 
  vector<string> vals = parseString(str,","); 
  for(vector<string>::size_type i=0; i<vals.size(); i++){
    string param = biteString(vals[i], '='); 
    string value = vals[i]; 

    if (param == "vname"){
      if (value == m_name){
	vee_name = true; 
      }
    }if(param=="x" && vee_name){
      m_new_pos[0]=strtod(value.c_str(),NULL); 
    }else if(param=="y" && vee_name){
      m_new_pos[1]=strtod(value.c_str(),NULL); 
    }else if(param=="temp" && vee_name){
      m_temp[0]=m_temp[1]; 
      m_temp[1]=strtod(value.c_str(),NULL); 
    }
  }
  cout<<"v_name: "<<m_name<<endl; 
}

//---------------------------------------------------------
/*
bool BHV_Front::dx_delay(double delay, double *init_time){
  if(getBufferCurrTime()-*init_time>=delay){
    return true; 
  }else{
    return false; 
  }
}
*/
//-------------------------------------------------------
void BHV_Front::keepFront(){
  updateSensorData(); 
  if(m_temp[1]>=m_t_avg+m_off){
    m_head=min_dhead(m_dir[0],m_dir[3]); 
  }else if(m_temp[1]<=m_t_avg-m_off){
    m_head=min_dhead(m_dir[1],m_dir[2]); 
  }
 if(m_new_pos[0]>m_x_max){
    m_head=min_dhead(m_dir[2],m_dir[3]); 
  }else if(m_new_pos[0]<m_x_min){
    m_head=min_dhead(m_dir[0],m_dir[1]); 
  }
  return; 
}
//--------------------------------------------------------
void BHV_Front::getTemps(){
  bool ok1; 
  string str = getBufferStringVal("T_SOUTH",ok1); 
  str = stripBlankEnds(str); 
  str = biteString(str, '='); 
  string value = str; 
  m_t_hot = strtod(value.c_str(),NULL); 
  str = getBufferStringVal("T_NORTH",ok1); 
  str = stripBlankEnds(str); 
  str = biteString(str, '='); 
  value = str; 
  m_t_cold = strtod(value.c_str(),NULL); 
  m_t_avg = (m_t_hot + m_t_cold)/2.0;
  return; 
}

//----------------------------------------------------------
double BHV_Front::min_dhead(double cat, double dog){
  if(abs(cat-m_head)>=abs(dog-m_head)){
    return dog; 
  }else{
    return cat; 
  }
}
//--------------------------------------------------------
/*
void BHV_Front::computeGrad(){
  updateSensorData(); 
  double dx=m_new_pos[0]-m_old_pos[0]; 
  double dy=m_new_pos[1]-m_old_pos[1];
  double dT=m_temp[1]-m_temp[0]; 
  m_grad[0]=dT/dx; 
  m_grad[1]=dT/dy; 
}
*/
//------------------------------------------------------------

