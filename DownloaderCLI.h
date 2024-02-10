#pragma once

#include "FileDownloader.h"
#include <regex>
#include <string_view>
#include <numeric>

class DownloaderCLI
{
public:
    DownloaderCLI(int argc, char* argv[]) : argc(argc), argv(argv) {}

    void run();

private:
    int argc;
    char** argv;

    bool validateArgs() const {
        return argc >= 3;
    }

    bool validateURL(const std::string& url) const;

    bool validateFilePath(const std::string& filePath) const;
    std::string accumulateParams();
};

