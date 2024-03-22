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
#include "common.h"
#include "types/encoding/BitHelpers.h"
#include "types/encoding/basic/BasicTypeImpl.h"
#include "RTI/encoding/BasicDataElements.h"

IEEE1516E_NS_START

DEFINE_BASIC_TYPE_IMPL( HLAinteger32BEImplementation, Integer32 )

//------------------------------------------------------------------------------------------
//                                       CONSTRUCTORS                                       
//------------------------------------------------------------------------------------------
// Constructor: Default
// Uses internal memory.
HLAinteger32BE::HLAinteger32BE()
{
	this->_impl = new HLAinteger32BEImplementation( 0 );
}

// Constructor: Initial Value
// Uses internal memory.
HLAinteger32BE::HLAinteger32BE( const Integer32& inData )
{
	this->_impl = new HLAinteger32BEImplementation( inData );
}

// Constructor: External memory
// This instance changes or is changed by contents of external memory.
// Caller is responsible for ensuring that the external memory is
// valid for the lifetime of this object or until this object acquires
// new memory through setDataPointer.
// A null value will construct instance to use internal memory.
HLAinteger32BE::HLAinteger32BE( Integer32* inData )
{
	this->_impl = new HLAinteger32BEImplementation( inData );
}

// Constructor: Copy
// Uses internal memory.
HLAinteger32BE::HLAinteger32BE( const HLAinteger32BE& rhs )
{
	this->_impl = new HLAinteger32BEImplementation( rhs.get() );
}

HLAinteger32BE::~HLAinteger32BE()
{
	delete this->_impl;
}

//------------------------------------------------------------------------------------------
//                                     INSTANCE METHODS
//------------------------------------------------------------------------------------------
// Return a new copy of the DataElement
// Copy uses internal memory.
std::auto_ptr<DataElement> HLAinteger32BE::clone() const
{
	return std::auto_ptr<DataElement>( new HLAinteger32BE(*this) );
}

// Encode this element into a new VariableLengthData
VariableLengthData HLAinteger32BE::encode() const
	throw( EncoderException )
{
	VariableLengthData data;
	this->encode( data );

	return data;
}

// Encode this element into an existing VariableLengthData
void HLAinteger32BE::encode( VariableLengthData& inData ) const
	throw( EncoderException )
{
	// Assign a buffer to take the Integer32
	char buffer[BitHelpers::LENGTH_INT];
	BitHelpers::encodeIntBE( this->get(), buffer, 0 );
	
	inData.setData( buffer, BitHelpers::LENGTH_INT );
}

// Encode this element and append it to a buffer
void HLAinteger32BE::encodeInto( std::vector<Octet>& buffer ) const
	throw( EncoderException )
{
	char data[BitHelpers::LENGTH_INT];
	BitHelpers::encodeIntBE( this->get(), data, 0 );

	buffer.insert( buffer.end(), data, data + BitHelpers::LENGTH_INT );
}

// Decode this element from the RTI's VariableLengthData.
void HLAinteger32BE::decode( const VariableLengthData& inData )
	throw( EncoderException )
{
	if( inData.size() < BitHelpers::LENGTH_INT )
		throw EncoderException( L"Insufficient data in buffer to decode value" );

	Integer32 value = BitHelpers::decodeIntBE( (const char*)inData.data(), 0 );
	this->set( value );
}

// Decode this element starting at the index in the provided buffer
// Return the index immediately after the decoded data.
size_t HLAinteger32BE::decodeFrom( const std::vector<Octet>& buffer, size_t index )
	throw( EncoderException )
{
	Integer32 value = BitHelpers::decodeIntBE( buffer, index );
	this->set( value );
	return index + BitHelpers::LENGTH_INT;
}

// Return the size in bytes of this element's encoding.
size_t HLAinteger32BE::getEncodedLength() const
	throw( EncoderException )
{
	return BitHelpers::LENGTH_INT;
}

// Return the octet boundary of this element.
unsigned int HLAinteger32BE::getOctetBoundary() const
{
	return BitHelpers::LENGTH_INT;
}

// Return a hash of the encoded data
// Provides mechanism to map DataElement discriminants to variants
// in VariantRecord.
Integer64 HLAinteger32BE::hash() const
{
	return 31 * 7 + this->get();
}

// Change this instance to use supplied external memory.
// Caller is responsible for ensuring that the external memory is
// valid for the lifetime of this object or until this object acquires
// new memory through this call.
// Null pointer results in an exception.
void HLAinteger32BE::setDataPointer( Integer32* inData )
	throw( EncoderException )
{
	this->_impl->setUseExternalMemory( inData );
}

// Set the value to be encoded.
// If this element uses external memory, the memory will be modified.
void HLAinteger32BE::set( Integer32 inData )
{
	this->_impl->setValue( inData );
}

// Get the value from encoded data.
Integer32 HLAinteger32BE::get() const
{
	return this->_impl->getValue();
}

//------------------------------------------------------------------------------------------
//                                    OPERATOR OVERLOADS
//------------------------------------------------------------------------------------------
// Assignment Operator
// Uses existing memory of this instance.
HLAinteger32BE& HLAinteger32BE::operator= ( const HLAinteger32BE& rhs )
{
	this->_impl->setUseInternalMemory( rhs.get() );
	return *this;
}

// Assignment of the value to be encoded data.
// If this element uses external memory, the memory will be modified.
HLAinteger32BE& HLAinteger32BE::operator= ( Integer32 rhs )
{
	this->set( rhs );
	return *this;
}

// Conversion operator to Integer32
// Return value from encoded data.
HLAinteger32BE::operator Integer32() const
{
	return this->get();
}

//------------------------------------------------------------------------------------------
//                                      STATIC METHODS
//------------------------------------------------------------------------------------------

IEEE1516E_NS_END
