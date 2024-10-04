#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>

std::vector<float> temperature;
std::vector<float> spo2;
std::vector<float> pulseRate;

std::string ReadConsoleOutput(const std::string& command)
{
    std::string output;

    // Create anonymous pipe
    HANDLE pipeRead, pipeWrite;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;
    if (!CreatePipe(&pipeRead, &pipeWrite, &sa, 0))
    {
        throw std::runtime_error("Failed to create pipe");
    }

    // Create process and redirect output to pipe
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.hStdError = pipeWrite;
    si.hStdOutput = pipeWrite;
    si.dwFlags |= STARTF_USESTDHANDLES;
    if (!CreateProcess(NULL, const_cast<char*>(command.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
    {
        throw std::runtime_error("Failed to create process");
    }
    CloseHandle(pipeWrite);

    // Read output from the read end of the pipe
    char buffer[4096];
    DWORD bytesRead;
    while (ReadFile(pipeRead, buffer, sizeof(buffer), &bytesRead, NULL))
    {
        if (bytesRead == 0)
        {
            break;
        }
        output.append(buffer, bytesRead);
    }

    CloseHandle(pipeRead);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return output;
}

void getMaxMinValue(const std::vector<float> & vitals,float *maxValue, float *minValue)
{
    *maxValue = *std::max_element(vitals.begin(), vitals.end());
    *minValue = *std::min_element(vitals.begin(), vitals.end());
}

void DisplayMaxMinVitals(std::string vitalName , const std::vector<float> & vitals)
{
    float maxValue ;
    float minValue ;
    getMaxMinValue(vitals,&maxValue,&minValue);
    std::cout << vitalName <<" :" << std::endl;
    std::cout << "  Max: " << maxValue << std::endl;
    std::cout << "  Min: " << minValue << std::endl;
}

void ProcessVitals(float temperatureValue, float spo2Value, float pulseRateValue)
{
    temperature.push_back(temperatureValue);
    spo2.push_back(spo2Value);
    pulseRate.push_back(pulseRateValue);
    
    DisplayMaxMinVitals("Temperature",temperature);
    DisplayMaxMinVitals("SPO2",spo2);
    DisplayMaxMinVitals("PulseRate",pulseRate);

    //TODO simple moving average of last 5 values 
}

int main()
{
    std::string sender_executable_name = "sender.exe";
    std::string console_output = ReadConsoleOutput(sender_executable_name);
    std::cout << console_output << std::endl;

    std::istringstream iss(console_output);
    std::string line;

    // Skip the header line
    std::getline(iss, line);

    // Read the incoming data 
    while (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        float temperatureValue, spo2Value, pulseRateValue;
        lineStream >> temperatureValue >> spo2Value >> pulseRateValue;
		ProcessVitals(temperatureValue, spo2Value, pulseRateValue);
    }

    return 0;

}



   
 
