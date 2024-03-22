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
#include "HLA13Common.h"
#include "types/Region.h"

using namespace portico13;
extern Logger* logger;

RTI::RegionToken RTI::RTIambassador::getRegionToken( RTI::Region *theRegion )
	throw( RTI::FederateNotExecutionMember, 
	       RTI::ConcurrentAccessAttempted,
	       RTI::RegionNotKnown,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] getRegionToken(): regionAddress=%p", theRegion );
	
	// call the method
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	int token = env->CallIntMethod( privateRefs->rti->jproxy,
	                                privateRefs->rti->GET_REGION_TOKEN,
	                                privateRefs->rti->convertRegion(*theRegion) );
	
	// run the exception check
	privateRefs->rti->exceptionCheck();
	
	logger->trace( "[Finished] getRegionToken(): token=%u", token );

	return token;
}

RTI::Region* RTI::RTIambassador::getRegion( RTI::RegionToken token )
	throw( RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::RegionNotKnown,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] getRegion(): token=%u", token );
	
	// call the method
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	jobject proxy = env->CallObjectMethod( privateRefs->rti->jproxy,
	                                       privateRefs->rti->GET_REGION,
	                                       (jint)token );
	
	// run the exception check
	privateRefs->rti->exceptionCheck();

	// create a new region to return
	RTI::Region *region = new portico13::Region( privateRefs->getRti(), &(*proxy) );

	logger->trace( "[Finished] getRegion(): token=%u", token );

	return region;
}

//////////////////////////////////
// Data Distribution Management //
//////////////////////////////////
// 9.2
RTI::Region* RTI::RTIambassador::createRegion( RTI::SpaceHandle space, RTI::ULong extents )
	throw( RTI::SpaceNotDefined,
	       RTI::InvalidExtents,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] createRegion(): spaceHandle=%u, extents=%u", space, extents );
	
	// call the method
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	jobject proxy = env->CallObjectMethod( privateRefs->rti->jproxy,
	                                       privateRefs->rti->CREATE_REGION,
	                                       (jint)space,
	                                       (jint)extents );
	
	// run the exception check
	privateRefs->rti->exceptionCheck();

	// create a new region to return
	RTI::Region *region = new portico13::Region( privateRefs->rti, proxy );
	
	logger->trace( "[Finished] createRegion(): spaceHandle=%u, extents=%u", space, extents );

	return region;
}

// 9.3
void RTI::RTIambassador::notifyAboutRegionModification( RTI::Region &theRegion )
	throw( RTI::RegionNotKnown,
	       RTI::InvalidExtents,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] modifyRegion(): regionAddress=%p", &theRegion );
	
	// call the method
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	env->CallObjectMethod( privateRefs->rti->jproxy,
	                       privateRefs->rti->NOTIFY_OF_REGION_MODIFICATION,
	                       privateRefs->rti->convertRegion(theRegion) );
	
	// run the exception check
	privateRefs->rti->exceptionCheck();
	
	logger->trace( "[Finished] modifyRegion(): regionAddress=%p", &theRegion );
}

// 9.4
void RTI::RTIambassador::deleteRegion( RTI::Region *theRegion )
	throw( RTI::RegionNotKnown,
	       RTI::RegionInUse,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] deleteRegion(): regionAddress=%p", theRegion );
	
	// call the method
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	env->CallObjectMethod( privateRefs->rti->jproxy,
	                       privateRefs->rti->DELETE_REGION,
	                       privateRefs->rti->convertRegion(*theRegion) );
	
	// run the exception check
	privateRefs->rti->exceptionCheck();
	
	logger->trace( "[Finished] deleteRegion(): regionAddress=%p", theRegion );
}

