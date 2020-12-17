/*
 * Filename:   EvtQueue.hpp
 * Created on: Dec 1, 2020
 * Author:     idq
 *
 * Copyright (C) 2020 id Quantique SA. All Rights Reserved.
 *
 * THIS COMPUTER PROGRAM IS PROPRIETARY AND CONFIDENTIAL TO ID QUANTIQUE SA
 * AND ITS LICENSORS AND CONTAINS TRADE SECRETS OF ID QUANTIQUE SA THAT ARE
 * PROVIDED PURSUANT TO A WRITTEN AGREEMENT CONTAINING RESTRICTIONS ON USE
 * AND DISCLOSURE. ANY USE, REPRODUCTION, OR TRANSFER EXCEPT AS PROVIDED IN
 * SUCH AGREEMENT IS STRICTLY PROHIBITED.
 */

#ifndef ELVIS_CONTROLLINK_BAL_EVTQUEUE_HPP_
#define ELVIS_CONTROLLINK_BAL_EVTQUEUE_HPP_

#include <thread>
#include <vector>
#include <eventpp/eventqueue.h>

namespace bal
{
	typedef enum ULoggingType
	{
		UINFO,
		UDEBUG,
		UWARN,
		UERROR,
		UFATAL
	} ULoggingType;

	typedef enum UCommandID
	{
		Init = 100,
		Calibrate,
		Shutdown,
		Reset,
		GetData,
		SetData,
		StatusUpdate,
		ActionA,
		ActionB,
		ActionC,
		Max_unknown
	} UCommandID;

	typedef enum
	{
		eventMSStatus,
		eventMSData,
		eventMSAction,
		eventOther,
	} EventType;


	class GeneralArgs
	{
	public:
		/** 
		* Single argument constructor
		*/
		GeneralArgs(UCommandID cid) : CommandID(cid), ClientIndex(0){

													  };
		/** 
		* Principal command category
		*/
		UCommandID CommandID{UCommandID::Max_unknown};
		/** 
		* If dealing with multiple clients addressing BAL this
		* identifier migh become useful
		*/
		int32_t ClientIndex{-1};
		/** 
		* Human readible client identifier
		*/
		std::string ClientObjectTypeName{};
		/** Error flag use to tag events signaling error
		* f.ex. returned if a variable with given name does not exist
		*/
		ULoggingType LoggingType{ULoggingType::UINFO};
		/** Message giving additional context to an event. 
		* f. ex. explaing the error flag cause
		* This is useful to propogate exceptions up to client */
		std::string ReturnMessage{};
	};
    class IQueueEventListner
    {
        public:
        virtual void reactEventFn(EventType evType, const std::string &textId, std::shared_ptr<bal::GeneralArgs> argsPtr) = 0;
        virtual ~IQueueEventListner() = 0;
    };
    inline IQueueEventListner::~IQueueEventListner() {}

	class EvtQueue
	{
		/**
	* Unused but may simplify the event queue definition (TBD)
	*/
	public:
		typedef struct
		{
			int eventId;
			const std::string &text;
			std::shared_ptr<GeneralArgs> args;
		} Event;

		EvtQueue();
		int LaunchQueue();
		int StopQueueAndWait();
		int PutOnQueue(EventType evType, const std::string &textId, std::shared_ptr<GeneralArgs> argsPtr = nullptr);
		//int AppendListener(EventType, eventCallback callback);
		int AppendListener(std::vector<EventType> &evTypes, IQueueEventListner &listener);

	private:
		using RawQueueType = eventpp::EventQueue<EventType, void(EventType, const std::string &, std::shared_ptr<GeneralArgs>)>;
		std::thread _th;
		RawQueueType _equeue;
		void ThreadRunner();
		std::atomic<bool> _stopEventQueue;
		std::atomic<int> _msWait;
	};

} // namespace bal
#endif
