#include <iostream>
using namespace std;


class Hardware
{
    public:
    virtual void read() = 0;
    virtual void write() = 0;


};


class GazeboHardware :public Hardware
{
    public:
    void read() override
    {
        cout<<"Gazebo: reading wheel position"<<endl;
    }

    void write() override
    {
        cout<<"Gazebo: sending velocity"<<endl;
    }

};


class RealHardware : public Hardware
{

public:
    void read() override
    {
        cout<<"Real: reading encoder"<<endl;
    }

    void write() override
    {
        cout<<"Real: sending to motor driver"<<endl;
    }
};

void run_loop(Hardware& hw)
{
    hw.read();
    hw.write();
}

int main()
{
    GazeboHardware H ;
    run_loop(H);
    RealHardware R;
    run_loop(R);
}