/*
 *   Copyright 2009 The Portico Project
 *
 *   This file is part of portico.
 *
 *   portico is free software; you can redistribute it and/or modify
 *   it under the terms of the Common Developer and Distribution License (CDDL)
 *   as published by Sun Microsystems. For more information see the LICENSE file.
 *
 *   Use of this software is strictly AT YOUR OWN RISK!!!
 *   If something bad happens you do not have permission to come crying to me.
 *   (that goes for your lawyer as well)
 *
 */
#ifndef JAVARTI_H_
#define JAVARTI_H_

#include "common.h"
#include "utils/Logger.h"
#include "utils/ExceptionHacks.h"

//#include <fedtime.hh>
//#include "../types/ExceptionHacks.h"
//#include "../types/RegionImpl.h"

PORTICO13_NS_START

// this struct will hold handle value pair-set data
struct HVPS
{
	jintArray    handles;
	jobjectArray values;
};

class JavaRTI
{
	//----------------------------------------------------------
	//                    STATIC VARIABLES
	//----------------------------------------------------------

	//----------------------------------------------------------
	//                   INSTANCE VARIABLES
	//----------------------------------------------------------
	public:
		static int rtiCounter;

		int     id;
		Logger  *logger;
		jclass  jproxyClass;
		jclass  jregionClass;
		jobject jproxy;

		HLA::FederateAmbassador *fedamb; // the user FederateAmbassador implementation

		// Method ID Cache Variables are listed below //

	private:
		JavaVM *jvm;
		// exception information
		char *eName;
		char *eReason;

	//----------------------------------------------------------
	//                      CONSTRUCTORS
	//----------------------------------------------------------
	public:
		JavaRTI( JavaVM *jvm );
		virtual ~JavaRTI();

	//----------------------------------------------------------
	//                    INSTANCE METHODS
	//----------------------------------------------------------
	public:
		int          getId();
		void         exceptionCheck();
		jbyteArray   convertTag( const char *tag );
		jintArray    convertAHS( const HLA::AttributeHandleSet& ahs );
		jintArray    convertAHA( HLA::AttributeHandle array[], HLA::ULong size );
		jintArray    convertFHS( const HLA::FederateHandleSet& fhs );
		jdouble      convertTime( const HLA::FedTime& time );
		void         pushTime( jdouble time, HLA::FedTime& fedtime );
		char*        convertAndReleaseJString( jstring string );
		char*        convertJTag( jbyteArray tag );
		jobject      convertRegion( const HLA::Region& region );
		jobjectArray convertRegions( HLA::Region *theRegions[], HLA::ULong size );

		HVPS convertAHVPS( const HLA::AttributeHandleValuePairSet& attributes )
			throw( HLA::RTIinternalError );
		HVPS convertPHVPS( const HLA::ParameterHandleValuePairSet& parameters )
			throw( HLA::RTIinternalError );

		void pushException( char *exceptionName, char *reason );

		long currentTimeMillis();
		
		// Attach current thread to JVM if it isn't, return a usable JNIEnv
		JNIEnv* getJniEnvironment() throw( HLA::RTIinternalError );

		// methods to convert to HLA types from Java types
		HLA::AttributeHandleSet* convertToAHS( jintArray array );
	private:
		JNIEnv* attachToJVM() throw( HLA::RTIinternalError );
		void initialize() throw( HLA::RTIinternalError );
		void detachFromJVM(); /* should not be called currently */
		void cacheMethodIds() throw( HLA::RTIinternalError );
		void cacheMethod( jmethodID *handle, const char *method, const char *signature )
			throw( HLA::RTIinternalError );
		void cacheMethod( jmethodID *handle, jclass clazz, const char *method, const char *signature )
			throw( HLA::RTIinternalError );

	//----------------------------------------------------------
	//                     STATIC METHODS
	//----------------------------------------------------------

	//----------------------------------------------------------
	// Method ID Cache Variables
	//----------------------------------------------------------
	public: // public for easy accessibility
		jclass BYTE_ARRAY;

