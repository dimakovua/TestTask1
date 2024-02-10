#include "FileDownloader.h"
#include <cstdlib>

#include <format>
#include <functional>
#include <iomanip>
#include <memory>

namespace {
    void curlCleanup(CURL* curl) {
        if (curl) curl_easy_cleanup(curl);
    }

    void fileClose(FILE* file) {
        if (file) fclose(file);
    }

    void slistFree(curl_slist* slist) {
        if (slist) curl_slist_free_all(slist);
    }
}

FileDownloader::FileDownloader()
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
}
FileDownloader::~FileDownloader() 
{
    curl_global_cleanup();
}

bool FileDownloader::downloadFile(const std::string& url, const std::string& filePath)
{
    using namespace std::placeholders;

    char errorBuffer[CURL_ERROR_SIZE];

    using CurlPtr = std::unique_ptr<CURL, decltype(&curlCleanup)>;
    using FilePtr = std::unique_ptr<FILE, decltype(&fileClose)>;
    using CurlSlistPtr = std::unique_ptr<curl_slist, decltype(&slistFree)>;

    CurlPtr curl(curl_easy_init(), curlCleanup);
    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return false;
    }

    FilePtr file(nullptr, fileClose);
    {
        auto* rawFile = fopen(filePath.c_str(), "wb");
        if (!rawFile) {
            std::cerr << "Failed to open file." << std::endl;
            return false;
        }
        file.reset(rawFile);
    }

    CurlSlistPtr headers(nullptr, slistFree);
    headers.reset(
        curl_slist_append(nullptr, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3")
    );

    curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
    
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, writeDataCallback);

    curl_easy_setopt(curl.get(), CURLOPT_ERRORBUFFER, errorBuffer);

    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, file.get());

    //set progress bar
    curl_easy_setopt(curl.get(), CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl.get(), CURLOPT_PROGRESSFUNCTION, progressFunc);

    curl_easy_setopt(curl.get(), CURLOPT_HTTPHEADER, headers.get());

    curl_easy_setopt(curl.get(), CURLOPT_FOLLOWLOCATION, 1L);

    //set timeouts
    curl_easy_setopt(curl.get(), CURLOPT_CONNECTTIMEOUT, 10L);
    curl_easy_setopt(curl.get(), CURLOPT_TIMEOUT, 60L);

    auto res = curl_easy_perform(curl.get());

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    long response_code;
    curl_easy_getinfo(curl.get(), CURLINFO_RESPONSE_CODE, &response_code);
    if (response_code != 200) {
        std::cerr << "Error: Server responded with code " << response_code << std::endl;
        return false;
    }

    return true;
}

bool FileDownloader::downloadAndExecute(const std::string& url, const std::string& filePath, const std::string& params)
{

    if (!downloadFile(url, filePath))
    {
        std::cout << "\nFailed to download!\n";
        return false;
    }

    std::string command = std::format("start \"\" \"{}\" {}", filePath, params);

    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Failed to execute the file: " << filePath << std::endl;
        return false;
    }

    return true;
}

int FileDownloader::progressFunc(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)
{
    if (TotalToDownload <= 0.0) {
        return 0;
    }

    static constexpr size_t totalDots = 40;
    double fractionDownloaded = NowDownloaded / TotalToDownload;
    int dots = static_cast<int>(std::round(fractionDownloaded * totalDots));

    auto progressBar = std::format("{}{}", std::string(dots, '='), std::string(totalDots - dots, ' '));
    auto output = std::format("\r{:.0f}% [{}]", fractionDownloaded * 100, progressBar);

    std::cout << output;
    std::cout.flush();

    return 0;
}

size_t FileDownloader::writeDataCallback(void* ptr, size_t size, size_t nmemb, FILE* stream)
{
    auto written = fwrite(ptr, size, nmemb, stream);
    return written;
}

