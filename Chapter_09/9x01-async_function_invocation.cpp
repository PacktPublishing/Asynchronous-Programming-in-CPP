#include <future>
#include <iostream>

void func() { 
    std::cout << "Using function pointer" << std::endl; 
}

class FuncObjectClass {
   public:
    void operator()() { 
        std::cout << "Using function object class" << std::endl; 
    }
};

int main() {
    // Run function asynchronously from a function pointer.
    auto fut1 = std::async(func);

    // Run function asynchronously using a lambda function
    auto lambda_func = []() { 
        std::cout << "Using lambda function\n"; 
    };
    auto fut2 = std::async(lambda_func);

    // Run function asynchronously using an embedded lambda function
    auto fut3 = std::async([]() { 
        std::cout << "Using embedded lambda function\n"; 
    });

    // Run function asynchronously using a function object (overloading operator() )
    // FuncObjectClass obj1;
    auto fut4 = std::async(FuncObjectClass());

    // Run function asynchronously using a non-static member function
    class Obj {
       public:
        void func() { 
            std::cout << "Using a non-static member function\n"; 
        }
        static void static_func() { 
            std::cout << "Using a static member function\n"; 
        }
    };

    Obj obj2;

    // In this case, the first argument is the reference to the function
    // and second is the object reference.
    auto fut5 = std::async(&Obj::func, &obj2);

    // Run function asynchronously using a static member function. Only the first argument is needed.
    auto fut6 = std::async(&Obj::static_func);

    // Wait for async functions to finish.
    fut1.get();
    fut2.get();
    fut3.get();
    fut4.get();
    fut5.get();
    fut6.get();
    return 0;
}