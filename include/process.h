#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
    Process(int pid);

  //the following are accessor functions  
  int Pid();                               //  See src/process.cpp
  std::string User();                      //  See src/process.cpp
  std::string Command();                   //  See src/process.cpp
  float CpuUtilization() const;                  //  See src/process.cpp
  std::string Ram();                       //  See src/process.cpp
  long int UpTime();                       //  See src/process.cpp
  bool operator<(Process const& a) const;  //  See src/process.cpp
  void Set_ActiveJiff();
  void Set_Startime();
  void Set_SysUptime();

  //  Declare any necessary private members
 private:
 int _pid; //using Mr.david silver's notation
 long _actjiff,_startime,_sysuptime;  // here _uptime = startime , check https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
 float _cpuUtil;
};

#endif