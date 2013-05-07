/*****************************************************************/
/*    NAME: M.Benjamin, H.Schmidt, J. Leonard                    */
/*    ORGN: NAVSEA Newport RI and MIT Cambridge MA               */
/*    FILE: BHV_Circle.cpp                                        */
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
#include <math.h>
#include "BHV_Circle.h"
#include "MBUtils.h"
#include "AngleUtils.h"
#include "BuildUtils.h"
#include "ZAIC_PEAK.h"
#include <iostream>

using namespace std;

//-----------------------------------------------------------
// Procedure: Constructor

BHV_Circle::BHV_Circle(IvPDomain gdomain) : 
  IvPBehavior(gdomain)
{

  m_domain = subDomain(gdomain, "course");

  // Default values for behavior state variables
  m_osx     = 0;
  m_osy     = 0;
  m_oshead  = 0;
  m_now   = 0;

  m_radius = 0;
  m_range = 0;
  m_startang = 0;
  m_presc_ang = 0;
  m_ang = 0;
  m_send = true;
  m_arc  = 360;

  m_priority_wt = 100;

  m_raderr = 1;
  m_angerr = 20;

  addInfoVars("NAV_X, NAV_Y, NAV_HEADING");
}

//---------------------------------------------------------------
// Procedure: setParam - handle behavior configuration parameters

bool BHV_Circle::setParam(string param, string val) 
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  double double_val = atof(val.c_str());
  if(param == "name")
    IvPBehavior::setParam("name", val);
  else if((param == "radius") && (isNumber(val))) {
    m_radius = double_val;
    return(true);
  }
  else if((param == "arc") && (isNumber(val))) {
    m_arc = double_val;
    return(true);
  }
  else if(param == "center") {
    vector<string> svec = parseString(val,',');
    m_cenx = strtod(svec[0].c_str(),NULL);
    m_ceny = strtod(svec[1].c_str(),NULL);
    return(true);
  }
  return(false);
}

//-----------------------------------------------------------
// Procedure: onIdleState

void BHV_Circle::onIdleState() 
{
  updateState();
}

//-----------------------------------------------------------

void BHV_Circle::onIdleToRunState()
{
  m_presc_ang = 0;
  m_ang = 0;
  m_startang = -1;
}
//-----------------------------------------------------------
// Procedure: onRunState

IvPFunction* BHV_Circle::onRunState() 
{
  updateState();

  if((m_range >= m_radius-m_raderr) && (m_range <= m_radius+m_raderr) && (m_startang == -1)){
    cout << "[CIRC] Latched! Angle: " << m_ang << endl;
    m_startang = m_ang;
    m_presc_ang = 0;
  }

  if(m_presc_ang >= m_arc-m_angerr){
    cout << "[CIRC] Breakaway! Angle Prescribed: " << m_presc_ang << endl;
    m_presc_ang = 0;
    m_startang = -1;
    m_send = false;
  }

  double rel_course = 180 - (90/m_radius)*(m_range);
  if (rel_course<0) rel_course = 0;
  
  IvPFunction *ipf = 0;

  int ang = (double)(m_ang+rel_course);
  ipf = buildCourseWithZAIC(ang % 360);

  if(ipf)
    ipf->setPWT(m_priority_wt);
  if(m_send)
    return(ipf);
  else setComplete();

}
//-----------------------------------------------------------
// Procedure; updateState

void BHV_Circle::updateState()
{
  cout << "[CIRC] Update" << endl;
  bool ok1,ok2,ok3;
  m_osx = getBufferDoubleVal("NAV_X",ok1);
  m_osy = getBufferDoubleVal("NAV_Y",ok2);
  m_oshead = getBufferDoubleVal("NAV_HEADING",ok3);
  if (!ok1 || !ok2 || !ok3){
    postWMessage("No Ownship X/Y/H data");
    return;
  }
  
  m_now = getBufferCurrTime();
  
  m_range = pow(pow(m_osx-m_cenx,2) + pow(m_osy-m_ceny,2),0.5);
  double prev_ang = m_ang;
  m_ang   = relAng(m_osx,m_osy,m_cenx,m_ceny);
  double diff = m_ang-prev_ang;
  
  if (diff<0) diff = -1*diff;
  if (diff>(m_radius/2)) diff = m_radius/2;
  if (m_ang != 0) m_presc_ang += diff;
  
  cout << "[CIRC] Cur: " << m_ang << " Prev: " << prev_ang << " Presc: " << m_presc_ang << endl;
  postMessage("M_PRESC",m_presc_ang);
}
//-----------------------------------------------------------
// Procedure: buildFunctionWithZAIC

IvPFunction* BHV_Circle::buildCourseWithZAIC(double ang) {

  ZAIC_PEAK crs_zaic(m_domain, "course");
  crs_zaic.setSummit(ang);
  crs_zaic.setPeakWidth(0);
  crs_zaic.setBaseWidth(180.0);
  crs_zaic.setSummitDelta(0);  
  crs_zaic.setValueWrap(true);
  if(crs_zaic.stateOK() == false) {
    string warnings = "Course ZAIC problems " + crs_zaic.getWarnings();
    postWMessage(warnings);
    return(0);
  }

  IvPFunction* hdd_ipf = crs_zaic.extractIvPFunction();
  
  return(hdd_ipf);
}
  
