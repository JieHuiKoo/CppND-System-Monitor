#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { 
    
    cpu_ = Processor();

    return cpu_;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    
    processes_ = {};

    vector<int> pids = LinuxParser::Pids();

    for (auto pid : pids)
    {
        // Create new process
        Process process = Process(pid);
        processes_.push_back(process);
    }

    // std::cout << processes_[4].CpuUtilization() << std::endl;
    // std::sort(processes_.begin(), processes_.end(), [](Process& lhs, Process& rhs){
    //     return  lhs.GetCpuUtilization() > rhs.GetCpuUtilization();});
    std::sort(processes_.begin(), processes_.end());
    return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel();}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 

    float total_mem = 0;
    for (auto process : processes_)
    {
        total_mem += std::stof(process.Ram());
    }

    return total_mem;
 }

// TODO: Return the operating system name
std::string System::OperatingSystem() {
    return LinuxParser::OperatingSystem();
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
   return LinuxParser::RunningProcesses();
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return 0; }
