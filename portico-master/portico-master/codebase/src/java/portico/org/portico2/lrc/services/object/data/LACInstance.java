/*
 *   Copyright 2018 The Portico Project
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
package org.portico2.lrc.services.object.data;

import java.io.Serializable;

import org.portico.lrc.model.ACMetadata;
import org.portico.lrc.model.RegionInstance;
import org.portico2.common.PorticoConstants;

/**
 * This class represents a single instance of an attribute that exists inside a {@link LOCInstance}.
 * It contains information such as the handle and type (metadata) of the attribute, however, it
 * does not hold the value of the attribute.
 * <p/>
 * 
 * <b>NOTE</b><p/>
 * {@link LOCInstance} and {@link LACInstance} objects only live in the LRC (hence the 'L' prefix).
 */
public class LACInstance implements Serializable
{
	//----------------------------------------------------------
	//                    STATIC VARIABLES
	//----------------------------------------------------------
	private static final long serialVersionUID = 98121116105109L;

	//----------------------------------------------------------
	//                   INSTANCE VARIABLES
	//----------------------------------------------------------
	private int handle;
	private int owner;
	private LOCInstance container;
	private ACMetadata type;
	private RegionInstance region;

	//----------------------------------------------------------
	//                      CONSTRUCTORS
	//----------------------------------------------------------
	protected LACInstance( ACMetadata type, LOCInstance container )
	{
		this.handle = type.getHandle();
		this.owner = PorticoConstants.NULL_HANDLE;
		this.container = container;
		this.type = type;
		this.region = null;
	}

	//----------------------------------------------------------
	//                    INSTANCE METHODS
	//----------------------------------------------------------

	////////////////////////////////////////////////////////////////////////////////////////
	///  Accessors and Mutators   //////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////
	public LOCInstance getContainer()
	{
		return container;
	}

	public void setContainer( LOCInstance container )
	{
		this.container = container;
	}

	/**
	 * The handle of the attribute type from the FOM. This is not a unique handle for the instance
	 * itself, but the handle that identifies the attribute within the FOM.
	 */
	public int getHandle()
	{
		return handle;
	}

	/**
	 * The federate handle of the owning federate. If this attribute is not owned by any federate,
	 * {@link PorticoConstants#NULL_HANDLE} is used.
	 */
	public int getOwner()
	{
		return owner;
	}

	/**
	 * Returns <code>true</code> if the attribute is not currently owned by anyone.
	 */
	public boolean isUnowned()
	{
		return owner == PorticoConstants.NULL_HANDLE;
	}

	/**
	 * Returns <code>true</code> if the attribute is owned by the RTI.
	 */
	public boolean isOwnedByRti()
	{
		return owner == PorticoConstants.RTI_HANDLE;
	}
	
	/**
	 * Returns <code>true</code> if the attribute is owned by the provided federate handle,
	 * <code>false</code> otherwise.
	 */
	public boolean isOwnedBy( int federateHandle )
	{
		return owner == federateHandle;
	}
	
	/**
	 * The federate handle of the owning federate. If this attribute is not owned by any federate,
	 * {@link PorticoConstants#NULL_HANDLE} should be used.
	 */
	public void setOwner( int owner )
	{
		this.owner = owner;
	}
	
	/**
	 * Removes any owner information for the attribute. Makes it as if this attribute is unowned.
	 */
	public void unown()
	{
		this.owner = PorticoConstants.NULL_HANDLE;
	}

	public ACMetadata getType()
	{
		return type;
	}

	public void setType( ACMetadata type )
	{
		this.type = type;
		this.handle = type.getHandle();
	}

	/**
	 * Returns the region associated with this particular attribute. If there is no region for the
	 * attribute, null is returned.
	 */
	public RegionInstance getRegion()
	{
		return this.region;
	}
	
	public void setRegion( RegionInstance region )
	{
		this.region = region;
	}

	//----------------------------------------------------------
	//                     STATIC METHODS
	//----------------------------------------------------------
}
