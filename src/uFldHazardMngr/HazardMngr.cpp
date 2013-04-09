/*****************************************************************/
/*    NAME: Michael Benjamin, Henrik Schmidt, and John Leonard   */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: HazardMgr.cpp                                        */
/*    DATE: Oct 26th 2012                                        */
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

#include <iterator>
#include "MBUtils.h"
#include "HazardMngr.h"
#include "XYFormatUtilsHazard.h"
#include "ACTable.h"
#include <string>

using namespace std;

//---------------------------------------------------------
// Constructor

HazardMngr::HazardMngr()
{
  // Config variables
  m_master              = true;
  m_swath_width_desired = 25;
  m_pd_desired          = 0.9;
  m_name                = "noname";

  // State Variables 
  m_sensor_config_requested = false;
  m_sensor_config_set   = false;
  m_swath_width_granted = 0;
  m_pd_granted          = 0;

  m_sensor_config_reqs = 0;
  m_sensor_config_acks = 0;
  m_sensor_report_reqs = 0;
  m_detection_reports  = 0;
  m_hazard_reports     = 0;
  m_slave_report_received = false;
  m_detecting = true;

  m_summary_reports = 0;

  m_req_flag = 0;
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool HazardMngr::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key   = msg.GetKey();
    string sval  = msg.GetString(); 

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
    
    if(key == "UHZ_CONFIG_ACK") 
      handleMailSensorConfigAck(sval);

    else if(key == "UHZ_OPTIONS_SUMMARY") 
      handleMailSensorOptionsSummary(sval);

    else if(key == "UHZ_HAZARD_REPORT")
      handleMailHazardReport(sval);

    else if(key == "UHZ_DETECTION_REPORT") 
      handleMailDetectionReport(sval);

    else if(key == "HAZARDSET_REQUEST") 
      handleMailReportRequest();
    
    else if(key == "SURVEY_INFO"){
      if(sval == "ended")
	m_detecting = false;
      if(sval == "begin")
	m_detecting = true;
    }
	 
    else if(key == "SLAVE_REPORT"){
      parseIncomingReport(sval); 
      m_slave_report_received = true;
      //      cout<<"slave report received"<<endl; 
    }
 
    else 
      reportRunWarning("Unhandled Mail: " + key);
  }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool HazardMngr::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool HazardMngr::Iterate()
{
  AppCastingMOOSApp::Iterate();

  if(m_detecting && !m_sensor_config_requested)
    postSensorConfigRequest();

  if(m_sensor_config_set)
    postSensorInfoRequest();
  
  if(m_req_flag)
    handleMailReportRequest();

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool HazardMngr::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(true);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if((param == "swath_width") && isNumber(value)) {
      m_swath_width_desired = atof(value.c_str());
      handled = true;
    }
    else if(((param == "sensor_pd") || (param == "pd")) && isNumber(value)) {
      m_pd_desired = atof(value.c_str());
      handled = true;
     }
    else if(param == "report_name") {
      value = stripQuotes(value);
      m_report_name = value;
      handled = true;
    }
    else if(param == "is_master") {
      if(value == "true") m_master=true;
      else if(value == "false") m_master=false;
      else continue; 
      handled = true;
    }
    else if(param == "name") {
      value = stripQuotes(value);
      m_name = value;
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);
  }
  
  m_hazard_set.setSource(m_host_community);
  m_class_hazard.setSource(m_host_community);
  m_hazard_set.setName(m_report_name);
  m_class_hazard.setName(m_report_name);
  
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void HazardMngr::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  m_Comms.Register("UHZ_DETECTION_REPORT", 0);
  m_Comms.Register("UHZ_HAZARD_REPORT",0);
  m_Comms.Register("UHZ_CONFIG_ACK", 0);
  m_Comms.Register("UHZ_OPTIONS_SUMMARY", 0);
  m_Comms.Register("HAZARDSET_REQUEST", 0);
  m_Comms.Register("SLAVE_REPORT",0); 
  m_Comms.Register("TIME_OUT",0); 
  m_Comms.Register("SURVEY_INFO",0);
}

//---------------------------------------------------------
// Procedure: postSensorConfigRequest

void HazardMngr::postSensorConfigRequest()
{
  string request = "vname=" + m_host_community;
  
  request += ",width=" + doubleToStringX(m_swath_width_desired,2);
  request += ",pd="    + doubleToStringX(m_pd_desired,2);

  m_sensor_config_requested = true;
  m_sensor_config_reqs++;
  Notify("UHZ_CONFIG_REQUEST", request);
}

//---------------------------------------------------------
// Procedure: postSensorInfoRequest

void HazardMngr::postSensorInfoRequest()
{
  string request = "vname=" + m_host_community;

  m_sensor_report_reqs++;
  Notify("UHZ_SENSOR_REQUEST", request);
}

//---------------------------------------------------------
// Procedure: handleMailSensorConfigAck

bool HazardMngr::handleMailSensorConfigAck(string str)
{
  // Expected ack parameters:
  string vname, width, pd, pfa, pclass;
  
  // Parse and handle ack message components
  bool   valid_msg = true;
  string original_msg = str;

  vector<string> svector = parseString(str, ',');
  unsigned int i, vsize = svector.size();
  for(i=0; i<vsize; i++) {
    string param = biteStringX(svector[i], '=');
    string value = svector[i];

    if(param == "vname")
      vname = value;
    else if(param == "pd")
      pd = value;
    else if(param == "width")
      width = value;
    else if(param == "pfa")
      pfa = value;
    else if(param == "pclass")
      pclass = value;
    else
      valid_msg = false;       

  }


  if((vname=="")||(width=="")||(pd=="")||(pfa=="")||(pclass==""))
    valid_msg = false;
  
  if(!valid_msg)
    reportRunWarning("Unhandled Sensor Config Ack:" + original_msg);

  
  if(valid_msg) {
    m_sensor_config_set = true;
    m_sensor_config_acks++;
    m_swath_width_granted = atof(width.c_str());
    m_pd_granted = atof(pd.c_str());
  }

  return(valid_msg);
}

