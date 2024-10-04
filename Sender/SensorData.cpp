// This program reads sensor data for temperature, SPO2, and pulse rate,
// writes the data to a file, and then reads the data from the file to print it to the console.

#include <iostream>   // For standard I/O operations
#include <fstream>    // For file I/O operations
#include <cstdlib>    // For rand() and srand() functions
#include <string>     // For using the string class
#include <iomanip>    // For setting output width using setw()

using namespace std;

// Function to generate a random number within a given range
int randoms(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Function to generate sensor data and write it to a file
void generateDataToFile(const string& filename) {
    // Open the file for writing
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error opening file for writing!" << endl;
        return;
    }

    // Write the header to the file
    outFile << "Temperature(°C) SPO2(%)  Pulse Rate(bpm)" << endl;

    // Write 50 sets of sensor data to the file
    for (int i = 0; i < 50; i++) {
        outFile << setw(10) << randoms(0, 102)   // Generate and write a random temperature value
                << setw(10) << randoms(60, 100)  // Generate and write a random SPO2 value
                << setw(10) << randoms(90, 220)  // Generate and write a random pulse rate value
                << endl;
    }

    // Close the file
    outFile.close();
}

// Function to read sensor data from a file and print it to the console
void readDataFromFile(const string& filename) {
    // Open the file for reading
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error opening file for reading!" << endl;
        return;
    }

    string line;

    // Read and print the header from the file
    if (getline(inFile, line)) {
        cout << line << endl;
    }

    // Read and print each line of sensor data from the file
    while (getline(inFile, line)) {
        cout << line << endl;
    }

    // Close the file
    inFile.close();
}

int main() {
    // Define the filename for the sensor data file
    const string filename = "sensor_data.txt";

    // Generate sensor data and write it to the file
    generateDataToFile(filename);

    // Read sensor data from the file and print it to the console
    readDataFromFile(filename);

    return 0;
}