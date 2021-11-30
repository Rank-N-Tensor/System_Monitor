#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

//  Return this process's ID
//process constructor
 Process::Process(int pid): _pid(pid){
    Process::Set_SysUptime();
    Process::Set_ActiveJiff();
    Process::Set_Startime();
    try {
    _cpuUtil = float(_actjiff)/float((_sysuptime-_startime)) ;
} catch(...){
    _cpuUtil=0;
}
}
void Process::Set_ActiveJiff(){
    _actjiff=LinuxParser::ActiveJiffies(_pid);
}

void Process::Set_Startime(){
    _startime=LinuxParser::UpTime(_pid);
}

void Process::Set_SysUptime(){
    _sysuptime = LinuxParser::UpTime();
}

int Process::Pid() { return _pid; }

//  Return this process's CPU utilization
float Process::CpuUtilization() const{ 
    return _cpuUtil ; 
}
 
//  Return the command that generated this process
string Process::Command() { 
  return  LinuxParser::Command(_pid);
 }

//  Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(_pid) ; }

//  Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(_pid); }

//  Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime()-  LinuxParser::UpTime(_pid); } //Uptime function has been overloaded

//  Overload the "less than" comparison operator for Process objects

bool Process::operator<(Process const& a) const { return CpuUtilization() < a.CpuUtilization(); }  // error arising due to const nature of return value, if calculating directly from formula, the value is susceptible to change