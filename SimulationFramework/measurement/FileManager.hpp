#pragma once

#include <fstream>
#include <filesystem>

namespace SF::measurement
{
    class FileManager
    {
    public:
        FileManager(const std::string &filename)
        {
            file.open(std::filesystem::current_path().parent_path().append(filename));
        }

        ~FileManager()
        {
            file.close();
        }

        std::ofstream& operator()()
        {
            return file;
        }

    private:
        std::ofstream file{};
    };
}