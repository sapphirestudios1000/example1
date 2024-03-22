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
#pragma once

#include "RTI/SpecificConfig.h"

namespace portico1516e
{
	/**
	 * Describes the byte ordering of {@link BasicType} datatypes.
	 */
	enum RTI_EXPORT Endianness
	{
		ENDIANNESS_LITTLE,
		ENDIANNESS_BIG
	};
}
