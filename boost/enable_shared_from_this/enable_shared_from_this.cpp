#include <iostream>
#include <memory>

class aaa : public std::enable_shared_from_this<aaa>
{
public:
    void func() {
        auto sptr_ = shared_from_this(); //this -> shared_ptr
        std::cout << "sptr_: " <<  &*sptr_ << std::endl;
        std::cout << "this : " <<  this << std::endl;
    }
};

int main() {
    auto ptr = std::make_shared<aaa>();
    ptr->func();
    return 0;
}
