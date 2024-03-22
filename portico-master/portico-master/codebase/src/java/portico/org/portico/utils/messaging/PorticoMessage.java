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
package org.portico.utils.messaging;

import java.io.IOException;
import java.io.ObjectInput;
import java.io.ObjectInputStream;
import java.io.ObjectOutput;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.util.HashSet;
import java.util.Set;

import org.portico.lrc.compat.JRTIinternalError;
import org.portico.lrc.utils.MessageHelpers;
import org.portico2.common.PorticoConstants;
import org.portico2.common.messaging.MessageType;

/**
 * This is the parent class of all Portico request messages. All messages sent through the Portico
 * framework should extend this message. It contains a number of settings to identify the source
 * and target federates, the timestamp associated with the message, whether immediate processing
 * is required (i.e. the message should not be queued) and so forth.
 * 
 * <p/>
 * <b>Broadcast and Targeted Messages</b>
 * <p/>
 * A message is considered broadcast if the target federate for it is set to
 * {@link PorticoConstants#NULL_HANDLE}. This is the default state for all messages. These
 * messages will be sent to all federates rather than just a specific federate.
 * 
 * <p/>
 * <b>Immediate Processing Flag</b>
 * <p/>
 * A common approach for handing incoming messages is to queue them up somewhere. To signal to any
 * message handling components (or perhaps a connection, depending on the implementation) that a
 * message needs to be handlded immediately, you can set the immediate processing required flag via
 * the {@link #setImmediateProcessingFlag(boolean)} method.
 * 
 * <p/>
 * <b>IMPLEMENTATION NOTE:</b> Although this class *doesn't* implement <code>java.io.Externalizable
 * </code>, it does provide implementations of the required methods so that subclasses can call
 * them if they want to (thus removing the need to handle the parent class data). It also
 * implements private writeObject and readObject methods for faster serialization where a child
 * class is not Externalizable.
 */
public abstract class PorticoMessage implements Serializable, Cloneable
{
	//----------------------------------------------------------
	//                    STATIC VARIABLES
	//----------------------------------------------------------
	private static final long serialVersionUID = 98121116105109L;
	
	//----------------------------------------------------------
	//                   INSTANCE VARIABLES
	//----------------------------------------------------------
	protected int sourceFederate;
	protected int targetFederate;
	protected int targetFederation;
	protected boolean isFromRti; // true if this message was generated or forwarded by the RTI
	protected boolean immediate; // does this message require immediate processing?

	protected double timestamp;
	
	protected Set<Integer> multipleTargets; // only used if we have multiple targets for a message

	//----------------------------------------------------------
	//                      CONSTRUCTORS
	//----------------------------------------------------------

	protected PorticoMessage()
	{
		super();
		this.isFromRti = false;
		this.sourceFederate = PorticoConstants.NULL_HANDLE;
		this.targetFederate = PorticoConstants.NULL_HANDLE;
		this.targetFederation = PorticoConstants.NULL_HANDLE;
		this.timestamp = PorticoConstants.NULL_TIME;
		this.immediate = false;
		
		this.multipleTargets = null; // only set when it has to be
	}

	//----------------------------------------------------------
	//                    INSTANCE METHODS
	//----------------------------------------------------------
	/**
	 * @return The {@link MessageType} that identifies this particular class of message.
	 */
	public abstract MessageType getType();

	/**
	 * Should this message be sent asynchornously? If it should, this will return true.
	 * At the moment, only messages sent FROM the RTI are async. For all other messages,
	 * if they are control messages they are synchronous; if they are data messages then
	 * they are asynchronous.
	 * 
	 * @return true if this messages should be sent asynchronously; false otherwise
	 */
	public boolean isAsync()
	{
		return this.isFromRti;
	}

	/**
	 * Messages can be generated by or routed through the RTI. The "source federate" field is
	 * often used to represent the original source/instigator or "owner" of a message for the
	 * purposes of identifying the federate. However, we need to know what did/didn't come from
	 * the RTI in order to route messages. As such, there is a special boolean flag on a message
	 * specifically to tell us if it has been routed via the RTI, and this is it.
	 * 
	 * @return True if the message came via the RTI, false otherwise
	 */
	public boolean isFromRti()
	{
		return this.isFromRti;
	}
	
