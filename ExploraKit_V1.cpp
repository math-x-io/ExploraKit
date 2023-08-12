#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;  // Namespace alias for the filesystem library

// Check if a directory is special
bool isSpecialDirectory(const fs::path &directory)
{
    static const std::vector<std::string> specialDirs = {"$Recycle.Bin", "$Windows.~BT", "$Windows.~WS"};
    for (const auto &specialDir : specialDirs)
    {
        if (directory.filename() == specialDir)
        {
            return true;  // Directory is special
        }
    }
    return false;  // Directory is not special
}

// Update the progress bar
void updateProgressBar(uintmax_t currentSize, uintmax_t totalSize)
{
    float progress = static_cast<float>(currentSize) / totalSize;
    int barWidth = 50;
    int numChars = static_cast<int>(progress * barWidth);

    std::cout << "\r[";
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < numChars)
        {
            std::cout << "=";
        }
        else
        {
            std::cout << "-";
        }
    }
    std::cout << "] " << int(progress * 100.0) << "%";
    std::cout.flush();  // Flush the output buffer for immediate display
}

// Recursive search for a file in a directory
bool searchFileRecursive(const fs::path &directory, const std::string &targetFileName, uintmax_t totalSize, uintmax_t &currentSize)
{
    try
    {
        for (const auto &entry : fs::directory_iterator(directory))
        {
            if (isSpecialDirectory(entry.path()))
            {
                continue;  // Skip special directories
            }

            try
            {
                if (fs::is_directory(entry))
                {
                    uintmax_t subDirTotalSize = fs::space(entry).capacity;
                    if (searchFileRecursive(entry, targetFileName, subDirTotalSize, currentSize))
                    {
                        return true;  // File found in subdirectory
                    }
                }
                else if (fs::is_regular_file(entry))
                {
                    std::string fileName = entry.path().filename().string();
                    if (fileName == targetFileName)
                    {
                        std::cout << std::endl;
                        std::cout << "File found: " << entry.path() << std::endl;
                        return true;  // File found in the current directory
                    }
                }

                currentSize += entry.file_size();
                updateProgressBar(currentSize, totalSize); 
            }
            catch (const std::exception &ex)
            {
                // Handle exceptions while accessing directories
            }
        }
        return false;  // File not found in the directory
    }
    catch (const std::exception &ex)
    {
        return false;  // Handle exceptions while accessing the directory
    }
}

int main()
{
    std::string targetFileName;
    std::cout << "Enter the name of the file you want to search: ";
    std::cin >> targetFileName;

    char exploreAll;
    do
    {
        std::cout << "Do you want to search on all storage drives? (y/n): ";
        std::cin >> exploreAll;
    } while (exploreAll != 'y' && exploreAll != 'Y' && exploreAll != 'n' && exploreAll != 'N');

    std::vector<fs::path> storageList;

    // Choose to search on all drives or specific drives
    if (exploreAll == 'y' || exploreAll == 'Y')
    {
        // Search on all available drives
        for (char driveLetter = 'A'; driveLetter <= 'Z'; ++driveLetter)
        {
            fs::path drivePath(std::string(1, driveLetter) + ":/");
            if (fs::exists(drivePath))
            {
                storageList.push_back(drivePath);
            }
        }
    }
    else
    {
        // Search on user-specified drives
        for (char driveLetter = 'A'; driveLetter <= 'Z'; ++driveLetter)
        {
            fs::path drivePath(std::string(1, driveLetter) + ":/");
            if (fs::exists(drivePath))
            {
                std::cout << driveLetter << ": ";
                std::wcout << fs::space(drivePath).capacity / (1024 * 1024) << " MB" << std::endl;
            }
        }

        // Ask the user to enter a valid drive letter
        std::string selectedDrive;
        bool validDrive = false;
        do
        {
            std::cout << "Enter the drive letter of an existing drive to explore: ";
            std::cin >> selectedDrive;
            selectedDrive = std::toupper(selectedDrive.front());

            fs::path drivePath(selectedDrive + ":/");
            if (fs::exists(drivePath))
            {
                validDrive = true;
            }
            else
            {
                std::cout << "Invalid drive letter. Please enter a valid drive letter." << std::endl;
            }
        } while (!validDrive);

        storageList.push_back(fs::path(selectedDrive + ":/"));
    }

    // Record the start time of the search
    auto start = std::chrono::high_resolution_clock::now();

    bool found = false;
    for (const fs::path &storage : storageList)
    {
        std::cout << "Search in: " << storage << std::endl;
        if (fs::is_directory(storage))
        {
            uintmax_t currentSize = 0;
            uintmax_t totalSize = fs::space(storage).capacity;
            updateProgressBar(0, totalSize);
            if (searchFileRecursive(storage, targetFileName, totalSize, currentSize))
            {
                found = true;
                break;
            }
            std::cout << std::endl;
        }
    }

    // Record the end time of the search
    auto end = std::chrono::high_resolution_clock::now();

    // Display the search result and duration
    if (!found)
    {
        std::cout << "The file '" << targetFileName << "' was not found." << std::endl;
    }

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "\nSearch duration: " << duration << " ms" << std::endl;

    return 0;
