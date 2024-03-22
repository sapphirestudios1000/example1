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

// get ourselves a logger we can use
// I was going to put this in the private refs, but it just created too
// much typing when it came to actually using it (privateRefs->logger->debug(...))
portico13::Logger* logger = new portico13::Logger( "c++" );

////////////////////////////////////
// Federation Management Services //
////////////////////////////////////
void RTI::RTIambassador::createFederationExecution( const char* executionName, const char *FED )
	throw( RTI::FederationExecutionAlreadyExists,
	       RTI::CouldNotOpenFED,
	       RTI::ErrorReadingFED,
	       RTI::ConcurrentAccessAttempted,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] createFederationExecution(): name=%s, fedfile=%s", executionName, FED );
	JNIEnv *env = privateRefs->rti->getJniEnvironment();

	// get java versions of the parameters
	jstring jExecutionName = env->NewStringUTF( executionName );
	jstring jFED = env->NewStringUTF( FED );
	
	// call the method
	env->CallVoidMethod( privateRefs->rti->jproxy,
	                     privateRefs->rti->CREATE_FEDERATION,
	                     jExecutionName,
	                     jFED );

	// clean up and run the exception check
	env->DeleteLocalRef( jExecutionName );
	env->DeleteLocalRef( jFED );
	privateRefs->rti->exceptionCheck();
	
	logger->trace( "[Finished] createFederationExecution(): name=%s, fedfile=%s", executionName, FED );
}

void RTI::RTIambassador::destroyFederationExecution( const char *executionName )
	throw( RTI::FederatesCurrentlyJoined,
	       RTI::FederationExecutionDoesNotExist,
	       RTI::ConcurrentAccessAttempted,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] destroyFederationExecution(): name=%s", executionName );
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	
	// get java versions of the parameters
	jstring jExecutionName = env->NewStringUTF( executionName );
	
	// call the method
	env->CallVoidMethod( privateRefs->rti->jproxy,
	                     privateRefs->rti->DESTROY_FEDERATION,
	                     jExecutionName );

	// clean up and run the exception check
	env->DeleteLocalRef( jExecutionName );
	privateRefs->rti->exceptionCheck();

	logger->trace( "[Finished] destroyFederationExecution(): name=%s", executionName );
}

// 4.4
RTI::FederateHandle
RTI::RTIambassador::joinFederationExecution( const char *federateName,
                                             const char *executionName,
                                             RTI::FederateAmbassador *federateAmbassador )
	throw( RTI::FederateAlreadyExecutionMember,
	       RTI::FederationExecutionDoesNotExist,
	       RTI::CouldNotOpenFED,
	       RTI::ErrorReadingFED,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] joinFederationExecution(): federationName=%s, federateName=%s",
	               executionName, federateName );
	JNIEnv *env = privateRefs->rti->getJniEnvironment();

	// check the federate ambassador
	if( federateAmbassador == NULL )
		throw RTI::RTIinternalError( "Null FederateAmbassador given to joinFederationExecution()" );

	// get java versions of the parameters
	jstring jFederateName = env->NewStringUTF( federateName );
	jstring jExecutionName = env->NewStringUTF( executionName );

	// store the user federate ambassador
	privateRefs->rti->fedamb = federateAmbassador;

	// call the method
	jint federateHandle = env->CallIntMethod( privateRefs->rti->jproxy,
	                                          privateRefs->rti->JOIN_FEDERATION,
	                                          jFederateName,
	                                          jExecutionName );

	// clean up and run the exception check
	env->DeleteLocalRef( jFederateName );
	env->DeleteLocalRef( jExecutionName );
	privateRefs->rti->exceptionCheck();
	
	logger->trace( "[Finished] joinFederationExecution(): federationName=%s, federateName=%s",
	               executionName, federateName );

	return federateHandle;
}

// 4.5
void RTI::RTIambassador::resignFederationExecution( RTI::ResignAction theAction )
	throw( RTI::FederateOwnsAttributes,
	       RTI::FederateNotExecutionMember,
	       RTI::InvalidResignAction,
	       RTI::ConcurrentAccessAttempted,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] resignFederationExecution(): action=%d", theAction );
	JNIEnv *env = privateRefs->rti->getJniEnvironment();

	// call the method
	env->CallVoidMethod( privateRefs->rti->jproxy, privateRefs->rti->RESIGN_FEDERATION, theAction );

	// run the exception check
	privateRefs->rti->exceptionCheck();

	// remove our reference to the user federate ambassador
	privateRefs->rti->fedamb = NULL;

	logger->trace( "[Finished] resignFederationExecution(): action=%d", theAction );	
}

