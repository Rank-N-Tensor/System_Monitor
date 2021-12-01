#include "processor.h"
#include "linux_parser.h"
#include<iostream>

//  Return the aggregate CPU utilization
//got formula from here  https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux


//this is an aggregate calculation. i.e, since first started and doesnt represent "instantaneous" usage
float Processor::Utilization() { 
    _idlejiff = LinuxParser::IdleJiffies();
    _totjiff=LinuxParser::Jiffies(); 
    _utilisation= ((float(_totjiff)-float(_idlejiff)))/float(_totjiff); // had divide by 0 error here, because both the _totjiff and _idlejiff were 0
    std::cout<< _utilisation<<std::endl;
    return _utilisation;
} 