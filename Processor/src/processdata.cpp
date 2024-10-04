#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <Windows.h>

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

void ProcessVitals(int temperature, int spo2Value, int pulseRate)
{
    std::vector<int> temperatures;
    std::vector<int> spo2;
    std::vector<int> pulseRates;
    temperatures.push_back(temperature);
    spo2.push_back(spo2Value);
    pulseRates.push_back(pulseRate);

    // Calculate max, min
    int maxTemperature = *std::max_element(temperatures.begin(), temperatures.end());
    int minTemperature = *std::min_element(temperatures.begin(), temperatures.end());

    int maxSpo2 = *std::max_element(spo2.begin(), spo2.end());
    int minSpo2 = *std::min_element(spo2.begin(), spo2.end());

    int maxPulseRate = *std::max_element(pulseRates.begin(), pulseRates.end());
    int minPulseRate = *std::min_element(pulseRates.begin(), pulseRates.end());

    // Output the results
    std::cout << "Temperature (°C):" << std::endl;
    std::cout << "  Max: " << maxTemperature << std::endl;
    std::cout << "  Min: " << minTemperature << std::endl;

    std::cout << "SPO2 (%):" << std::endl;
    std::cout << "  Max: " << maxSpo2 << std::endl;
    std::cout << "  Min: " << minSpo2 << std::endl;

    std::cout << "Pulse Rate (bpm):" << std::endl;
    std::cout << "  Max: " << maxPulseRate << std::endl;
    std::cout << "  Min: " << minPulseRate << std::endl;


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
        int temperature, spo2Value, pulseRate;
        lineStream >> temperature >> spo2Value >> pulseRate;
		ProcessVitals(temperature, spo2Value, pulseRate);
    
    }

    return 0;

}



   
 
