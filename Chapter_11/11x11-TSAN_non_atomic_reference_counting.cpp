#include <iostream>
#include <thread>

class RefCountedObject {
   public:
    
    void Ref() {      
        ++ref_;
    }

    void Unref() {
        --ref_;
    }

   private:
    // ref_ should be atomic to avoid synchronization issues
    int ref_{0};
};

int main() {
  RefCountedObject obj1;
  std::jthread t1(&RefCountedObject::Ref, &obj1);
  std::jthread t2(&RefCountedObject::Unref, &obj1);
  return 0;
}