//---------------------------------------------------------
// Procedure: handleMailHazardReport
//      Note: Report will look like:
//            UHZ_HAZARD_REPORT = x=45,y-88,label=12,type=hazard

bool HazardMngr::handleMailHazardReport(string str)
{
  m_hazard_reports++;

  XYHazard classed_hazard = string2Hazard(str);

  string classed_label = classed_hazard.getLabel();

  int xi = m_hazard_set.findHazard(classed_label);
  if(xi ==-1)
    m_hazard_set.addHazard(classed_hazard);
  else
    m_hazard_set.setHazard(xi, classed_hazard);
  
  string event = "Classified: label=" + classed_hazard.getLabel()
    +" type="+classed_hazard.getType();
  reportEvent(event);

  return(true);
  
}

//---------------------------------------------------------
// Procedure: handleMailDetectionReport
//      Note: The detection report should look something like:
//            UHZ_DETECTION_REPORT = vname=betty,x=51,y=11.3,label=12 

bool HazardMngr::handleMailDetectionReport(string str)
{
  m_detection_reports++;

  XYHazard new_hazard = string2Hazard(str);

  string hazlabel = new_hazard.getLabel();
  
  if(hazlabel == "") {
    reportRunWarning("Detection report received for hazard w/out label");
    return(false);
  }

  int ix = m_hazard_set.findHazard(hazlabel);
  if(ix == -1)
    m_hazard_set.addHazard(new_hazard);
  else
    m_hazard_set.setHazard(ix, new_hazard);

  string event = "New Detection, label=" + new_hazard.getLabel();
  event += ", x=" + doubleToString(new_hazard.getX(),1);
  event += ", y=" + doubleToString(new_hazard.getY(),1);

  reportEvent(event);

  string req = "vname=" + m_host_community + ",label=" + hazlabel;

  Notify("UHZ_CLASSIFY_REQUEST", req);


  return(true);
}


//---------------------------------------------------------
// Procedure: handleMailReportRequest

void HazardMngr::handleMailReportRequest()
{

  string summary_report;
  m_summary_reports++;

  for(int i = 0;i<m_hazard_set.size();i++){
    XYHazard haz = m_hazard_set.getHazard(i);
    if(haz.getType()!="benign" && !m_class_hazard.hasHazard(haz.getLabel()) )
      m_class_hazard.addHazard(haz);
  }
  
  summary_report = m_class_hazard.getSpec("final_report");      

  if(m_master){
    if (m_req_flag = 0) m_req_flag = MOOSTime();    
    if( (m_slave_report_received) ){ //|| ((MOOSTime()-m_req_flag)>20) ){
      Notify("HAZARDSET_REPORT", summary_report); 
      m_req_flag = 0;
      m_slave_report_received = false;
    }
  }
  else{
    summary_report = "src_node="+m_name+",dest_node=all,var_name=SLAVE_REPORT,string_val=\""+summary_report+"\"";
    Notify("NODE_MESSAGE_LOCAL", summary_report); 
  }
}

//------------------------------------------------------------
// Procedure: buildReport()

bool HazardMngr::buildReport() 
{
  m_msgs << "Config Requested:"                                  << endl;
  m_msgs << "    swath_width_desired: " << m_swath_width_desired << endl;
  m_msgs << "             pd_desired: " << m_pd_desired          << endl;
  m_msgs << "   config requests sent: " << m_sensor_config_reqs  << endl;
  m_msgs << "                  acked: " << m_sensor_config_acks  << endl;
  m_msgs << "------------------------ "                          << endl;
  m_msgs << "Config Result:"                                     << endl;
  m_msgs << "       config confirmed: " << boolToString(m_sensor_config_set) << endl;
  m_msgs << "    swath_width_granted: " << m_swath_width_granted << endl;
  m_msgs << "             pd_granted: " << m_pd_granted          << endl << endl;
  m_msgs << "--------------------------------------------" << endl << endl;

  m_msgs << "               sensor requests: " << m_sensor_report_reqs << endl;
  m_msgs << "                hazard reports: " << m_hazard_reports     << endl;
  m_msgs << "             detection reports: " << m_detection_reports  << endl << endl; 

  m_msgs << "   Hazardset Reports Requested: " << m_summary_reports << endl;
  m_msgs << "      Hazardset Reports Posted: " << m_summary_reports << endl;
  m_msgs << "                   Report Name: " << m_report_name << endl;

  return(true);
}


//--------------------------------------------------------------------
// Procedure: parseIncomingReport() (from slave)

void HazardMngr::parseIncomingReport(string report){
  vector<string> svec;
  vector<string> repvec = parseString(report, '#');
  for (int i=2;i<repvec.size();i++){
    //cout << repvec[i] << endl;
    /*    svec = parseString(repvec[i], ',');
    for (int j=0;j<svec.size();j++){
      biteString(svec[j],'=');
    }
    XYHazard cheesesteak;
    cheesesteak.setX(strtod(svec[0].c_str(),NULL));
    cheesesteak.setX(strtod(svec[1].c_str(),NULL));
    cheesesteak.setLabel(svec[2]);
    */
    XYHazard haz = string2Hazard(repvec[i]);
    haz.setType("hazard");
    if (!m_hazard_set.hasHazard(haz.getLabel())) m_hazard_set.addHazard(haz);
  }

  return; 
}
