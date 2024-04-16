#include <iostream>
#include <thread>
#include <string>

void download(std::string file) {
    for (int i = 0; i < 10; i++) {
        std::cout << "Downloading " << file
                  << " (" << i * 10 << "%)..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }   
    std::cout << "Download complete: " << file << std::endl;
}

int main() {
    std::thread th1(download, "th1sayhello.zip");
    th1.join(); // returns when the thread execution has completed.


    std::thread th2([] {
        download("th2sayhello.zip");
    });
    th2.detach(); // allowing them to execute independently from each other.

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return 0;
}