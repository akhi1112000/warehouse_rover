#include <iostream>
#include <string>

class Base
{
public:
    virtual void on_init(std::string info)
    {
        info_ = info;
        std::cout << "Base saved: " << info_ << std::endl;
    }
protected:
    std::string info_;
};

class Rover : public Base
{
public:

    void on_init(std::string info) override
    {
        Base::on_init(info);
        std::cout << "Rover on_init" << std::endl;
        std::cout << "info_ is: [" << info_ << "]" << std::endl;
    }
};

int main()
{
    Rover r;
    Base & b = r;
    b.on_init("left_wheel_joint");
}