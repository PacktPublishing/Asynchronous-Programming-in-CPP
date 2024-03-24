#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

class JthreadWrapper {
   public:
    JthreadWrapper(const std::function<void(const std::string&)>& func, const std::string& str)
        : t(func, str), name(str) {
            std::ostringstream oss;
            oss << "Thread " << name << " being created" << std::endl;
            std::cout << oss.str();
        }

    ~JthreadWrapper() { 
        std::ostringstream oss;
        oss << "Thread " << name << " being destroyed" << std::endl; 
        std::cout << oss.str();
    }

   private:
    std::jthread t;
    std::string name;
};

void func(const std::string& name) {
    std::stringstream oss;
    oss << "Thread " << name << " starting..." << std::endl;
    std::cout << oss.str();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    oss.clear();    
    oss << "Thread " << name << " finishing..." << std::endl;
    std::cout << oss.str();
}

int main() {
    JthreadWrapper t1(func, "t1");
    JthreadWrapper t2(func, "t2");
    JthreadWrapper t3(func, "t3");

    std::this_thread::sleep_for(std::chrono::seconds(4));

    // t1, t2, t3 will be destroyed when main exits
    std::cout << "Main thread exiting..." << std::endl;

    return 0;
}