		// federation management
		jmethodID CREATE_FEDERATION;
		jmethodID DESTROY_FEDERATION;
		jmethodID JOIN_FEDERATION;
		jmethodID RESIGN_FEDERATION;
		jmethodID REGISTER_FEDERATION_SYNCH;
		jmethodID REGISTER_FEDERATION_SYNCH_FEDHANDLESET;
		jmethodID SYNCH_POINT_ACHIEVED;
		jmethodID REQUEST_FEDERATION_SAVE_TIME;
		jmethodID REQUEST_FEDERATION_SAVE;
		jmethodID FEDERATE_SAVE_BEGUN;
		jmethodID FEDERATE_SAVE_COMPLETE;
		jmethodID FEDERATE_SAVE_NOT_COMPLETE;
		jmethodID REQUEST_FEDERATION_RESTORE;
		jmethodID FEDERATE_RESTORE_COMPLETE;
		jmethodID FEDERATE_RESTORE_NOT_COMPLETE;

		// publication and subscription
		jmethodID PUBLISH_OBJECT_CLASS;
		jmethodID UNPUBLISH_OBJECT_CLASS;
		jmethodID PUBLISH_INTERACTION_CLASS;
		jmethodID UNPUBLISH_INTERACTION_CLASS;
		jmethodID SUBSCRIBE_OBJECT_CLASS_ATTRIBUTES_ACTIVELY;
		jmethodID SUBSCRIBE_OBJECT_CLASS_ATTRIBUTES_PASSIVELY;
		jmethodID UNSUBSCRIBE_OBJECT_CLASS;
		jmethodID SUBSCRIBE_INTERACTION_CLASS;
		jmethodID SUBSCRIBE_INTERACTION_CLASS_PASSIVELY;
		jmethodID UNSUBSCRIBE_INTERACTION_CLASS;

		// object management
		jmethodID REGISTER_OBJECT_INSTANCE;
		jmethodID REGISTER_OBJECT_INSTANCE_WITH_NAME;
		jmethodID UPDATE_ATTRIBUTE_VALUES;
		jmethodID UPDATE_ATTRIBUTE_VALUES_WITH_TIME;
		jmethodID SEND_INTERACTION;
		jmethodID SEND_INTERACTION_WITH_TIME;
		jmethodID DELETE_OBJECT_INSTANCE;
		jmethodID DELETE_OBJECT_INSTANCE_WITH_TIME;
		jmethodID LOCAL_DELETE_OBJECT_INSTANCE;
		jmethodID CHANGE_ATTRIBUTE_TRANSPORTATION_TYPE;
		jmethodID CHANGE_INTERACTION_TRANSPORTATION_TYPE;
		jmethodID REQUEST_OBJECT_ATTRIBUTE_VALUE_UPDATE;
		jmethodID REQUEST_CLASS_ATTRIBUTE_VALUE_UPDATE;

		// ownership management
		jmethodID UNCONDITIONAL_DIVEST;
		jmethodID NEGOTIATED_DIVEST;
		jmethodID ATTRIBUTE_ACQUISITION;
		jmethodID ATTRIBUTE_ACQUISITION_AVAILABLE;
		jmethodID ATTRIBUTE_OWNERSHIP_RELEASE_RESPOSE;
		jmethodID CANCEL_NEGOTIATED_DIVEST;
		jmethodID CANCEL_OWNERSHIP_ACQUISITION;
		jmethodID QUERY_ATTRIBUTE_OWNERSHIP;
		jmethodID IS_ATTRIBUTE_OWNED_BY_FEDERATE;

		// time management
		jmethodID ENABLE_TIME_REGULATION;
		jmethodID DISABLE_TIME_REGULATION;
		jmethodID ENABLE_TIME_CONSTRAINED;
		jmethodID DISABLE_TIME_CONSTRAINED;
		jmethodID TIME_ADVANCE_REQUEST;
		jmethodID TIME_ADVANCE_REQUEST_AVAILABLE;
		jmethodID NEXT_EVENT_REQUEST;
		jmethodID NEXT_EVENT_REQUEST_AVAILABLE;
		jmethodID FLUSH_QUEUE_REQUEST;
		jmethodID ENABLE_ASYNCHRONOUS_DELIVERY;
		jmethodID DISABLE_ASYNCHRONOUS_DELIVERY;
		jmethodID QUERY_LBTS;
		jmethodID QUERY_FEDERATE_TIME;
		jmethodID QUERY_MIN_NEXT_EVENT_TIME;
		jmethodID MODIFY_LOOKAHEAD;
		jmethodID QUERY_LOOKAHEAD;
		jmethodID RETRACT;
		jmethodID CHANGE_ATTRIBUTE_ORDER_TYPE;
		jmethodID CHANGE_INTERACTION_ORDER_TYPE;

