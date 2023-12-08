#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

Process::Process(int pid) 
{
    pid_ = pid;
    Process::CpuUtilization();
    Process::Ram();
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    cpuUtilization_ = LinuxParser::CpuUtilization(pid_);
    return cpuUtilization_;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
std::string Process::Ram() { 
    vmSize_ = LinuxParser::Ram(pid_);

    if (vmSize_ != -1)
    {
        return std::to_string(vmSize_);
    }
    else
        return "0";
}

// TODO: Return the user (name) that generated this process
std::string Process::User() { 
    return LinuxParser::User(pid_);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return this->cpuUtilization_<a.cpuUtilization_;}