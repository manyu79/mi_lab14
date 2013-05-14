/*****************************************************************/
/*    NAME: Henrik Schmidt                                       */
/*    ORGN: MIT Cambridge MA                                     */
/*    FILE: FrntEstimate.h                                 */
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

#ifndef FRONTESTIMATE_HEADER
#define FRONTESTIMATE_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
//#include "MOOSLIB/MOOSLib.h"
//#include "MOOSGenLib/MOOSGenLib.h"
//#include "MOOSUtilityLib/MOOSGeodesy.h"
#include <../lib_henrik_util/CSimAnneal.h>
#include <../lib_henrik_util/CFrontSim.h>
#include <string>
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"


class CFrntEstimate : public AppCastingMOOSApp
{
 public:
  CFrntEstimate();
  virtual ~CFrntEstimate();

  bool Iterate();
  bool OnNewMail(MOOSMSG_LIST &NewMail);
  bool OnConnectToServer();
  bool OnStartUp();
  bool buildReport();
 protected:

  CSimAnneal anneal;

  bool in_survey;
  bool completed;
  bool concurrent;
  bool adaptive;
  bool report_sent;
  bool new_anneal_report;
  double temp_fac;
  // Front parameters
  double offset;
  double angle;
  double amplitude;
  double period;
  double wavelength;
  double alpha;
  double beta;
  double T_N;
  double T_S;
  double stat_T_N;
  double stat_T_S;
  
  double temperature;
  // Partnering Parameters
  double p_offset;
  double p_angle;
  double p_amplitude;
  double p_period;
  double p_wavelength;
  double p_alpha;
  double p_beta;
  double p_T_N;
  double p_T_S;
  double p_temperature;

  int num_report;
  bool partner_report;
  bool master;

  // Parameter weights
  double wt_offset;
  double wt_angle;
  double wt_amplitude;
  double wt_period;
  double wt_wavelength;
  double wt_alpha;
  double wt_beta;
  double wt_T_N;
  double wt_T_S;
  double wt_temperature;

  // lower limits for annealer
  double min_offset;
  double min_angle;
  double min_amplitude;
  double min_period;
  double min_wavelength;
  double min_alpha;
  double min_beta;
  double min_T_N;
  double min_T_S;
  // upper limits for annealer
  double max_offset;
  double max_angle;
  double max_amplitude;
  double max_period;
  double max_wavelength;
  double max_alpha;
  double max_beta;
  double max_T_N;
  double max_T_S;
  double delta_t;

  int num_param; 
  int cooling_steps;
  int num_meas;
  int anneal_step;

  std::string vname;

  void postParameterReport();
  void parseParameterReport(std::string val);
  void postParameterToPartner();

};

#endif 

