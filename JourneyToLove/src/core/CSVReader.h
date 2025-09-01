#pragma once
#include <string>
#include <vector>

class CSVReader
{
public:
    // Constructor
    CSVReader();

    // Read data from a file and store it
    bool ReadData(const std::string& filename, char delimiter = ',');

    // Getters for data
    const std::vector<std::vector<std::string>>& GetData() const;
    size_t GetRows() const;
    size_t GetCols() const;

private:
    std::vector<std::vector<std::string>> data;
};
