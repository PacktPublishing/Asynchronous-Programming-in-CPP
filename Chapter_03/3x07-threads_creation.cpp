#include <iostream>
#include <syncstream>
#include <thread>

#define sync_cout std::osyncstream(std::cout)

void func() {
    sync_cout << "Using function pointer" << std::endl;
}

class FuncObjectClass {
   public:
    void operator()() {
        sync_cout << "Using function object class" << std::endl;
    }
};

int main() {
    // Start thread 1 using a function pointer.
    std::thread t1(func);

    // Start thread 2 using a lambda function
    auto lambda_func = []() {
        sync_cout << "Using lambda function" << std::endl;
    };
    std::thread t2(lambda_func);

    // Start thread 3 using an embedded lambda function
    std::thread t3([]() {
        sync_cout << "Using embedded lambda function" << std::endl;
    });

    // Start thread 4 using a function object (overloading operator() )
    // FuncObjectClass obj1;
    std::thread t4{FuncObjectClass()};

    // Start thread 5 using a non-static member function
    class Obj {
       public:
        void func() {
            sync_cout << "Using a non-static member function" << std::endl;
        }
        static void static_func() {
            sync_cout << "Using a static member function" << std::endl;
        }
    };

    Obj obj2;

    // In this case, the first argument is the reference to the function
    // and second is the object reference.
    std::thread t5(&Obj::func, &obj2);

    // Start thread 6 using a static member function. Only the first argument is needed.
    std::thread t6(&Obj::static_func);

    // Wait threads to finish
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    return 0;
}