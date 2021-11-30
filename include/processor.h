#ifndef PROCESSOR_H
#define PROCESSOR_H
#include<sstream>
#include<fstream>
class Processor {
 public:
  float Utilization();  //  See src/processor.cpp


  void Set_TotJiff();
  void Set_IdleJiff();

  private:
    long  _totjiff, _idlejiff;
    float _utilisation;
};

#endif