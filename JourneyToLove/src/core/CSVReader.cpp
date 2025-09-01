#include "CSVReader.h"
#include <iostream>
#include <fstream>
#include <sstream>

CSVReader::CSVReader() {}

bool CSVReader::ReadData(const std::string & CSVname, char delimiter)
{
    std::ifstream CSV(CSVname);
    if (!CSV.is_open())
    {
        std::cerr << "Error: Could not open CSV " << CSVname << std::endl;
        return false;
    }

    data.clear();
    std::string line;
    while (std::getline(CSV, line))
    {
        std::vector<std::string> row;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, delimiter))
        {
            row.push_back(cell);
        }
        data.push_back(row);
    }

    CSV.close();
    return true;
}

const std::vector<std::vector<std::string>>& CSVReader::GetData() const
{
    return data;
}

size_t CSVReader::GetRows() const
{
    return data.size();
}

size_t CSVReader::GetCols() const
{
    if (data.empty())
    {
        return 0;
    }
    return data[0].size();
}