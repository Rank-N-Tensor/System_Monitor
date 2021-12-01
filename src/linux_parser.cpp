#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include<iostream>
#include "linux_parser.h"
using std::stof;
using std::string;
using std::ifstream;
using std::istringstream;
using std::to_string;
using std::all_of;
using std::vector;

//trying to create a generic function
template <typename CpuData>
//CpuData Extractor( string const &reqd_key,const std::optional<int> &pid=std::nullopt){ // couldnt use optional because i could not figure out how to use c++17
  CpuData Extractor(string const &reqd_key,bool pid_status,int pid = 0){
  string line,key,file;
  CpuData value,actual_value;

  if(pid_status) {
   file=LinuxParser::kProcDirectory+to_string(pid)+LinuxParser::kStatusFilename;
  }
  else{
    file =LinuxParser::kProcDirectory+LinuxParser::kStatFilename ;
  }
  ifstream datastrm(file);
  while(datastrm.is_open()){
    while(getline(datastrm,line)){
      istringstream strm(line);
      while(strm>>key>>value){
        if(key==reqd_key){
          actual_value= value;
        }
      }
    }
    datastrm.close();
  }  

  return actual_value;
}

template <typename ProcData>  // this returns a vector of ProcData containing individual entries of a process's interaction with the cpu
vector<ProcData> Extractor_Proc(int &pid){
  string iterator,line;
  vector<ProcData> pid_data;
  ifstream pidstrm(LinuxParser::kProcDirectory+to_string(pid)+LinuxParser::kStatFilename);
  while(pidstrm.is_open()){
    getline(pidstrm,line);
    istringstream strm(line);
    while(strm>>iterator){
      pid_data.push_back(iterator);
    }
    pidstrm.close();
   }
   return pid_data;
}


// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

//  Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  long mem_T, mem_F,value;
  string line,key;
  std::ifstream memstrm(kProcDirectory+kMeminfoFilename);
  while(memstrm.is_open()){
    while(getline(memstrm,line)){
      replace(line.begin(),line.end(),':',' ');
      std::istringstream strm(line);
      while(strm>>key>>value){
        if(key=="MemTotal"){
          
          mem_T=value;
        }
        else if(key=="MemFree"){
          
          mem_F=value;
        }
      }
    }
    memstrm.close();
  }
  return ((float)mem_T - (float)mem_F)/((float)mem_T);
}


//  Read and return the system uptime
long LinuxParser::UpTime() { 
 long utime;
 string line;
 ifstream ut_strm(kProcDirectory+kUptimeFilename);
 while(ut_strm.is_open()){
     while (getline(ut_strm,line)){
         istringstream strm(line);
         strm>>utime;
 }
    ut_strm.close();
}
return utime;
}

/*
ok bear in mind , a jiffy is tick (like in a clock)
so the total number of jiffes is essentially the time elapsed
*/

//  Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
   vector<string> cpu_util_jiffies;
  string line,cpu_no,user,nice,system,idle,iowait, irq,softirq,steal,guest,guest_nice;
  ifstream cpustrm(kProcDirectory+ kStatFilename);
  while(cpustrm.is_open()){
  getline(cpustrm,line);
  istringstream strm(line);
  strm>>cpu_no>>user>>nice>>system>>idle>>iowait>>irq>>softirq>>steal>>guest>>guest_nice;
  cpu_util_jiffies={user,nice,system,idle,iowait, irq,softirq,steal,guest,guest_nice};
  cpustrm.close();
  }
  return cpu_util_jiffies;
 }




//  Read and return the number of active jiffies for a PID

long LinuxParser::ActiveJiffies(int pid) {
    auto pid_data = Extractor_Proc<string>(pid);
    long tot_time=std::stol(pid_data[13])+std::stol(pid_data[14])+std::stol(pid_data[15])+std::stol(pid_data[16]);
    return tot_time / sysconf(_SC_CLK_TCK);
}

//  Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  auto jiffies = CpuUtilization();
  auto active= std::stol(jiffies[CPUStates::kUser_]) + std::stol(jiffies[CPUStates::kSystem_]) + std::stol(jiffies[CPUStates::kNice_]) + std::stol(jiffies[CPUStates::kGuest_]) + std::stol(jiffies[CPUStates::kGuestNice_]) +std::stol(jiffies[CPUStates::kSteal_]) +std::stol(jiffies[CPUStates::kIRQ_]) + std::stol(jiffies[CPUStates::kSoftIRQ_]);
 
  return active;
 }

//  Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  auto jiffies = CpuUtilization();
  auto idle = std::stol(jiffies[CPUStates::kIdle_]) + std::stol(jiffies[CPUStates::kIOwait_]) ;
  return idle;
 }

//  Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  auto jiffies=CpuUtilization(); 
  return ActiveJiffies() + IdleJiffies(); // in essence the total number of jiffies for the system
 }

//  Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return Extractor<int>("processes",false); //using template, remmber to reassign CpuData to a generic type

 }

//  Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return Extractor<int>("procs_running",false);
 }

//  Read and return the command associated with a process

string LinuxParser::Command(int pid) { 
  ifstream cmdstrm(kProcDirectory+to_string(pid)+kCmdlineFilename);
  string line;
  getline(cmdstrm,line);
  return line;
 }

//  Read and return the memory used by a process

string LinuxParser::Ram(int pid) { 
  return Extractor<string>("VmSize:",true,pid);
}

//  Read and return the user ID associated with a process

string LinuxParser::Uid(int pid) { 
  return Extractor<string>("Uid:",true,pid);  
  }

//  Read and return the user associated with a process

string LinuxParser::User(int pid) { 
  ifstream pid_user_strm(kProcDirectory+to_string(pid)+kPasswordPath);
  string line,user,actual_user,uid;
  char  x;
  while (pid_user_strm.is_open()) {
    while (getline(pid_user_strm, line)) {
      replace(line.begin(),line.end(),':',' ');  
      istringstream strm(line);
      while(strm>>user>>x>>uid){
          if(uid==Uid(pid)){
              actual_user= user;
          }
      }
    }
    pid_user_strm.close();
  }
  return actual_user;
}

//  Read and return the uptime of a process

long LinuxParser::UpTime(int pid ) { 
  auto pid_data = Extractor_Proc<string>(pid);
  long time;
  try{
  time= std::stol(pid_data[21])/sysconf(_SC_CLK_TCK);
 }catch(...){
time = 0;
}
return time;
}