/*****************************************************************/
/*    NAME: Henrik Schmidt                                       */
/*    ORGN: MIT Cambridge MA                                     */
/*    FILE: FrntEstimate.cpp                                 */
/*    DATE: Aug. 2, 2008                                         */
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

#include "FrntEstimate.h"

#include <iostream>
#include <sstream>
#include <math.h>
#include "MBUtils.h"

using namespace std;


CFrntEstimate::CFrntEstimate()
{
  num_param = 9;
  num_meas = 0;
  in_survey = false;
  anneal.clearMeas();
  anneal_step = 0;
  concurrent = false;
  adaptive   = false;
  report_sent = true;
  new_anneal_report=false;
  temp_fac = 1;
  cooling_steps = 100;
  min_offset = -200;
  max_offset = 0;
  min_angle = -10;
  max_angle = 40;
  min_amplitude = 0;
  max_amplitude = 50;
  min_period = 60;
  max_period = 180;
  min_wavelength = 50;
  max_wavelength = 150;
  min_alpha = -200;
  max_alpha = 200;
  min_beta = 10;
  max_beta = 50;
  min_T_N = 15;
  max_T_N = 25; 
  min_T_S = 20;
  max_T_S = 30; 
  partner_report = false;
  master = true;

  // Weights: These are the weights (relative to 1) for the master vehicle.
  wt_offset = 1;
  wt_angle = 1;
  wt_amplitude = 1;
  wt_period = 1;
  wt_wavelength = 1;
  wt_alpha = 1;
  wt_beta = 1;
  wt_T_N = 1;
  wt_T_S = 1;
  wt_temperature = 1;


  stat_T_N = 20;
  stat_T_S = 25;
  first_report = true;
}

CFrntEstimate::~CFrntEstimate()
{
}

