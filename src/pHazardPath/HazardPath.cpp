/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: HazardPath.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <list>
#include <iterator>
#include "MBUtils.h"
#include "HazardPath.h"
#include <string>
#include <stdlib.h>
#include <math.h>

using namespace std;

std::vector< std::vector< std::vector<int> > > trackline_vec; 
std::vector<double> m_pos; 

//---------------------------------------------------------
// Constructor

HazardPath::HazardPath()
{
}

//---------------------------------------------------------
// Destructor

HazardPath::~HazardPath()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool HazardPath::OnNewMail(MOOSMSG_LIST &NewMail)
{
   MOOSMSG_LIST::iterator p;
   
   for(p=NewMail.begin(); p!=NewMail.end(); p++) {
      CMOOSMsg &msg = *p;

      if(msg.GetString()=="NAV_X"){
	m_pos[0]=msg.GetDouble(); 
      }else if(msg.GetString()=="NAV_Y"){
	m_pos[1]=msg.GetDouble(); 
      }
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool HazardPath::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()

bool HazardPath::Iterate()
{
   // happens AppTick times per second
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
// happens before connection is open

bool HazardPath::OnStartUp()
{
  list<string> sParams;                                                                                         
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {         
    list<string>::iterator p;                                           
    for(p=sParams.begin(); p!=sParams.end(); p++) {                 
      string original_line = *p;                                    
      string line = *p;                                    
      string param = stripBlankEnds(toupper(biteString(line, '=')));  
      string value = stripBlankEnds(line);
                                                           
      if(param == "OFFSET") {                                      
        m_offset = 2*atoi(value.c_str())-10;                 
      }                                                             
      else if(param == "START_OFFSET") {                                  
        m_start_offset = atoi(value.c_str());    
      }                            
    }
  } 

  RegisterVariables();	
  genLawnMower(m_offset,m_start_offset); 
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void HazardPath::RegisterVariables()
{
  // Register("FOOBAR", 0);
  m_Comms.Register("NAV_X",0); 
  m_Comms.Register("NAV_Y",0); 
}

//---------------------------------------------------------

void HazardPath::genLawnMower(int offset, int start_offset){

  int tl[] = {-150,-75}; 
  int ll[] = {-150,-400}; 
  int lr[] = {400, -400}; 
  int tr[] = {400,-75}; 

  string points; 
  int xpt = tl[0]+start_offset; 
  int yoff = 10; 
  int num_time_repeat=1; 

  points = points + intToString(xpt) +","+intToString(tl[1]+yoff)+":"; 
  points = points + intToString(xpt) +","+intToString(lr[1]-yoff);
  xpt += offset;

  int i = 3; 
  while(xpt<lr[0]+offset){
    if(i%2==0){
      points = points +":" +intToString(xpt) +","+intToString(tl[1]+yoff); 
      points = points +":" +intToString(xpt) +","+intToString(lr[1]-yoff);
    }
    else
      {
	points = points +":" +intToString(xpt) +","+intToString(lr[1]-yoff);      
	points = points +":" +intToString(xpt) +","+intToString(tl[1]+yoff); 
      }
    xpt += offset; 
    i++; 
  }
  for (int r; r<num_time_repeat; num_time_repeat++){
    points += points; 
  }
  points = "points="+points; 
  cout << points << endl;
  Notify("PATH_UPDATE",points); 

  return; 
}


string HazardPath::intToString(int val)
 {
   char buff[500];
   sprintf(buff, "%d", val);
   string str = buff;
   return(str);
 }


std::vector<std::vector<std::vector<int> > > HazardPath::genTracklines(int swath){
  int tl[] = {-150,-75}; 
  int ll[] = {-150,-400}; 
  int lr[] = {400, -400}; 
  int tr[] = {400,-75}; 
  int offset = 5; 

  std::vector< std::vector<int> > trackline; 
  std::vector< std::vector< std::vector< int > > > result; 


  int num_lines=(tr[0]-tl[0]+0.5)/swath; 
    cout<<"num_lines= "<<num_lines<<endl; 
    for(int i=0; i<=num_lines; i++){
      trackline[0][0]= swath/2.0+swath*i; 
      trackline[0][1]= tl[1]+offset; 
      trackline[1][0]= trackline[0][0]; 
      trackline[1][1]= tl[1]-offset; 
      result[i]=trackline; 
    }

return result; 
}

bool HazardPath::traverseTrackline(int num){
  std::vector< std::vector<int> > trackline = trackline_vec[num]; 
  double capture_radius = .75; 
  std::string points= "points="; 
  if(getDist(trackline[0],m_pos)<getDist(trackline[1],m_pos)){
    points = points +intToString(trackline[0][0])+","+intToString(trackline[0][1])+":"; 
    points = points +intToString(trackline[1][0])+","+intToString(trackline[1][1]);
    m_Comms.Notify("PATH_UPDATE",points); 
    while(getDist(trackline[1],m_pos)>capture_radius){}
    return true; 
  }else{
    points = points +intToString(trackline[1][0])+","+intToString(trackline[1][1])+":"; 
    points = points +intToString(trackline[0][0])+","+intToString(trackline[0][1]);
    m_Comms.Notify("PATH_UPDATE",points); 
    while(getDist(trackline[0],m_pos)>capture_radius){}
    return true; 
  }
  return false;   
}

double HazardPath::getDist(std::vector<int> cat, std::vector<double> dog){
  double mouse = pow(dog[0]-cat[0],2)+pow(dog[1]-cat[1],2); 
  return pow(mouse,2); 
} 

