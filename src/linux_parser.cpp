#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <unistd.h>

#include "linux_parser.h"


using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  const std::filesystem::path pidsDirectory{kProcDirectory};
  for (auto const& dir_entry : std::filesystem::directory_iterator{pidsDirectory})
  {
    // Extract the second /
    std::string directory = dir_entry.path().string();
    std::string delim = "/proc/";
    std::string token = directory.substr(directory.find(delim)+delim.length(), directory.length());
    
    if (std::all_of(token.begin(), token.end(), [](unsigned char ch){return std::isdigit(ch);}))
      {
         pids.push_back(std::stoi(token));
      }
  }
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { return 0.0; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { return 0; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return process CPU utilization
float LinuxParser::CpuUtilization(int pid) { 
  
  std::vector<std::string> stat_vector;
  std::string line, token;
  std::string uptime_str;
  float cpu_usage, uptime, utime, stime, cutime, cstime, starttime;
  // Get uptime of the system
  std::ifstream uptime_stream(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename);
  if (uptime_stream.is_open()) {
    
  std::getline(uptime_stream, line);
  std::istringstream linestream(line);
  linestream >> uptime_str;
  uptime = std::stof(uptime_str);
  }

  std::ifstream stat_stream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatFilename);

  if (stat_stream.is_open()) {
    while (getline(stat_stream, token, ' ')) {
        // store token string in the vector
        stat_vector.push_back(token);
    }
  
    long system_hertz = sysconf(_SC_CLK_TCK);

    utime = std::stof(stat_vector[13]);
    stime = std::stof(stat_vector[14]);
    cutime = std::stof(stat_vector[15]);
    cstime = std::stof(stat_vector[16]);
    starttime = std::stof(stat_vector[21]);

    float total_time = utime + stime + cutime + cstime;
    float seconds = uptime - (starttime/system_hertz);
    cpu_usage = ((total_time/system_hertz) / seconds);
  }
  else{
    // Process ended so we produce 0% usage
    cpu_usage = 0;
  }

  return cpu_usage;
}

std::string LinuxParserHelper::GetStatLine(std::string stat, std::ifstream& stream){
  
  std::string first_token;
  std::string line;
  
  if (stream.is_open()) {
    
    // Skip lines
    while (first_token != stat && std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> first_token;
    }
  }
  else
    // Cannot find the file, process closed!
    return "";

  return line;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 

  int total_processes = 0;
  std::string token;

  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  std::string line = LinuxParserHelper::GetStatLine("processes", stream);

  std::istringstream linestream(line);

  // Get name of line, then the int
  linestream >> token >> token;

  total_processes = std::stoi(token);

  return total_processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int running_processes = 0;
  std::string token;

  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  std::string line = LinuxParserHelper::GetStatLine("procs_running", stream);

  std::istringstream linestream(line);

  // Get name of line, then the int
  linestream >> token >> token;

  running_processes = std::stoi(token);

  return running_processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
float LinuxParser::Ram(int pid) { 
  float vmSize = 0;

  std::string token;
  std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatusFilename);
  std::string line = LinuxParserHelper::GetStatLine("VmSize:", stream);

  if (line != "")
  {
    std::istringstream linestream(line);

    // Get name of line, then the int
    linestream >> token >> token;
    
    // Convert to MB
    vmSize = std::stoi(token)/(1*1000);
    
    return vmSize;
  }
  else
    return -1;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  
  std::string token;
  std::string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  
  if (stream.is_open()) {
    
    // Skip the first few lines
    for (auto i=0; i<8; i++)
      std::getline(stream, line);

    // Get the actual line that contains uid
    std::getline(stream, line);
  }

  std::istringstream linestream(line);
  
  linestream >> token >> token;

  return token;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 

  std::string line;

  std::string uid_str = LinuxParser::Uid(pid);
  std::string extracted_uid;
  std::string extracted_line;

  std::ifstream stream(kPasswordPath);

  if (stream.is_open()) {
    while (extracted_uid != uid_str && std::getline(stream, line))
    {

      extracted_line = line;
      for (auto i = 0; i<3; i++)
      {
        // Get Delimited UID
        std::string delimiter = ":";

        extracted_uid = line.substr(0, line.find(delimiter));

        line.erase(0, line.find(delimiter)+1);
      }
    }
  }
  std::string user = extracted_line.substr(0, extracted_line.find(":"));

  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
