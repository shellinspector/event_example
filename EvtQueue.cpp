
#include <iostream>
#include <ostream>
#include <sstream>
#include <memory> // std::shared_ptr
#include <chrono>
#include <stdexcept> // std::runtime_error
#include "EvtQueue.hpp"

using namespace bal;
using namespace std;

#define INTERVAL_SLEEP_MS 2000

/**
* Constructor
*/
EvtQueue::EvtQueue() : _stopEventQueue(true)
{
}

/**
* Launch a thread that will service the queue
*/
int EvtQueue::LaunchQueue()
{
    _stopEventQueue.store(false);
    _msWait.store(INTERVAL_SLEEP_MS);
    _th = std::thread(&EvtQueue::ThreadRunner, this);
    //_th.detach();
    return 0;
}

/**
* Sets queue thread to exit
*/
int EvtQueue::StopQueueAndWait()
{
    _stopEventQueue.store(true);
    _th.join();
    return 0;
}

/**
*  Thread runner that will service events from the queue

*/
void EvtQueue::ThreadRunner()
{
    uint64_t eventCount = 0;
    for (;;)
    {
        std::cout<<"inside the thread loop"<<endl;

        while (!_equeue.waitFor(std::chrono::milliseconds(1000)))
        {
            cout<<","<<flush;
        }
        if (_stopEventQueue.load())
        {
            break; // jump out, thread will finish
        }
        cout<<"processing event on queue"<<endl;

        _equeue.processOne();
    }
}

/**
* Append listener callback to be called on an event on a queue
* Listener is of IQueueEventListener interface
*/
int EvtQueue::AppendListener(std::vector<EventType> &evTypes, IQueueEventListner &listener)
{
    for (auto &eventType : evTypes)
    {
        _equeue.appendListener(eventType, [&](EventType type, const std::string &str, std::shared_ptr<GeneralArgs> args) {
            listener.reactEventFn(eventType, str, args);
        });
    }
    return 0;
}

/** 
* Put an event on event queue.
*
* If no argsPtr is supplied, the evType has to support missing argsPtr
* @param evType event type
* @param textId event Id 
* @param argsPtr communication package pointer
*/
int EvtQueue::PutOnQueue(EventType evType, const std::string &textId, std::shared_ptr<GeneralArgs> argsPtr)
{
    try
    {
        // add only if consumer thread is running
        if (!_stopEventQueue.load())
        {
            _equeue.enqueue(evType, textId, argsPtr);
            return 0;
        }
        else
        {
            return 1;
        }
    }
    catch (std::exception e)
    {
        throw std::runtime_error(std::string("Error: ") +
                                 typeid(e).name() + ": " + e.what());
    }
}
