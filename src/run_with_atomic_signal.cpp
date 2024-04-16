#include <iostream>
#include <thread>
#include <string>

using namespace std;
class Demo {
public:
    Demo() = default;
    ~Demo(){
        active.store(false); // finish executing
        if (mainThread.joinable()) {
            mainThread.join();
        }
        std::cout << " ~Demo() " << std::endl;
    };

    void Start() {
        std::cout << " Demo start " << std::endl;
        active.store(true);
        auto lambda = [&] {
            int count = 1;
            while(active.load()) {
                std::cout << " thread is working, count: " << count << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                count++;
            }
            std::cout << " thread exit" << std::endl;
        };
        mainThread = std::thread(lambda);
    }

    void Stop() {
        active.store(false);  // finish executing
        if (mainThread.joinable()) {
            mainThread.join();
        }
    }

private:
    atomic_bool active; // as a sync signal
    std::thread mainThread;
};


void testDestroyDemo() {
    Demo demo;
    demo.Start();
    std::cout << " Start demon in test success " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
}

int main() {
    testDestroyDemo();
    Demo demo;
    demo.Start();
    std::cout << " Main Start demon success " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(11));
    demo.Stop();
    std::cout << " Main Stop demon success " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}