	public void setIsFromRti( boolean fromRti )
	{
		this.isFromRti = fromRti;
		this.setSourceFederateIfNull( PorticoConstants.RTI_HANDLE );
		//this.sourceFederate = PorticoConstants.RTI_HANDLE;
	}
	
	/**
	 * Defaults to {@link PorticoConstants#NULL_HANDLE} unless otherwise set.
	 */
	public int getSourceFederate()
	{
		return this.sourceFederate;
	}

	/**
	 * Set the source federate to the given handle. Will overwrite what exists (unlike
	 * {@link #setSourceFederateIfNull(int)} which will preserve any existing value).
	 * 
	 * @param federateHandle The handle to set the source federate to
	 */
	public void setSourceFederate( int federateHandle )
	{
		this.sourceFederate = federateHandle;
		if( federateHandle == PorticoConstants.RTI_HANDLE )
			setIsFromRti( true );
	}

	/**
	 * Set the source federate to the given handle, but only if it is currently not set.
	 * If it is set, don't do anything.
	 * 
	 * @param federateHandle The handle to set as the source federate of the message
	 */
	public void setSourceFederateIfNull( int federateHandle )
	{
		if( this.sourceFederate == PorticoConstants.NULL_HANDLE )
			this.sourceFederate = federateHandle;
	}

	public void setTargetFederate( int federateHandle )
	{
		this.targetFederate = federateHandle;
	}
	
	/**
	 * Sets the target federate for this message. You have one of three general options:
	 * <ol>
	 *   <li>Send to all: Provide no arguments or {@link PorticoConstants#TARGET_ALL_HANDLE}</li>
	 *   <li>Send to single: Just use the handle of the federate you are targeting</li>
	 *   <li>Send to many: Include the handles for the mulitple federates you are targeting</li>
	 * </ol>
	 * 
	 * If this is a multiple-target message, the handle will be set to the symbolic value of
	 * {@link PorticoConstants#TARGET_MANY_HANDLE} and the actual target handles will be encoded
	 * elsewhere.
	 * 
	 * @param federateHandles The handles of the federates we are targeting
	 */
	public void setTargetFederates( int... federateHandles )
	{
		if( federateHandles.length == 0 )
		{
			this.targetFederate = PorticoConstants.TARGET_ALL_HANDLE;
		}
		else if( federateHandles.length == 1 )
		{
			this.targetFederate = federateHandles[0];
		}
		else
		{
			this.targetFederate = PorticoConstants.TARGET_MANY_HANDLE;
			this.multipleTargets = new HashSet<>();
			for( int i = 0; i < federateHandles.length; i++ )
				multipleTargets.add( federateHandles[i] );
		}
	}
	
	/**
	 * Sets the target federate for this message. You have one of three general options:
	 * <ol>
	 *   <li>Send to all: Provide no arguments or {@link PorticoConstants#TARGET_ALL_HANDLE}</li>
	 *   <li>Send to single: Just use the handle of the federate you are targeting</li>
	 *   <li>Send to many: Include the handles for the mulitple federates you are targeting</li>
	 * </ol>
	 * 
	 * If this is a multiple-target message, the handle will be set to the symbolic value of
	 * {@link PorticoConstants#TARGET_MANY_HANDLE} and the actual target handles will be encoded
	 * elsewhere.
	 * 
	 * @param federateHandles The handles of the federates we are targeting
	 */
	public void setTargetFederates( Set<Integer> targets )
	{
		if( targets.size() == 0 )
		{
			this.targetFederate = PorticoConstants.TARGET_ALL_HANDLE;
		}
		else if( targets.size() == 1 )
		{
			this.targetFederate = targets.stream().findFirst().get();
		}
		else
		{
			this.targetFederate = PorticoConstants.TARGET_MANY_HANDLE;
			this.multipleTargets = new HashSet<>( targets );
		}
	}
	
	/**
	 * Defaults to {@link PorticoConstants#NULL_HANDLE} unless otherwise set.
	 */
	public int getTargetFederate()
	{
		return this.targetFederate;
	}

	public Set<Integer> getMultipleTargets()
	{
		if( this.multipleTargets == null )
		{
			Set<Integer> targets = new HashSet<>();
			targets.add( targetFederate );
			return targets;
		}
		else
			return this.multipleTargets;
	}

	/**
	 * Defaults to {@link PorticoConstants#TARGET_ALL_HANDLE} unless otherwise set.
	 */
	public int getTargetFederation()
	{
		return this.targetFederation;
	}

	public void setTargetFederation( int federationHandle )
	{
		this.targetFederation = federationHandle;
	}

	/**
	 * This method will return the current timestamp value for this message. By default it will
	 * return {@link PorticoConstants#NULL_TIME}. If you want a message to no longer have a
	 * timestamp, you should call {@link #setTimestamp(double)
	 * setTimestamp(PorticoConstants.NULL_TIME)}.
	 */
	public double getTimestamp()
	{
		return this.timestamp;
	}

	/**
	 * This method will return true if the current timestamp is not equal to
	 * {@link PorticoConstants#NULL_TIME}. If it is equal to that value, the message is not meant
	 * to be timestamped (thus, false will be returned).
	 */
	public void setTimestamp( double timestamp )
	{
		this.timestamp = timestamp;
	}

	/**
	 * Returns <code>true</code> if a timestamp has been set for this message.
	 */
	public boolean isTimestamped()
	{
		return this.timestamp != PorticoConstants.NULL_TIME;
	}

	public boolean isTimeAdvance()
	{
		return false;
	}

	/**
	 * Is this callback type a "message" as defined in the HLA specification. The specification
	 * defines "messages" as follows:
	 * <pre>
	 * 3.1.51 message: A change of object instance attribute value, an interaction, or a deletion
	 *                 of an existing object instance, often associated with a particular point on
	 *                 the High Level Architecture (HLA) time axis, as denoted by the associated
	 *                 timestamp.
	 * </pre>
	 * 
	 * @return Hardcoded to return <code>false</code>. Should be overridden to return
	 * <code>true</code> in any message type falls into this category.
	 */
	public boolean isSpecDefinedMessage()
	{
		return false;
	}
	
	/**
	 * Returns <code>true</code> if there is no target federate for this message
	 */
	public boolean isBroadcast()
	{
		return this.targetFederate == PorticoConstants.NULL_HANDLE;
	}
	
	/**
	 * Returns <code>true</code> if immediate processing of this message is required. If this is
	 * <code>true</code>, it is a directive that the message should be processed as soon as
	 * possible. Generally speaking, this means that the message should not be placed into any
	 * queue for later processing and should be processed right away. For example, when a message
	 * with this flag is offered to the LRCMessageQueue, it in turn drops the message directly into
	 * the incoming message sink of the associated kernel.
	 */
	public boolean isImmediateProcessingRequired()
	{
		return this.immediate;
	}
	
	/**
	 * Set the "immediate processing required" flag to the given value.
	 */
	public void setImmediateProcessingFlag( boolean value )
	{
		this.immediate = value;
	}

	/**
	 * Returns the simple (non-qualified) name of the implementation class.
	 */
	public String getIdentifier()
	{
		return getClass().getSimpleName();
	}

	/////////////////////////////////////////////////////////////
	/////////////////////// Clone Methods ///////////////////////
	/////////////////////////////////////////////////////////////
	public Object clone() throws CloneNotSupportedException
	{
		return super.clone();
	}

	/**
	 * A type-safe version of clone that will call {@link #clone()} and cast it to the given type
	 * before returning it.
	 */
	public <T extends PorticoMessage> T clone( Class<T> expectedType ) throws JRTIinternalError
	{
		try
		{
			return expectedType.cast( this.clone() );
		}
		catch( CloneNotSupportedException cnse )
		{
			throw new JRTIinternalError( "Error performing clone()", cnse );
		}
	}
	