bool CFrntEstimate::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();
  string sVal;
  
  if(m_MissionReader.GetConfigurationParam("vname",sVal))
    {
      vname = sVal;
    }     
  if(m_MissionReader.GetConfigurationParam("master",sVal))
    {
      if( sVal == "false") master = false;
    }
  
  if(m_MissionReader.GetConfigurationParam("temperature_factor",sVal))
    {
      temp_fac = atof(sVal.c_str());
    }     
  
  if(m_MissionReader.GetConfigurationParam("cooling_steps",sVal))
    {
      cooling_steps = atoi(sVal.c_str());
    }     
  
  if(m_MissionReader.GetConfigurationParam("min_offset",sVal))
    {
      min_offset = atof(sVal.c_str());
    }     
  if(m_MissionReader.GetConfigurationParam("max_offset",sVal))
    {
      max_offset = atof(sVal.c_str());
    }     
  
  if(m_MissionReader.GetConfigurationParam("min_angle",sVal))
    {
      min_angle = atof(sVal.c_str());
    }     
  if(m_MissionReader.GetConfigurationParam("max_angle",sVal))
    {
      max_angle = atof(sVal.c_str());
    }     
  
  if(m_MissionReader.GetConfigurationParam("min_amplitude",sVal))
    {
      min_amplitude = atof(sVal.c_str());
    }     
  if(m_MissionReader.GetConfigurationParam("max_amplitude",sVal))
    {
      max_amplitude = atof(sVal.c_str());
    }     
  
  if(m_MissionReader.GetConfigurationParam("min_period",sVal))
    {
      min_period = atof(sVal.c_str());
    }     
  if(m_MissionReader.GetConfigurationParam("max_period",sVal))
    {
      max_period = atof(sVal.c_str());
    }     
  
  if(m_MissionReader.GetConfigurationParam("min_wavelength",sVal))
    {
      min_wavelength = atof(sVal.c_str());
    }     
  if(m_MissionReader.GetConfigurationParam("max_wavelength",sVal))
    {
      max_wavelength = atof(sVal.c_str());
    }     
  
  if(m_MissionReader.GetConfigurationParam("min_alpha",sVal))
    {
      min_alpha = atof(sVal.c_str());
    }     
  if(m_MissionReader.GetConfigurationParam("max_alpha",sVal))
    {
      max_alpha = atof(sVal.c_str());
    }     
  
  if(m_MissionReader.GetConfigurationParam("min_beta",sVal))
    {
      min_beta = atof(sVal.c_str());
    }     
  if(m_MissionReader.GetConfigurationParam("max_beta",sVal))
    {
      max_beta = atof(sVal.c_str());
    }     
  
  if(m_MissionReader.GetConfigurationParam("min_T_N",sVal))
    {
      min_T_N = atof(sVal.c_str());
    }     
  if(m_MissionReader.GetConfigurationParam("max_T_N",sVal))
    {
      max_T_N = atof(sVal.c_str());
    }     
  
  if(m_MissionReader.GetConfigurationParam("min_T_S",sVal))
    {
      min_T_S = atof(sVal.c_str());
    }     
  if(m_MissionReader.GetConfigurationParam("max_T_S",sVal))
    {
      max_T_S = atof(sVal.c_str());
    }     
  
  if(m_MissionReader.GetConfigurationParam("concurrent",sVal))
    {
      concurrent = MOOSStrCmp(sVal,"true");
      if ( concurrent )
	MOOSTrace("Annealing Concurrently with Survey \n");
    }     
  
  if(m_MissionReader.GetConfigurationParam("adaptive",sVal))
    {
      adaptive = MOOSStrCmp(sVal,"true");
      if ( adaptive )
	MOOSTrace("Using Adaptive Simulated Annealing \n");
    }     
  
  cout << "Finished Getting Parameters" << endl;
  delta_t = 1/(cooling_steps);

  // Initialize annealer
  anneal.setVars(num_param, temp_fac, adaptive);
  vector<double> vars;
  vars.push_back(min_offset);
  vars.push_back(min_angle);
  vars.push_back(min_amplitude);
  vars.push_back(min_period);
  vars.push_back(min_wavelength);
  vars.push_back(min_alpha);
  vars.push_back(min_beta);
  vars.push_back(min_T_N);
  vars.push_back(min_T_S);
  anneal.setMinVal(vars);
  vars.clear();
  vars.push_back(max_offset);
  vars.push_back(max_angle);
  vars.push_back(max_amplitude);
  vars.push_back(max_period);
  vars.push_back(max_wavelength);
  vars.push_back(max_alpha);
  vars.push_back(max_beta);
  vars.push_back(max_T_N);
  vars.push_back(max_T_S);
  anneal.setMaxVal(vars);
  vars.clear();
  vars.push_back(0.5*(max_offset+min_offset));
  vars.push_back(0.5*(max_angle+min_angle));
  vars.push_back(0.5*(max_amplitude+min_amplitude));
  vars.push_back(0.5*(max_period+min_period));
  vars.push_back(0.5*(max_wavelength+min_period));
  vars.push_back(0.5*(max_alpha+min_alpha));
  vars.push_back(0.5*(max_beta+min_beta));
  vars.push_back(0.5*(max_T_N+min_T_N));
  vars.push_back(0.5*(max_T_S+min_T_S));
  anneal.setInitVal(vars);
  cout << "Started Annealer" << endl;
  return(true);
}

bool CFrntEstimate::OnConnectToServer()
{
  m_Comms.Register("SURVEY_UNDERWAY",0);
  m_Comms.Register("UCTD_MSMNT_REPORT",0);
  m_Comms.Register("APPCAST_REQ",0);
  m_Comms.Register("PARTNER_REPORT",0);
  AppCastingMOOSApp::RegisterVariables();
  cout << "Registered for Variables" << endl;
  return(true);
}


