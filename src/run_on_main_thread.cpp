#include <iostream>
#include <thread>
#include <string>

void download(std::string file) 
{
    for (int i = 0; i < 10; i++) {
        std::cout << "Downloading " << file
                  << " (" << i * 10 << "%)..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }   
    std::cout << "Download complete: " << file << std::endl;
}


int main() {
    download("hello.zip");
    return 0;
}