/**
Demo project for asynchronous event dispatching using event queue. Once run, it asks user
to enter a string, then packs this string into an event arguments and posts on event queue.
The events on the queue are serviced asynchrounsly by a separate worker thread.
Servicing an event means that a subscriber's call back functions are called with the event
data as arguments.
*/



#include "eventpp/eventqueue.h"
#include <iostream>
#include <vector>
#include <chrono>
#include "EvtQueue.hpp"



class Handler: public bal::IQueueEventListner
{
    void reactEventFn(bal::EventType evType, const std::string &textId, std::shared_ptr<bal::GeneralArgs> argsPtr)
	{
		std::cout << std::boolalpha << "Got event: " << evType << " textId " << textId << std::endl;
    }
};

    
int main () {
    
//    std::shared_ptr<>bal::EvtQueue queue;
	std::shared_ptr<bal::EvtQueue> queue = std::make_shared<bal::EvtQueue>();
    Handler handler;
    std::vector<bal::EventType> allEvtTypes;
	for (int i = bal::EventType::eventMSStatus; i <= bal::EventType::eventOther; i++)
    {
       allEvtTypes.push_back(static_cast<bal::EventType>(i));
    }
    queue->AppendListener(allEvtTypes, handler);
	queue->LaunchQueue();

    std::string inputstr{};
    while( inputstr!="quit")
    {
        std::cout<< "Enter new command:"<<std::endl;
        std::getline(std::cin,inputstr);
        std::cout<< "Entered "<<inputstr <<std::endl;
        std::shared_ptr<bal::GeneralArgs> args = std::make_shared<bal::GeneralArgs>(bal::UCommandID::GetData);
		queue->PutOnQueue(bal::EventType::eventMSStatus, inputstr, args);

    }
	queue->StopQueueAndWait();
    /*eventpp::EventQueue<int, void (const std::string &, const bool)> queue;

    queue.appendListener(3, [](const std::string s, bool b) {
                std::cout << std::boolalpha << "Got event 3, s is " << s << " b is " << b << std::endl;
                });
    queue.appendListener(5, [](const std::string s, bool b) {
                std::cout << std::boolalpha << "Got event 5, s is " << s << " b is " << b << std::endl;
                });

    // The listeners are not triggered during enqueue.
    queue.enqueue(3, "Hello", true);
    queue.enqueue(5, "World", false);
   
    // Process the event queue, dispatch all queued events.

    queue.process();
    auto microsecondsUTC1 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    while(!queue.waitFor(std::chrono::milliseconds(20))){
        std::cout<<"."<<std::flush;

    }

    std::cout<<std::endl;
    auto microsecondsUTC2 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "timediff="<<microsecondsUTC2-microsecondsUTC1<<std::endl;
    */
} 