// 9.5
RTI::ObjectHandle
RTI::RTIambassador::registerObjectInstanceWithRegion( RTI::ObjectClassHandle theClass,
                                                      const char *theObject,
                                                      RTI::AttributeHandle theAttributes[],
                                                      RTI::Region *theRegions[],
                                                      RTI::ULong size )
	throw( RTI::ObjectClassNotDefined,
	       RTI::ObjectClassNotPublished,
	       RTI::AttributeNotDefined,
	       RTI::AttributeNotPublished,
	       RTI::RegionNotKnown,
	       RTI::InvalidRegionContext,
	       RTI::ObjectAlreadyRegistered,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	if( logger->isTraceEnabled() )
	{
		char* attributeString = arrayToString( theAttributes, size );
		char* regionString = arrayToString( theRegions, size );
		logger->trace( "[Starting] registerObjectInstanceWithRegion(): classHandle=%d, name=%s, attributes=%s, regions=%s",
		               theClass, theObject, attributeString, regionString );
		delete attributeString;
		delete regionString;
	}

	// get java versions of the parameters
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	jstring jObjectName = env->NewStringUTF( theObject );
	jintArray handles = privateRefs->rti->convertAHA( theAttributes, size );
	jobjectArray regions = privateRefs->rti->convertRegions( theRegions, size );

	// call the method
	// int reg(int, int[], region[])
	jint handle = env->CallIntMethod( privateRefs->rti->jproxy,
	                                  privateRefs->rti->REGISTER_OBJECT_WITH_NAME_AND_REGION,
	                                  theClass,
	                                  jObjectName,
	                                  handles,
	                                  regions );

	env->DeleteLocalRef( jObjectName );
	env->DeleteLocalRef( handles );
	env->DeleteLocalRef( regions );
	// run the exception check
	privateRefs->rti->exceptionCheck();
	
	logger->trace( "[Finished] registerObjectInstanceWithRegion(): objectHandle=%d", handle );
	return handle;
}

RTI::ObjectHandle
RTI::RTIambassador::registerObjectInstanceWithRegion( RTI::ObjectClassHandle theClass,
                                                      RTI::AttributeHandle theAttributes[],
                                                      RTI::Region *theRegions[],
                                                      RTI::ULong size )
	throw( RTI::ObjectClassNotDefined,
	       RTI::ObjectClassNotPublished,
	       RTI::AttributeNotDefined,
	       RTI::AttributeNotPublished,
	       RTI::RegionNotKnown,
	       RTI::InvalidRegionContext,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	if( logger->isTraceEnabled() )
	{
		char* attributeString = arrayToString( theAttributes, size );
		char* regionString = arrayToString( theRegions, size );
		logger->trace( "[Starting] registerObjectInstanceWithRegion(): classHandle=%d, attributes=%s, regions=%s",
		               theClass, attributeString, regionString );
		delete attributeString;
		delete regionString;
	}

	jintArray handles = privateRefs->rti->convertAHA( theAttributes, size );
	jobjectArray regions = privateRefs->rti->convertRegions( theRegions, size );

	// call the method
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	// int reg(int, int[], region[])
	jint handle = env->CallIntMethod( privateRefs->rti->jproxy,
	                                  privateRefs->rti->REGISTER_OBJECT_WITH_REGION,
	                                  theClass,
	                                  handles,
	                                  regions );

	env->DeleteLocalRef( handles );
	env->DeleteLocalRef( regions );
	// run the exception check
	privateRefs->rti->exceptionCheck();
	
	logger->trace( "[Finished] registerObjectInstanceWithRegion(): objectHandle=%d", handle );
	return handle;
}

// 9.6
void RTI::RTIambassador::associateRegionForUpdates( RTI::Region &theRegion,
                                                    RTI::ObjectHandle theObject,
                                                    const RTI::AttributeHandleSet &theAttributes )
	throw( RTI::ObjectNotKnown,
	       RTI::AttributeNotDefined,
	       RTI::InvalidRegionContext,
	       RTI::RegionNotKnown,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	if( logger->isTraceEnabled() )
	{
		char* attributeString = setToString( theAttributes );
		logger->trace( "[Starting] associateRegionForUpdates(): regionAddress=%p, object=%d, attributes=%s",
		               &theRegion, theObject, attributeString );
		delete attributeString;
	}
	
	// convert the parameters to their java equiv
	jintArray jAttributes = privateRefs->rti->convertAHS( theAttributes );
	
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	env->CallVoidMethod( privateRefs->rti->jproxy,
	                     privateRefs->rti->ASSOCIATE_REGION_FOR_UPDATES,
	                     privateRefs->rti->convertRegion(theRegion),
	                     theObject,
	                     jAttributes );
	
	env->DeleteLocalRef( jAttributes );
	// run the exception check
	privateRefs->rti->exceptionCheck();
	
	logger->trace( "[Finished] associateRegionForUpdates(): regionAddress=%p, object=%d",
	               &theRegion, theObject );
}

