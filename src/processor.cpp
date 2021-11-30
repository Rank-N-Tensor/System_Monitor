#include "processor.h"
#include "linux_parser.h"
//  Return the aggregate CPU utilization
//got formula from here  https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux

void Processor::Set_IdleJiff(){
    _idlejiff = LinuxParser::IdleJiffies();
} 

void Processor::Set_TotJiff(){
   _totjiff=LinuxParser::Jiffies(); 
}


//this is an aggregate calculation. i.e, since first started and doesnt represent "instantaneous" usage
float Processor::Utilization() { 
    _utilisation= ((float(_totjiff)-float(_idlejiff)))/float(_totjiff);
    return _utilisation;
} 