// 4.6
void RTI::RTIambassador::registerFederationSynchronizationPoint( const char *label,
                                                                 const char *theTag )
	throw( RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{ 
	logger->trace( "[Starting] registerFederationSynchronizationPoint(): label=%s", label );
	JNIEnv *env = privateRefs->rti->getJniEnvironment();

	// get java versions of the parameters
	jstring jLabel  = env->NewStringUTF( label );
	jbyteArray jTag = privateRefs->rti->convertTag( theTag );
	
	// call the method
	env->CallVoidMethod( privateRefs->rti->jproxy,
	                     privateRefs->rti->REGISTER_FEDERATION_SYNCH,
	                     jLabel,
	                     jTag );

	// clean up and run the exception check
	env->DeleteLocalRef( jLabel );
	env->DeleteLocalRef( jTag );
	privateRefs->rti->exceptionCheck();

	logger->trace( "[Finsihed] registerFederationSynchronizationPoint(): label=%s", label );
}

void RTI::RTIambassador::registerFederationSynchronizationPoint( const char *label,
                                                                 const char *theTag,
                                                                 const RTI::FederateHandleSet& theSet )
	throw( RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	if( logger->isTraceEnabled() )
	{
		char* federateList = setToString( theSet );
		logger->trace( "[Starting] registerFederationSynchronizationPoint(): label=%s, federates=%s",
		               label, federateList );
		delete federateList;
	}

	// get java versions of the parameters
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	jstring jLabel     = env->NewStringUTF( label );
	jbyteArray jTag    = privateRefs->rti->convertTag( theTag );
	jintArray jSyncSet = privateRefs->rti->convertFHS( theSet );
	
	// call the method
	env->CallVoidMethod( privateRefs->rti->jproxy,
	                     privateRefs->rti->REGISTER_FEDERATION_SYNCH_FEDHANDLESET,
	                     jLabel,
	                     jTag,
	                     jSyncSet );

	// clean up and run the exception check
	env->DeleteLocalRef( jLabel );
	env->DeleteLocalRef( jTag );
	env->DeleteLocalRef( jSyncSet );
	privateRefs->rti->exceptionCheck();
	
	logger->trace( "[Finished] registerFederationSynchronizationPoint(FederateHandleSet): label=%s",
	               label );
}

// 4.9
void RTI::RTIambassador::synchronizationPointAchieved( const char *label )
	throw( RTI::SynchronizationPointLabelWasNotAnnounced,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] synchronizationPointAchieved(): label=%s", label );
	JNIEnv *env = privateRefs->rti->getJniEnvironment();

	// get java versions of the parameters
	jstring jLabel  = env->NewStringUTF( label );
	
	// call the method
	env->CallVoidMethod( privateRefs->rti->jproxy,
	                     privateRefs->rti->SYNCH_POINT_ACHIEVED,
	                     jLabel );

	// clean up and run the exception check
	env->DeleteLocalRef( jLabel );
	privateRefs->rti->exceptionCheck();

	logger->trace( "[Finished] synchronizationPointAchieved(): label=%s", label );
}

// 4.11
void RTI::RTIambassador::requestFederationSave( const char *label, const RTI::FedTime& theTime )
	throw( RTI::FederationTimeAlreadyPassed,
	       RTI::InvalidFederationTime,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	// get java versions of the parameters
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	jstring jLabel = env->NewStringUTF( label );
	jdouble jTime  = privateRefs->rti->convertTime( theTime );

	logger->trace( "[Starting] requestFederationSave(): label=%s, time=%f", label, jTime );

	// call the method
	env->CallVoidMethod( privateRefs->rti->jproxy,
	                     privateRefs->rti->REQUEST_FEDERATION_SAVE_TIME,
	                     jLabel,
	                     jTime );

	// clean up and run the exception check
	env->DeleteLocalRef( jLabel );
	privateRefs->rti->exceptionCheck();
	
	logger->trace( "[Finished] requestFederationSave(withTime): label=%s", label );
}