		// data distribution management
		jmethodID GET_REGION;
		jmethodID GET_REGION_TOKEN;
		jmethodID CREATE_REGION;
		jmethodID NOTIFY_OF_REGION_MODIFICATION;
		jmethodID DELETE_REGION;
		jmethodID REGISTER_OBJECT_WITH_REGION;
		jmethodID REGISTER_OBJECT_WITH_NAME_AND_REGION;
		jmethodID ASSOCIATE_REGION_FOR_UPDATES;
		jmethodID UNASSOCIATE_REGION_FOR_UPDATES;
		jmethodID SUBSCRIBE_ATTRIBUTES_WITH_REGION;
		jmethodID SUBSCRIBE_ATTRIBUTES_PASSIVELY_WITH_REGION;
		jmethodID UNSUBSCRIBE_ATTRIBUTES_WITH_REGION;
		jmethodID SUBSCRIBE_INTERACTION_CLASS_WITH_REGION;
		jmethodID SUBSCRIBE_INTERACTION_CLASS_PASSIVELY_WITH_REGION;
		jmethodID UNSUBSCRIBE_INTERACTION_CLASS_WITH_REGION;
		jmethodID SEND_INTERACTION_WITH_REGION;
		jmethodID SEND_INTERACTION_WITH_TIME_AND_REGION;
		jmethodID REQUEST_CLASS_ATTRIBUTE_VALUE_UPDATE_WITH_REGION;

		// support services
		jmethodID GET_OBJECT_CLASS_HANDLE;
		jmethodID GET_OBJECT_CLASS_NAME;
		jmethodID GET_ATTRIBUTE_HANDLE;
		jmethodID GET_ATTRIBUTE_NAME;
		jmethodID GET_INTERACTION_CLASS_HANDLE;
		jmethodID GET_INTERACTION_CLASS_NAME;
		jmethodID GET_PARAMETER_HANDLE;
		jmethodID GET_PARAMETER_NAME;
		jmethodID GET_OBJECT_INSTANCE_HANDLE;
		jmethodID GET_OBJECT_INSTANCE_NAME;
		jmethodID GET_ROUTING_SPACE_HANDLE;
		jmethodID GET_ROUTING_SPACE_NAME;
		jmethodID GET_DIMENSION_HANDLE;
		jmethodID GET_DIMENSION_NAME;
		jmethodID GET_ATTRIBUTE_ROUTING_SPACE_HANDLE;
		jmethodID GET_OBJECT_CLASS;
		jmethodID GET_INTERACTION_ROUTING_SPACE_HANDLE;
		jmethodID GET_TRANSPORTATION_HANDLE;
		jmethodID GET_TRANSPORTATION_NAME;
		jmethodID GET_ORDERING_HANDLE;
		jmethodID GET_ORDERING_NAME;
		jmethodID ENABLE_CLASS_RELEVANCE_ADVISORY_SWITCH;
		jmethodID DISABLE_CLASS_RELEVANCE_ADVISORY_SWITCH;
		jmethodID ENABLE_ATTRIBUTE_RELEVANCE_ADVISORY_SWITCH;
		jmethodID DISABLE_ATTRIBUTE_RELEVANCE_ADVISORY_SWITCH;
		jmethodID ENABLE_ATTRIBUTE_SCOPE_ADVISORY_SWITCH;
		jmethodID DISABLE_ATTRIBUTE_SCOPE_ADVISORY_SWITCH;
		jmethodID ENABLE_INTERACTION_RELEVANCE_ADVISORY_SWITCH;
		jmethodID DISABLE_INTERACTION_RELEVANCE_ADVISORY_SWITCH;
		jmethodID TICK;
		jmethodID TICK_WITH_TIME;
		jmethodID KILL;

		// Region class methods
		jmethodID REGION_GET_NUMBER_OF_EXTENTS;
		jmethodID REGION_GET_RANGE_LOWER_BOUND;
		jmethodID REGION_GET_RANGE_UPPER_BOUND;
		jmethodID REGION_GET_SPACE_HANDLE;
		jmethodID REGION_SET_RANGE_LOWER_BOUND;
		jmethodID REGION_SET_RANGE_UPPER_BOUND;
		jmethodID REGION_GET_RANGE_UPPER_BOUND_NOTIFICATION_LIMIT;
		jmethodID REGION_GET_RANGE_LOWER_BOUND_NOTIFICATION_LIMIT;
};

PORTICO13_NS_END

#endif /*JAVARTI_H_*/
