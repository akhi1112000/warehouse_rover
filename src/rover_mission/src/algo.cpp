#include <future>
#include <iostream>
#include <thread>
#include <chrono>


using namespace std;

int slowAdd(int a, int b)
{
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return a+b;
}

int main()
{

        std::future<int> value = std::async(std::launch::async ,slowAdd,1,5 );



        auto statu = value.wait_for(std::chrono::seconds(100));
        

        if (statu == std::future_status::ready)
    cout << "ready" << endl;
else
    cout << "timeout" << endl;


        // cout<<boolalpha<< value.valid()<<endl;

        // cout<<value.get()<<endl;
        

        // cout<<boolalpha<<value.valid()<<endl;

        // std::this_thread::sleep_for(std::chrono::seconds(2));

        
}