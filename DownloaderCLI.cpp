#include "DownloaderCLI.h"


void DownloaderCLI::run() 
{
    if (!validateArgs()) {
        std::cerr << "Usage: <executable> <url> <file_path>\n";
        return;
    }
    if (!validateURL(argv[1])) {
        std::cerr << "Invalid URL: URL must start with http:// or https://\n";
        return;
    }
    if (!validateFilePath(argv[2])) {
        std::cerr << "Invalid file path: Path does not match Windows file path format\n";
        return;
    }

    FileDownloader downloader;
    std::string url{ argv[1] };
    std::string filePath{ argv[2] };

    //try to execute with params
    if (argc > 3) {
        std::string params = accumulateParams();

        if (downloader.downloadAndExecute(url, filePath, params))
            std::cout << "\nDownloaded and executed!\n";
        else
            std::cout << "\nFailed!\n";
    }
    else {
        if (downloader.downloadFile(url, filePath)) {
            std::cout << "\nDownloaded!\n";
        }
        else {
            std::cout << "\nFailed!\n";
        }
    }
}

bool DownloaderCLI::validateURL(const std::string& url) const
{
    static std::regex urlPattern(R"(^(https?):\/\/[^\s$.?#].[^\s]*$)");
    return std::regex_search(url, urlPattern);
}

bool DownloaderCLI::validateFilePath(const std::string& filePath) const
{
    static std::regex windowsPathPattern(R"(^[a-zA-Z]:\\(?:[^\\/:*?"<>|\r\n]+\\)*[^\\/:*?"<>|\r\n]*)");
    return std::regex_match(filePath, windowsPathPattern);
}

std::string DownloaderCLI::accumulateParams()
{
    std::vector<std::string_view> vec_params(argv + 3, argv + argc);
    return std::accumulate(
        std::next(vec_params.begin()), vec_params.end(), std::string(vec_params[0]),
        [](const std::string& a, std::string_view b) {
            return a + " " + std::string(b);
        }
    );
}