// 9.7
void RTI::RTIambassador::unassociateRegionForUpdates( RTI::Region &theRegion,
                                                      RTI::ObjectHandle theObject )
	throw( RTI::ObjectNotKnown,
	       RTI::InvalidRegionContext,
	       RTI::RegionNotKnown,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] unassociateRegionForUpdates(): regionAddress=%p, object=%d",
	               &theRegion, theObject );
	
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	env->CallVoidMethod( privateRefs->rti->jproxy,
	                     privateRefs->rti->UNASSOCIATE_REGION_FOR_UPDATES,
	                     privateRefs->rti->convertRegion(theRegion),
	                     theObject );

	// run the exception check
	privateRefs->rti->exceptionCheck();
	
	logger->trace( "[Finished] unassociateRegionForUpdates(): regionAddress=%p, object=%d",
	               &theRegion, theObject );
}

// 9.8
void RTI::RTIambassador::subscribeObjectClassAttributesWithRegion(
		RTI::ObjectClassHandle theClass,
		RTI::Region &theRegion,
		const RTI::AttributeHandleSet &attributeList,
		RTI::Boolean active )
	throw( RTI::ObjectClassNotDefined,
	       RTI::AttributeNotDefined,
	       RTI::RegionNotKnown,
	       RTI::InvalidRegionContext,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	/////////////////////
	// log the request //
	/////////////////////
	if( logger->isTraceEnabled() )
	{
		char* attributes = setToString( attributeList );
		if( active == RTI:: RTI_TRUE )
		{
			logger->trace( "[Starting] subscribeObjectClassAttributesWithRegion(active): classHandle=%d, attributes=%s, regionAddress=%p",
			               theClass, attributes, &theRegion );
		}
		else
		{
			logger->trace( "[Starting] subscribeObjectClassAttributesWithRegion(passive): classHandle=%d, attributes=%s regionAddress=%p",
			               theClass, attributes, &theRegion );
		}
		delete attributes;
	}
	
	/////////////////////////
	// do the subscription //
	/////////////////////////
	JNIEnv *env = privateRefs->rti->getJniEnvironment();

	// get java versions of the parameters
	jintArray jAttributes = privateRefs->rti->convertAHS( attributeList );

	if( active == RTI::RTI_TRUE )
	{
		// call the method
		env->CallVoidMethod( privateRefs->rti->jproxy,
		                     privateRefs->rti->SUBSCRIBE_ATTRIBUTES_WITH_REGION,
		                     theClass,
		                     privateRefs->rti->convertRegion(theRegion),
		                     jAttributes );
	}
	else
	{
		// call the method
		env->CallVoidMethod( privateRefs->rti->jproxy,
		                     privateRefs->rti->SUBSCRIBE_ATTRIBUTES_PASSIVELY_WITH_REGION,
		                     theClass,
		                     privateRefs->rti->convertRegion(theRegion),
		                     jAttributes );
	}

	// clean up and run the exception check
	env->DeleteLocalRef( jAttributes );
	privateRefs->rti->exceptionCheck();
	
	logger->trace( "[Finished] subscribeObjectClassAttributesWithRegion(): classHandle=%d", theClass );
}

