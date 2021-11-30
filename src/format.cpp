#include <string>

#include "format.h"

using namespace std;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int hours = seconds / 3600;
  int minutes = (seconds - (hours * 3600)) / 60;
  int seconds_left = (seconds - (hours * 3600)) - (minutes * 60);

  string time;
  string h_s = to_string(hours);
  string m_s = to_string(minutes);
  string s_l_s = to_string(seconds_left);
  if (hours < 10) {
    time += "0" + h_s;
  } else {
    time += h_s;
  }
  time += ":";
  if (minutes < 10) {
    time += "0" + m_s;
  } else {
    time += m_s;
  }
  time+= ":";
  if (seconds_left < 10) {
    time += "0" + s_l_s;
  } else {
    time += s_l_s;
  }
return time;
}