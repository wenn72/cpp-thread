#include <iostream>
#include <thread>
#include <string>
#include <future>

int download(std::string file) 
{
    for (int i = 0; i < 10; i++) {
        std::cout << "Downloading " << file
                  << " (" << i * 10 << "%)..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }   
    std::cout << "Download complete: " << file << std::endl;
    return 0;
}

int main() {
    std::thread th1(download, "th1SayHello.zip");
    th1.detach();


    std::thread th2([] {
        download("th2SayHello.zip");
    });
    th2.join();

    std::this_thread::sleep_for(std::chrono::milliseconds(400));

    std::future<int> ft = std::async(download, "asyncHello.zip");

    // check future 
    if (ft.wait_for(std::chrono::seconds(1)) == std::future_status::ready) {  
        std::cout << "Result is ready: " << ft.get() << std::endl;  
    } else {  
        std::cout << "Result is not ready yet." << std::endl;  
    }
    std::cout << "===> ft.get() : " << ft.get() << std::endl;
    return 0;
}