// 9.9
void RTI::RTIambassador::unsubscribeObjectClassWithRegion( RTI::ObjectClassHandle theClass,
                                                           RTI::Region &theRegion )
	throw( RTI::ObjectClassNotDefined,
	       RTI::RegionNotKnown,
	       RTI::ObjectClassNotSubscribed,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] unsubscribeObjectClassWithRegion(): classHandle=%d, regionAddress=%p",
	               theClass, &theRegion );
	
	// call the method
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	env->CallVoidMethod( privateRefs->rti->jproxy,
	                     privateRefs->rti->UNSUBSCRIBE_ATTRIBUTES_WITH_REGION,
	                     theClass,
	                     privateRefs->rti->convertRegion(theRegion) );
	
	// run the exception check
	privateRefs->rti->exceptionCheck();

	logger->trace( "[Finished] unsubscribeObjectClassWithRegion(): classHandle=%d, regionAddress=%p",
	               theClass, &theRegion );
}

// 9.10
void RTI::RTIambassador::subscribeInteractionClassWithRegion( RTI::InteractionClassHandle theClass,
                                                              RTI::Region &theRegion,
                                                              RTI::Boolean active )
	throw( RTI::InteractionClassNotDefined,
	       RTI::RegionNotKnown,
	       RTI::InvalidRegionContext,
	       RTI::FederateLoggingServiceCalls,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	/////////////////////
	// log the request //
	/////////////////////
	if( logger->isTraceEnabled() )
	{
		if( active == RTI:: RTI_TRUE )
			logger->trace( "[Starting] subscribeInteractionClassWithRegion(active): classHandle=%d", theClass );
		else
			logger->trace( "[Starting] subscribeInteractionClassWIthRegion(passive): classHandle=%d", theClass );
	}

	/////////////////////////
	// do the subscription //
	/////////////////////////
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	if( active == RTI::RTI_TRUE )
	{
		// call the method
		env->CallVoidMethod( privateRefs->rti->jproxy,
		                     privateRefs->rti->SUBSCRIBE_INTERACTION_CLASS_WITH_REGION,
		                     theClass,
		                     privateRefs->rti->convertRegion(theRegion) );
	}
	else
	{
		// call the method
		env->CallVoidMethod( privateRefs->rti->jproxy,
		                     privateRefs->rti->SUBSCRIBE_INTERACTION_CLASS_PASSIVELY_WITH_REGION,
		                     theClass,
		                     privateRefs->rti->convertRegion(theRegion) );
	}

	// run the exception check
	privateRefs->rti->exceptionCheck();
	
	logger->trace( "[Finished] subscribeInteractionClassWithRegion(): classHandle=%d", theClass );
}

// 9.11
void
RTI::RTIambassador::unsubscribeInteractionClassWithRegion( RTI::InteractionClassHandle theClass,
                                                           RTI::Region &theRegion )
	throw( RTI::InteractionClassNotDefined,
	       RTI::InteractionClassNotSubscribed,
	       RTI::RegionNotKnown,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] unsubscribeInteractionClassWithRegion(): classHandle=%d", theClass );
	
	// call the method
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	env->CallVoidMethod( privateRefs->rti->jproxy,
	                     privateRefs->rti->UNSUBSCRIBE_INTERACTION_CLASS_WITH_REGION,
	                     theClass,
	                     privateRefs->rti->convertRegion(theRegion) );
	
	// run the exception check
	privateRefs->rti->exceptionCheck();

	logger->trace( "[Finished] unsubscribeInteractionClassWithRegion(): classHandle=%d", theClass );
}

