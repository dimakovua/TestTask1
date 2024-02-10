#include "DownloaderCLI.h"

int main(int argc, char* argv[]) {
    DownloaderCLI cli(argc, argv);
    cli.run();
    return 0;
}
