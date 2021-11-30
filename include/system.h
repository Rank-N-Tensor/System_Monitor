#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();                   //  See src/system.cpp  //this is just a reference to the cpu, which is of type processor
  std::vector<Process>& Processes();  //  See src/system.cpp   //reference to vector of processes.
  float MemoryUtilization();          //  See src/system.cpp
  long UpTime();                      //  See src/system.cpp
  int TotalProcesses();               //  See src/system.cpp
  int RunningProcesses();             //  See src/system.cpp
  std::string Kernel();               //  See src/system.cpp
  std::string OperatingSystem();      //  See src/system.cpp

  //  Define any necessary private members
 private: 
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif