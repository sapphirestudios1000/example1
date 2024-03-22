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
#include "types/handles/HandleFriends.h"

//
// Provides the body for the standard HandleFriend types as declared in
// the HandleFriends.h header. This basically gives us privilieged access
// call the protected getImplementation() methods and get at the underlying
// data so we can do useful things that the standard interface won't allow.
//
#define HANDLE_FRIEND_BODY(Type)                                 \
Type Type##Friend::create( int32_t value )                       \
{                                                                \
	Type handle;                                                 \
	handle._impl->value = value;                                 \
	return handle;                                               \
}                                                                \
                                                                 \
int32_t Type##Friend::getInt( Type& handle )                     \
{                                                                \
	return handle._impl->value;                                  \
}                                                                \
                                                                 \
std::string Type##Friend::toString( Type* handle )               \
{                                                                \
	std::stringstream ss;                                        \
	ss << handle->_impl->value;                                  \
	return ss.str();                                             \
}                                                                \
                                                                 \
std::string Type##Friend::toString( Type& handle )               \
{                                                                \
	std::stringstream ss;                                        \
	ss << handle._impl->value;                                   \
	return ss.str();                                             \
}                                                                \
                                                                 \
std::string Type##Friend::toString( const Type& handle )         \
{                                                                \
	std::stringstream ss;                                        \
	ss << handle._impl->value;                                   \
	return ss.str();                                             \
}


// expand the macro
IEEE1516E_NS_START

HANDLE_FRIEND_BODY(FederateHandle)
HANDLE_FRIEND_BODY(ObjectClassHandle)
HANDLE_FRIEND_BODY(InteractionClassHandle)
HANDLE_FRIEND_BODY(ObjectInstanceHandle)
HANDLE_FRIEND_BODY(AttributeHandle)
HANDLE_FRIEND_BODY(ParameterHandle)
HANDLE_FRIEND_BODY(DimensionHandle)
HANDLE_FRIEND_BODY(MessageRetractionHandle)
HANDLE_FRIEND_BODY(RegionHandle)

IEEE1516E_NS_END
