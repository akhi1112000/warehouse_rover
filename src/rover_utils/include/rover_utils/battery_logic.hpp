#pragma once
#include <iostream>
#include <string>

class BatteryLogic
{
    public:

    BatteryLogic(double low ,double avg ,double high) : low_(low),avg_(avg),high_(high)
    {}

    std::string decide(double charge_p, bool is_charging) const
    {
        if(charge_p <low_ && !is_charging )
        {
            return "EMERGENCY_STOP";
        }

       else if(charge_p < avg_ && !is_charging)
        {

            return "RETURN_TO_DOCK";
        }

        else if(charge_p > high_ && is_charging)
        {
            return "DOCK_RELEASE";
        }
        else
        {
            return "";
        }
    }


    private:

   
    double low_;
    double avg_;
    double high_;
};