void RTI::RTIambassador::requestFederationSave( const char *label )
	throw( RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] requestFederationSave(): label=%s", label );
	JNIEnv *env = privateRefs->rti->getJniEnvironment();

	// get java versions of the parameters
	jstring jLabel  = env->NewStringUTF( label );
	
	// call the method
	env->CallVoidMethod( privateRefs->rti->jproxy,
	                     privateRefs->rti->REQUEST_FEDERATION_SAVE,
	                     jLabel );

	// clean up and run the exception check
	env->DeleteLocalRef( jLabel );
	privateRefs->rti->exceptionCheck();

	logger->trace( "[Finished] requestFederationSave(): label=%s", label );
}

// 4.13
void RTI::RTIambassador::federateSaveBegun()
	throw( RTI::SaveNotInitiated,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] federateSaveBegun()" );
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	
	// call the method
	env->CallVoidMethod( privateRefs->rti->jproxy, privateRefs->rti->FEDERATE_SAVE_BEGUN );
	
	// run the exception check
	privateRefs->rti->exceptionCheck();
	
	logger->trace( "[Finished] federateSaveBegun()" );
}

// 4.14
void RTI::RTIambassador::federateSaveComplete()
	throw( RTI::SaveNotInitiated,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] federateSaveComplete()" );
	JNIEnv *env = privateRefs->rti->getJniEnvironment();

	// call the method
	env->CallVoidMethod( privateRefs->rti->jproxy, privateRefs->rti->FEDERATE_SAVE_COMPLETE );
	
	// run the exception check
	privateRefs->rti->exceptionCheck();
	
	logger->trace( "[Finished] federateSaveComplete()" );
}

void RTI::RTIambassador::federateSaveNotComplete()
	throw( RTI::SaveNotInitiated,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] federateSaveNotComplete()" );
	JNIEnv *env = privateRefs->rti->getJniEnvironment();
	
	// call the method
	env->CallVoidMethod( privateRefs->rti->jproxy,
	                     privateRefs->rti->FEDERATE_SAVE_NOT_COMPLETE );
	
	// run the exception check
	privateRefs->rti->exceptionCheck();

	logger->trace( "[Finished] federateSaveNotComplete()" );
}

// 4.16
void RTI::RTIambassador::requestFederationRestore( const char *label )
	throw( RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RestoreInProgress,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] requestFederationRestore(): label=%s", label );
	JNIEnv *env = privateRefs->rti->getJniEnvironment();

	// get java versions of the parameters
	jstring jLabel  = env->NewStringUTF( label );
	
	// call the method
	env->CallVoidMethod( privateRefs->rti->jproxy,
	                     privateRefs->rti->REQUEST_FEDERATION_RESTORE,
	                     jLabel );

	// clean up and run the exception check
	env->DeleteLocalRef( jLabel );
	privateRefs->rti->exceptionCheck();
	
	logger->trace( "[Finished] requestFederationRestore(): label=%s", label );
}

// 4.20
void RTI::RTIambassador::federateRestoreComplete()
	throw( RTI::RestoreNotRequested,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] federateRestoreComplete()" );
	JNIEnv *env = privateRefs->rti->getJniEnvironment();

	// call the method
	env->CallVoidMethod( privateRefs->rti->jproxy,
	                     privateRefs->rti->FEDERATE_RESTORE_COMPLETE );
	
	// run the exception check
	privateRefs->rti->exceptionCheck();

	logger->trace( "[Finished] federateRestoreComplete()" );
}

void RTI::RTIambassador::federateRestoreNotComplete()
	throw( RTI::RestoreNotRequested,
	       RTI::FederateNotExecutionMember,
	       RTI::ConcurrentAccessAttempted,
	       RTI::SaveInProgress,
	       RTI::RTIinternalError )
{
	logger->trace( "[Starting] federateRestoreNotComplete()" );
	JNIEnv *env = privateRefs->rti->getJniEnvironment();

	// call the method
	env->CallVoidMethod( privateRefs->rti->jproxy,
	                     privateRefs->rti->FEDERATE_RESTORE_NOT_COMPLETE );
	
	// run the exception check
	privateRefs->rti->exceptionCheck();

	logger->trace( "[Finished] federateRestoreNotComplete()" );
}