bool CFrntEstimate::Iterate()
{
  AppCastingMOOSApp::Iterate();
  double curr_time = MOOSTime();
  //double temperature;
  new_anneal_report=false;
  if (concurrent)
    temperature = exp(- 2*double(anneal_step)/double(cooling_steps));
  else
    temperature = 1.0 - anneal_step*delta_t;

  if (num_meas>0 &&
      ((concurrent && !completed && in_survey) || 
       (!concurrent && completed && anneal_step <= cooling_steps)))
    {
      double energy = anneal.heatBath(temperature);
      MOOSTrace("Annealing step %d. Energy = %f\n",anneal_step,energy); 
      anneal_step += 1;
    }

  if (!report_sent && 
      ((concurrent && completed 
	) || (!concurrent && anneal_step == cooling_steps)))
    {
      vector<double> result;
      anneal.getEstimate(result);
      offset =     result[0];
      angle  =     result[1];
      amplitude =  result[2];
      period =     result[3];
      wavelength = result[4];
      alpha =      result[5];
      beta =       result[6];
      T_N  =       result[7];
      T_S  =       result[8];
      
      postParameterToPartner();
      //      postParameterReport();
      report_sent = true;
      new_anneal_report=false;
      cout << "SENT REPORTS" << endl;
    }
  


  AppCastingMOOSApp::PostReport();
  return(true);
}


//-------------------------------------------------------------
// Procedure: OnNewMail

bool CFrntEstimate::OnNewMail(MOOSMSG_LIST &NewMail)

{
  AppCastingMOOSApp::OnNewMail(NewMail);
  string value;

  MOOSMSG_LIST::iterator p;

// This loop checks ALL messages
  for(p=NewMail.begin(); p!=NewMail.end(); p++)
    {
      CMOOSMsg & rMsg = *p;
      
      if (rMsg.m_sKey == "UCTD_MSMNT_REPORT" && in_survey)
	{
	  value = rMsg.m_sVal;
	  CMeasurement buf;
	  buf = anneal.parseMeas(value);
	  anneal.addMeas(buf);
	  num_meas += 1;
	  MOOSTrace("New measurement added, Total = %d\n", num_meas);
	  report_sent =false;
	}
      else if (rMsg.m_sKey == "SURVEY_UNDERWAY")
	{
	  cout << "SURVEY_UNDERWAY=" + rMsg.m_sVal << endl;
	  if ( !in_survey && rMsg.m_sVal =="true")
	    {
	      MOOSTrace("Survey started\n");
              anneal.clearMeas();
	      in_survey = true;
	      completed = false;
	      report_sent =false;
	      num_meas = 0;
	    }
	  else if ( in_survey && rMsg.m_sVal == "false")
	    {
	      MOOSTrace("Survey completed\n");
	      in_survey = false;
	      completed = true;
	      anneal_step = 0;
	      m_Comms.Notify("T_NORTH",doubleToString(T_N));
	      m_Comms.Notify("T_SOUTH",doubleToString(T_S));


	    }
	}
      else if (rMsg.m_sKey == "PARTNER_REPORT")
	{
	  value = rMsg.m_sVal;
	  parseParameterReport(value);
	  if(master) postParameterReport();
	}
    }

  return(true);
}

void CFrntEstimate::postParameterToPartner()
{
  cout << "Sending to Partner" << endl;
  string sval;
  sval = "vname=" + vname;
  sval += ";offset=" + doubleToString(offset);
  sval += ";angle=" + doubleToString(angle);
  sval += ";amplitude=" + doubleToString(amplitude);
  sval += ";period=" + doubleToString(period);
  sval += ";wavelength=" + doubleToString(wavelength);
  sval += ";alpha=" + doubleToString(alpha);
  sval += ";beta=" + doubleToString(beta);
  
  if (first_report){
    sval += ";tempnorth=" + doubleToString(T_N);
    stat_T_N = T_N;
    sval += ";tempsouth=" + doubleToString(T_S);
    stat_T_S = T_S;
    first_report = false;
  }
  else{
    sval += ";tempnorth=" + doubleToString(stat_T_N);
    sval += ";tempsouth=" + doubleToString(stat_T_S);
  }
  string lval;
  lval = "src_node=" + vname + ",dest_node=all,var_name=PARTNER_REPORT,string_val=" + sval;
  m_Comms.Notify("NODE_MESSAGE_LOCAL",lval);
}

