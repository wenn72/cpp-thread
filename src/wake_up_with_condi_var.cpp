#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <memory>


class Worker {
public:
    Worker() = default;
    Worker(int num) : number(num), mtx(), cv() {
    };

    ~Worker() {
        if (awakeThread.joinable()) {
            awakeThread.join();
        }
        if (waitThread.joinable()) {
            waitThread.join();
        }
        std::cout << " ~ Worker : " << number << std::endl;
    };

    int number = 0;
    std::mutex mtx;
    std::condition_variable cv;
    std::thread awakeThread;
    std::thread waitThread;

    void RunAwake() {
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 模拟一些耗时操作
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "Worker : " << number << " is ready!" << std::endl;
        }
        cv.notify_all(); // Unblocks all of the threads currently waiting for this condition.
    }

    void Awake() {
        awakeThread = std::thread(&Worker::RunAwake, this);
    }

    void Wait(int seconds) {
        waitThread = std::thread(&Worker::RunWait, this, seconds);
    }

    void RunWait(int seconds) {
        std::unique_lock<std::mutex> lock(mtx);
        if (cv.wait_for(lock, std::chrono::seconds(seconds)) == std::cv_status::timeout) {
            std::cout << "Worker : " << number  << " Timeout occurred!" << std::endl;
        } else {
            std::cout << "Worker : " << number  << " Received data!" << std::endl;
        }
    }
};

int main() {
    int num = 5;
    std::vector<std::unique_ptr<Worker>> workers;
    workers.resize(num);
    
    for (int i = 0; i < num; i++) {
        workers.emplace_back(std::make_unique<Worker>(i)); // emplace_back support constructing in place
        workers.back()->Wait(2 * i + 1);
    }

    workers.back()->Awake();
    return 0;
}
