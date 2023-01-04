//#include <tinyfsm.hpp>
//
//#include "guard.hpp"
//#include "fsmlist.hpp"
//
//#include <iostream>
//
//class Idle; // forward declaration
//
//// ----------------------------------------------------------------------------
//// Transition functions
////
//
//static void CallMaintenance() {
//    std::cout << "*** calling maintenance ***" << std::endl;
//}
//
//static void CallFirefighters() {
//    std::cout << "*** calling firefighters ***" << std::endl;
//}
//
//
//// ----------------------------------------------------------------------------
//// State: Moving
////
//
//class Patrolling : public GuardFSM {
//    void react(FloorSensor const & e) override {
//        int floor_expected = current_floor + Motor::getDirection();
//        if(floor_expected != e.floor)
//        {
//            std::cout << "Floor sensor defect (expected " << floor_expected << ", got " << e.floor << ")" << std::endl;
//            transit<Panic>(CallMaintenance);
//        }
//        else
//        {
//            std::cout << "Reached floor " << e.floor << std::endl;
//            current_floor = e.floor;
//            if(e.floor == dest_floor)
//                transit<Idle>();
//        }
//    };
//};
//
//
//// ----------------------------------------------------------------------------
//// State: Idle
////
//
//class Idle : public GuardFSM {
//    void entry() override {
//        send_event(MotorStop());
//    }
//
//    void react(EyeContact const & e) override {
//        dest_floor = e.floor;
//
//        if(dest_floor == current_floor)
//            return;
//
//        /* lambda function used for transition action */
//        auto action = [] {
//            if(dest_floor > current_floor)
//                send_event(MotorUp());
//            else if(dest_floor < current_floor)
//                send_event(MotorDown());
//        };
//
//        transit<Moving>(action);
//    };
//};
//
//// ----------------------------------------------------------------------------
//// State: Chase
////
//
//class Chase : public GuardFSM {
//    void entry() override {
//        send_event(MotorStop());
//    }
//
//    void react(EyeContact const & e) override {
//        dest_floor = e.floor;
//
//        if(dest_floor == current_floor)
//            return;
//
//        /* lambda function used for transition action */
//        auto action = [] {
//            if(dest_floor > current_floor)
//                send_event(MotorUp());
//            else if(dest_floor < current_floor)
//                send_event(MotorDown());
//        };
//
//        transit<Moving>(action);
//    };
//};
//
//
//// ----------------------------------------------------------------------------
//// State: Arrest
////
//
//class Arrest : public GuardFSM {
//    void entry() override {
//        send_event(MotorStop());
//    }
//
//    void react(EyeContact const & e) override {
//        dest_floor = e.floor;
//
//        if(dest_floor == current_floor)
//            return;
//
//        /* lambda function used for transition action */
//        auto action = [] {
//            if(dest_floor > current_floor)
//                send_event(MotorUp());
//            else if(dest_floor < current_floor)
//                send_event(MotorDown());
//        };
//
//        transit<Moving>(action);
//    };
//};
//
//
//// ----------------------------------------------------------------------------
//// State: Examination
////
//
//class Examination : public GuardFSM {
//    void entry() override {
//        send_event(MotorStop());
//    }
//
//    void react(EyeContact const & e) override {
//        dest_floor = e.floor;
//
//        if(dest_floor == current_floor)
//            return;
//
//        /* lambda function used for transition action */
//        auto action = [] {
//            if(dest_floor > current_floor)
//                send_event(MotorUp());
//            else if(dest_floor < current_floor)
//                send_event(MotorDown());
//        };
//
//        transit<Moving>(action);
//    };
//};
//
//// ----------------------------------------------------------------------------
//// Base state: default implementations
////
//
//void GuardFSM::react(EyeContact const &) {
//    std::cout << "Eye contact!" << std::endl;
//}
//
//FSM_INITIAL_STATE(GuardFSM, Idle)