void CFrntEstimate::parseParameterReport(string val)
{
  cout << "Parsing from Partner" << endl;
  vector<string> sval;
  sval = parseString(val,';');
  cout << "  Biting" << endl;
  for(int i=0;i<sval.size();i++) biteString(sval[i],'=');
  if(sval.size()>1){
    cout << "  Setting" << endl;
    p_offset = atof(sval[1].c_str());
    p_angle  = atof(sval[2].c_str() );
    p_amplitude = atof(sval[3].c_str() );
    p_period = atof(sval[4].c_str() );
    p_wavelength = atof(sval[5].c_str() );
    p_alpha = atof(sval[6].c_str() );
    p_beta = atof(sval[7].c_str() );
    p_T_N = atof(sval[8].c_str() );
    p_T_S = atof(sval[9].c_str() );
    partner_report = true;
  }
  cout << "  Returning" << endl;
  return;
}

void CFrntEstimate::postParameterReport()
{

  string sval;
  if(master){
    if(!partner_report){
      sval = "vname=" + vname;
      sval += ",offset=" + doubleToString(offset);
      sval += ",angle=" + doubleToString(angle);
      sval += ",amplitude=" + doubleToString(amplitude);
      sval += ",period=" + doubleToString(period);
      sval += ",wavelength=" + doubleToString(wavelength);
      sval += ",alpha=" + doubleToString(alpha);
      sval += ",beta=" + doubleToString(beta);
      sval += ",tempnorth=" + doubleToString(stat_T_N);
      sval += ",tempsouth=" + doubleToString(stat_T_S);    
    }
    else{
      cout << endl;
      sval = "vname=" + vname;
      sval += ",offset=" + doubleToString((offset*wt_offset+p_offset)/(1+wt_offset));
      sval += ",angle=" + doubleToString((angle*wt_angle+p_angle)/(1+wt_angle));
      sval += ",amplitude=" + doubleToString((amplitude*wt_amplitude+p_amplitude)/(1+wt_amplitude));
      sval += ",period=" + doubleToString((period*wt_period+p_period)/(1+wt_period));
      sval += ",wavelength=" + doubleToString((p_wavelength*wt_wavelength+wavelength)/(1+wt_wavelength));
      sval += ",alpha=" + doubleToString((alpha*wt_alpha+p_alpha)/(1+wt_alpha));
      sval += ",beta=" + doubleToString((p_beta+beta*wt_beta)/(1+wt_beta));
      sval += ",tempnorth=" + doubleToString((p_T_N+stat_T_N*wt_T_N)/(1+wt_T_N));
      sval += ",tempsouth=" + doubleToString((p_T_S+stat_T_S*wt_T_S)/(1+wt_T_S));
    }
    m_Comms.Notify("UCTD_PARAMETER_ESTIMATE",sval);
    cout << "Sending to Shore" << endl;
  }
  return;
}


bool CFrntEstimate::buildReport()
{
  
  m_msgs<<"CFrntEstimate report"<<endl;
  m_msgs<<"____________________"<<endl;
  //if there is a new estimate, publish:
  //  if (new_anneal_report){
 
  //  postParameterReport();
  
  //}
  //else {
    double energy = anneal.heatBath(temperature);
    m_msgs<<"Annealing step = "<<anneal_step-1<<", Energy = "<<energy<<endl;    
   
    m_msgs<<"\nLast Anneal Report:\n"<<endl;
    m_msgs<<"vname="<<vname<<endl;
    m_msgs<<"offset="<<doubleToString(offset)<<endl;
    m_msgs<<"angle="<< doubleToString(angle)<<endl;
    m_msgs<<"amplitude=" <<doubleToString(amplitude)<<endl;
    m_msgs<<"period=" << doubleToString(period)<<endl;
    m_msgs<<"wavelength="<< doubleToString(wavelength)<<endl;
    m_msgs<<"alpha="<< doubleToString(alpha)<<endl;
    m_msgs<<"beta=" << doubleToString(beta)<<endl;
    m_msgs<<"tempnorth=" << doubleToString(T_N)<<endl;
    m_msgs<< "tempsouth=" << doubleToString(T_S)<<endl;
    if (partner_report)
      m_msgs<< "Got something from partner"<<endl;
 

    //}
  return(true);
}
