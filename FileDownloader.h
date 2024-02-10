#pragma once

#include <cstdio>
#include <curl/curl.h>
#include <iostream>
#include <string>

class FileDownloader {
public:

    FileDownloader();
    ~FileDownloader();

    [[nodiscard]]
    bool downloadFile(const std::string& url, const std::string& filePath);
    bool downloadAndExecute(const std::string& url, const std::string& filePath, const std::string& params);

private:
    static int progressFunc(void* ptr, double TotalToDownload, double NowDownloaded,
                            double TotalToUpload, double NowUploaded);
    static size_t writeDataCallback(void* ptr, size_t size, size_t nmemb, FILE* stream);
};


