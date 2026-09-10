#include <gtest/gtest.h>

#include "rover_utils/battery_logic.hpp"



TEST(BatteryLogicTest,EmergencyWhenCritical)
{

BatteryLogic logic(5.0, 20.0, 90.0);

EXPECT_EQ(logic.decide(3.0, false), "EMERGENCY_STOP");

}

TEST(BatteryLogicTest,DOCK_RELEASE_FULL_CHARGE)
{
BatteryLogic logic(5.0, 20.0, 90.0);

EXPECT_EQ(logic.decide(92, true), "DOCK_RELEASE");
}

TEST(BatteryLogicTest,RETURN_TO_DOCK_NEED_TO_CHARGE)
{

BatteryLogic logic(5.0, 20.0, 90.0);

EXPECT_EQ(logic.decide(15, false), "RETURN_TO_DOCK");
}

TEST(BatteryLogicTest,CHECK_CHARGING)
{

BatteryLogic logic(5.0, 20.0, 90.0);

EXPECT_EQ(logic.decide(3, true), "");
}

TEST(BatteryLogicTest,CHECK_WORKING)
{

BatteryLogic logic(5.0, 20.0, 90.0);

EXPECT_EQ(logic.decide(50, false), "");
}