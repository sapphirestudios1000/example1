/*
 *   Copyright 2012 The Portico Project
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
#include "rtiamb/PorticoRtiAmbassador.h"

//------------------------------------------------------------------------------------------
//                                       CONSTRUCTORS                                       
//------------------------------------------------------------------------------------------
PORTICO1516E_NS_START

PorticoRtiAmbassador::PorticoRtiAmbassador()
{
	this->javarti = Runtime::getRuntime()->newRtiAmbassador();
	this->logger = new Logger( "c++" );
	this->datatypeRetriever = new DatatypeRetrieval( this->javarti );
}

PorticoRtiAmbassador::~PorticoRtiAmbassador()
{
	Runtime::getRuntime()->removeRtiAmbassador( this->javarti );
	delete this->logger;
	delete this->datatypeRetriever;
}

PORTICO1516E_NS_END

//------------------------------------------------------------------------------------------
//                               STANDARD HEADER CONSTRUCTORS                                       
//------------------------------------------------------------------------------------------
IEEE1516E_NS_START

RTIambassador::RTIambassador() throw()
{
	
}

RTIambassador::~RTIambassador()
{
	
}

IEEE1516E_NS_END