// 9.12
RTI::EventRetractionHandle
RTI::RTIambassador::sendInteractionWithRegion( RTI::InteractionClassHandle theInteraction,
                                               const RTI::ParameterHandleValuePairSet &parameters,
                                               const RTI::FedTime& theTime,
                                               const char *theTag,
                                               const RTI::Region &theRegion )
	throw( RTI::InteractionClassNotDefined,
	       RTI::InteractionClassNotPublished,
	       RTI::InteractionParameterNotDefined,
	       RTI::InvalidFederationTime,
	       RTI::RegionNotKnown,
	       RTI::InvalidRegionContext,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	// get java versions of the parameters
	jbyteArray jTag = privateRefs->rti->convertTag( theTag );
	jdouble jTime = privateRefs->rti->convertTime( theTime );
	HVPS values = privateRefs->rti->convertPHVPS( parameters );
	
	// log the request
	if( logger->isTraceEnabled() )
	{
		char* parameterString = mapToString( parameters );
		logger->trace( "[Starting] sendInteractionWithRegion(TSO): classHandle=%d, parameters=%s, time=%f",
		               theInteraction, parameterString, jTime );
		delete parameterString;
	}
		
	// call the method
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	env->CallVoidMethod( privateRefs->rti->jproxy,
	                     privateRefs->rti->SEND_INTERACTION_WITH_TIME_AND_REGION,
	                     theInteraction,
	                     values.handles,
	                     values.values,
	                     jTag,
	                     privateRefs->rti->convertRegion(theRegion),
	                     jTime );
	
	// clean up and run the exception check
	env->DeleteLocalRef( jTag );
	env->DeleteLocalRef( values.handles );
	env->DeleteLocalRef( values.values );
	privateRefs->rti->exceptionCheck();

	logger->trace( "[Finished] sendInteractionWithRegion(TSO): classHandle=%d", theInteraction );

	// return an empty retraction handle
	return RTI::EventRetractionHandle();
}

void
RTI::RTIambassador::sendInteractionWithRegion( RTI::InteractionClassHandle theInteraction,
                                               const RTI::ParameterHandleValuePairSet &parameters,
                                               const char *theTag,
                                               const RTI::Region &theRegion )
	throw( RTI::InteractionClassNotDefined,
	       RTI::InteractionClassNotPublished,
	       RTI::InteractionParameterNotDefined,
	       RTI::RegionNotKnown,
	       RTI::InvalidRegionContext,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	if( logger->isTraceEnabled() )
	{
		char* parameterString = mapToString( parameters );
		logger->trace( "[Starting] sendInteractionWithRegion(RO): classHandle=%d, parameters=%s",
		               theInteraction, parameterString );
		delete parameterString;
	}

	// get java versions of the parameters
	jbyteArray jTag = privateRefs->rti->convertTag( theTag );
	HVPS values = privateRefs->rti->convertPHVPS( parameters );
	
	// call the method
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	env->CallVoidMethod( privateRefs->rti->jproxy,
	                     privateRefs->rti->SEND_INTERACTION_WITH_REGION,
	                     theInteraction,
	                     values.handles,
	                     values.values,
	                     jTag,
	                     privateRefs->rti->convertRegion(theRegion) );
	
	// clean up and run the exception check
	env->DeleteLocalRef( jTag );
	env->DeleteLocalRef( values.handles );
	env->DeleteLocalRef( values.values );
	privateRefs->rti->exceptionCheck();
	
	logger->trace( "[Finished] sendInteractionWithRegion(RO): classHandle=%d", theInteraction );
}

// 9.13
void RTI::RTIambassador::requestClassAttributeValueUpdateWithRegion(
		RTI::ObjectClassHandle theClass,
		const RTI::AttributeHandleSet &theAttributes,
		const RTI::Region &theRegion )
	throw( RTI::ObjectClassNotDefined,
	       RTI::AttributeNotDefined,
	       RTI::RegionNotKnown,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	if( logger->isTraceEnabled() )
	{
		char* attributeString = setToString( theAttributes );
		logger->trace( "[Starting] requestClassAttributeValueUpdateWithRegion(): theClass=%d, attributes=%s, regionAddress=%p",
		               theClass, attributeString, &theRegion );
		delete attributeString;
	}
	
	// convert the parameters to their java equiv
	jintArray jAttributes = privateRefs->rti->convertAHS( theAttributes );

	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	env->CallVoidMethod( privateRefs->rti->jproxy,
	                     privateRefs->rti->REQUEST_CLASS_ATTRIBUTE_VALUE_UPDATE_WITH_REGION,
	                     theClass,
	                     jAttributes,
	                     privateRefs->rti->convertRegion(theRegion) );

	env->DeleteLocalRef( jAttributes );
	// run the exception check
	privateRefs->rti->exceptionCheck();
	
	logger->trace( "[Finished] requestClassAttributeValueUpdateWithRegion(): theClass=%d regionAddress=%p",
	               theClass, &theRegion );
}

