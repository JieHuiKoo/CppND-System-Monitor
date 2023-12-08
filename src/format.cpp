#include <string>
#include <iostream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    long hours = (seconds / 60) / 60;
    long mins = (seconds - (hours*60*60)) / 60;
    long secs = seconds - (hours*60*60 + mins*60); 

    std::string formatted = "";
    formatted += std::to_string(hours);
    formatted += ":";
    formatted += std::to_string(mins);
    formatted += ":";
    formatted += std::to_string(secs);
    formatted += "     ";

    return formatted;
    }