	/////////////////////////////////////////////////////////////
	/////////////////// Serialization Methods ///////////////////
	/////////////////////////////////////////////////////////////
	private void writeObject( ObjectOutputStream oos ) throws IOException
	{
		writeExternal( oos );
	}

	private void readObject( ObjectInputStream ois)  throws IOException, ClassNotFoundException
	{
		readExternal( ois );
	}
	
	public void readExternal( ObjectInput input ) throws IOException, ClassNotFoundException
	{
		this.isFromRti = input.readBoolean();
		this.sourceFederate = input.readInt();
		this.targetFederate = input.readInt();
		this.targetFederation = input.readInt();
		this.timestamp = input.readDouble();
		this.immediate = input.readBoolean();
	}
	
	public void writeExternal( ObjectOutput output ) throws IOException
	{
		// FIXME What about multiple targets?
		output.writeBoolean( isFromRti );
		output.writeInt( sourceFederate );
		output.writeInt( targetFederate );
		output.writeInt( targetFederation );
		output.writeDouble( timestamp );
		output.writeBoolean( immediate );
	}
	
	protected String bytesToString( byte[] bytes )
	{
		if( bytes != null )
			return new String(bytes);
		else
			return "";
	}
	
	protected byte[] stringToBytes( String string )
	{
		if( string == null )
			return "".getBytes();
		else
			return string.getBytes();
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////// Manual Marshaling Methods ///////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	/**
	 * If message types support manual marshaling, they should override this method to return
	 * <code>true</code>. By default it returns <code>false</code> and the automatic serialization
	 * mechanisms will be used by the {@link MessageHelpers}. If this method returns
	 * <code>true</code>, the {@link #marshal(ObjectOutput)} and {@link #unmarshal(ObjectInput)}
	 * methods will be used for optimized performance. PLEASE: only use this if you absolutely
	 * have to.
	 * <p/>
	 * <b>NOTE:</b> For this to work successfully, each message class must have a unique id (so
	 * that on unmarhsal, an appropriate instance can be created). Thus, there must be an entry
	 * for the class in the MessageHelpers.newMessageForId() private method. This method is hard
	 * coded so that you can really only make use of this facility if really, really want it. Yes,
	 * it's not clean or elegant, but that's also half the point in this case. Don't use this!
	 */
	public boolean supportsManualMarshal()
	{
		return false;
	}
	
	/**
	 * <i><b>NOTE</b>: These methods are provided to bypass the typical serialization process used
	 * to convert messages into byte[]'s for sending. Unless absolute, extreme, total performance
	 * is needed for a particular message, these methods should not be used. They are only for
	 * those methods that are high-volume (like interactions and reflections). These methods as
	 * they exist here do nothing, they are designed to be overridden by the subclass. By
	 * declaring them here, they can be invoked on a generic PorticoMessage, hence the reason they
	 * exist. See {@link MessageHelpers#deflate(PorticoMessage)}.</i>
	 * <p/>
	 * This method will marshal our local values into the given buffer.
	 */
	public void marshal( ObjectOutput buffer ) throws IOException
	{
	}
	
	/**
	 * <i><b>NOTE</b>: These methods are provided to bypass the typical serialization process used
	 * to convert messages into byte[]'s for sending. Unless absolute, extreme, total performance
	 * is needed for a particular message, these methods should not be used. They are only for
	 * those methods that are high-volume (like interactions and reflections). These methods as
	 * they exist here do nothing, they are designed to be overridden by the subclass. By
	 * declaring them here, they can be invoked on a generic PorticoMessage, hence the reason they
	 * exist. See {@link MessageHelpers#inflate(byte[],Class)}.</i>
	 * <p/>
	 * This method will unmarshal our local values from given buffer.
	 */
	public void unmarshal( ObjectInput buffer ) throws IOException, ClassNotFoundException
	{
	}
	
	//----------------------------------------------------------
	//                     STATIC METHODS
	//----------------------------------------------------------
}
