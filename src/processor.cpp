#include "processor.h"
#include "linux_parser.h"
#include <iostream>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    
    std::vector<std::string> utilisation;
    std::string token;
    std::string line;

    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        
        // Get name of line first
        linestream >> token;

        // Get the rest of the line
        while(linestream >> token){
        utilisation.push_back(token);
        }
    }
    
    float user = std::stoi(utilisation[0]);
    float nice = std::stoi(utilisation[1]);
    float system = std::stoi(utilisation[2]);
    float idle = std::stoi(utilisation[3]);
    float iowait = std::stoi(utilisation[4]);
    float irq = std::stoi(utilisation[5]);
    float softirq = std::stoi(utilisation[6]);
    float steal = std::stoi(utilisation[7]);

    static float previowait, previdle, prevuser, prevnice, prevsystem, previrq, prevsoftirq, prevsteal;

    float PrevIdle = previdle + previowait;
    float Idle = idle + iowait;

    float PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
    float NonIdle = user + nice + system + irq + softirq + steal;

    float PrevTotal = PrevIdle + PrevNonIdle;
    float Total = Idle + NonIdle;

    // differentiate: actual value minus the previous one
    float totald = Total - PrevTotal;
    float idled = Idle - PrevIdle;

    previdle = idle;
    previowait = iowait;
    prevuser = user;
    prevnice = nice;
    prevsystem = system;
    previrq = irq;
    prevsoftirq = softirq;
    prevsteal = steal;

    float CPU_Percentage = (totald - idled)/totald;

    return CPU_Percentage;
}