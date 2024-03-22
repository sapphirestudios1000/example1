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

#include "BitHelpers.h"

IEEE1516E_NS_START

//----------------------------------------------------------
//                     STATIC METHODS
//----------------------------------------------------------
void BitHelpers::copyMemoryLE( char* dest, const char* source, size_t length )
{
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	for( size_t i = 0 ; i < length ; ++i )
		dest[length - 1 - i] = source[i];
#else
	::memcpy( dest, source, length );
#endif
}

void BitHelpers::copyMemoryBE( char* dest, const char* source, size_t length )
{
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	::memcpy( dest, source, length );
#else
	for( size_t i = 0 ; i < length ; ++i )
		dest[length - 1 - i] = source[i];
#endif
}

////////////////////////////////////////////////////////////
// Float Helpers
////////////////////////////////////////////////////////////
void BitHelpers::encodeFloatBE( float value, char* buffer, size_t offset )
{
	char* asChars = (char*)&value;
	copyMemoryBE( buffer + offset, asChars, LENGTH_FLOAT );
}

float BitHelpers::decodeFloatBE( const char* buffer, size_t offset )
{
	char asBytes[LENGTH_FLOAT];
	const char* offsetPtr = buffer + offset;

	copyMemoryBE( asBytes, offsetPtr, LENGTH_FLOAT );

	return *((float*)asBytes);
}

void BitHelpers::encodeFloatBE( float value, std::vector<Octet>& buffer )
{
    char asBytes[LENGTH_FLOAT];
    copyMemoryBE( asBytes, (char*)&value, LENGTH_FLOAT );

    buffer.insert( buffer.end(), asBytes, asBytes + LENGTH_FLOAT );
}

float BitHelpers::decodeFloatBE( const std::vector<Octet>& buffer, size_t index )
	throw (EncoderException)
{
	if( index + LENGTH_FLOAT > buffer.size() )
		throw EncoderException( L"Insufficient data in buffer to decode value" );

	char asBytes[LENGTH_FLOAT];
	copyMemoryBE( asBytes, &buffer[index], LENGTH_FLOAT );

	return *((float*)asBytes);
}

void BitHelpers::encodeFloatLE( float value, char* buffer, size_t offset )
{
	char* asChars = (char*)&value;
	copyMemoryLE( buffer + offset, asChars, LENGTH_FLOAT );
}

float BitHelpers::decodeFloatLE( const char* buffer, size_t offset )
{
	char asBytes[LENGTH_FLOAT];
	const char* offsetPtr = buffer + offset;

	copyMemoryLE( asBytes, offsetPtr, LENGTH_FLOAT );

	return *((float*)asBytes);
}

void BitHelpers::encodeFloatLE( float value, std::vector<Octet>& buffer )
{
    char asBytes[LENGTH_FLOAT];
    copyMemoryLE( asBytes, (char*)&value, LENGTH_FLOAT );

    buffer.insert( buffer.end(), asBytes, asBytes + LENGTH_FLOAT );
}

float BitHelpers::decodeFloatLE( const std::vector<Octet>& buffer, size_t index )
	throw (EncoderException)
{
	if( index + LENGTH_FLOAT > buffer.size() )
		throw EncoderException( L"Insufficient data in buffer to decode value" );

	char asBytes[LENGTH_FLOAT];
	copyMemoryLE( asBytes, &buffer[index], LENGTH_FLOAT );

	return *((float*)asBytes);
}

////////////////////////////////////////////////////////////
// Double Helpers
////////////////////////////////////////////////////////////
void BitHelpers::encodeDoubleBE( double value, char* buffer, size_t offset )
{
	char* asChars = (char*)&value;
	copyMemoryBE( buffer + offset, asChars, LENGTH_DOUBLE );
}

double BitHelpers::decodeDoubleBE( const char* buffer, size_t offset )
{
	char asBytes[LENGTH_DOUBLE];
	const char* offsetPtr = buffer + offset;

	copyMemoryBE( asBytes, offsetPtr, LENGTH_DOUBLE );

	return *((double*)asBytes);
}

void BitHelpers::encodeDoubleBE( double value, std::vector<Octet>& buffer )
{
    char asBytes[LENGTH_DOUBLE];
    copyMemoryBE( asBytes, (char*)&value, LENGTH_DOUBLE );

    buffer.insert( buffer.end(), asBytes, asBytes + LENGTH_DOUBLE );
}

double BitHelpers::decodeDoubleBE( const std::vector<Octet>& buffer, size_t index )
	throw (EncoderException)
{
	if( index + LENGTH_DOUBLE > buffer.size() )
		throw EncoderException( L"Insufficient data in buffer to decode value" );

	char asBytes[LENGTH_DOUBLE];
	copyMemoryBE( asBytes, &buffer[index], LENGTH_DOUBLE );

	return *((double*)asBytes);
}

void BitHelpers::encodeDoubleLE( double value, char* buffer, size_t offset )
{
	char* asChars = (char*)&value;
	copyMemoryLE( buffer + offset, asChars, LENGTH_DOUBLE );
}

double BitHelpers::decodeDoubleLE( const char* buffer, size_t offset )
{
	char asBytes[LENGTH_DOUBLE];
	const char* offsetPtr = buffer + offset;

	copyMemoryLE( asBytes, offsetPtr, LENGTH_DOUBLE );

	return *((double*)asBytes);
}

void BitHelpers::encodeDoubleLE( double value, std::vector<Octet>& buffer )
{
    char asBytes[LENGTH_DOUBLE];
    copyMemoryLE( asBytes, (char*)&value, LENGTH_DOUBLE );

    buffer.insert( buffer.end(), asBytes, asBytes + LENGTH_DOUBLE );
}

double BitHelpers::decodeDoubleLE( const std::vector<Octet>& buffer, size_t index )
	throw (EncoderException)
{
	if( index + LENGTH_DOUBLE > buffer.size() )
		throw EncoderException( L"Insufficient data in buffer to decode value" );

	char asBytes[LENGTH_DOUBLE];
	copyMemoryLE( asBytes, &buffer[index], LENGTH_DOUBLE );

	return *((double*)asBytes);
}

////////////////////////////////////////////////////////////
// Short Helpers
////////////////////////////////////////////////////////////
void BitHelpers::encodeShortBE( int16_t value, char* buffer, size_t offset )
{
	char* asChars = (char*)&value;
	copyMemoryBE( buffer + offset, asChars, LENGTH_SHORT );
}

int16_t BitHelpers::decodeShortBE( const char* buffer, size_t offset )
{
	char asBytes[LENGTH_SHORT];
	const char* offsetPtr = buffer + offset;

	copyMemoryBE( asBytes, offsetPtr, LENGTH_SHORT );

	return *((int16_t*)asBytes);
}

void BitHelpers::encodeShortBE( int16_t value, std::vector<Octet>& buffer )
{
    char asBytes[LENGTH_SHORT];
    copyMemoryBE( asBytes, (char*)&value, LENGTH_SHORT );

    buffer.insert( buffer.end(), asBytes, asBytes + LENGTH_SHORT );
}

int16_t BitHelpers::decodeShortBE( const std::vector<Octet>& buffer, size_t index )
	throw( EncoderException )
{
	if( index + LENGTH_SHORT > buffer.size() )
		throw EncoderException( L"Insufficient data in buffer to decode value" );

	char asBytes[LENGTH_SHORT];
	copyMemoryBE( asBytes, &buffer[index], LENGTH_SHORT );

	return *((int16_t*)asBytes);
}

void BitHelpers::encodeShortLE( int16_t value, char* buffer, size_t offset )
{
	char* asChars = (char*)&value;
	copyMemoryLE( buffer + offset, asChars, LENGTH_SHORT );
}

int16_t BitHelpers::decodeShortLE( const char* buffer, size_t offset )
{
	char asBytes[LENGTH_SHORT];
	const char* offsetPtr = buffer + offset;

	copyMemoryLE( asBytes, offsetPtr, LENGTH_SHORT );

	return *((int16_t*)asBytes);
}

void BitHelpers::encodeShortLE( int16_t value, std::vector<Octet>& buffer )
{
    char asBytes[LENGTH_SHORT];
    copyMemoryLE( asBytes, (char*)&value, LENGTH_SHORT );

    buffer.insert( buffer.end(), asBytes, asBytes + LENGTH_SHORT );
}

int16_t BitHelpers::decodeShortLE( const std::vector<Octet>& buffer, size_t index )
	throw( EncoderException )
{
	if( index + LENGTH_SHORT > buffer.size() )
		throw EncoderException( L"Insufficient data in buffer to decode value" );

	char asBytes[LENGTH_SHORT];
	copyMemoryLE( asBytes, &buffer[index], LENGTH_SHORT );

	return *((int16_t*)asBytes);
}

////////////////////////////////////////////////////////////
// Int Helpers
////////////////////////////////////////////////////////////
void BitHelpers::encodeIntBE( int32_t value, char* buffer, size_t offset )
{
	char* asChars = (char*)&value;
	copyMemoryBE( buffer + offset, asChars, LENGTH_INT );
}

int32_t BitHelpers::decodeIntBE( const char* buffer, size_t offset )
{
	char asBytes[LENGTH_INT];
	const char* offsetPtr = buffer + offset;

	copyMemoryBE( asBytes, offsetPtr, LENGTH_INT );

	return *((int32_t*)asBytes);
}

void BitHelpers::encodeIntBE( int32_t value, std::vector<Octet>& buffer )
{
    char asBytes[LENGTH_INT];
    copyMemoryBE( asBytes, (char*)&value, LENGTH_INT );
    buffer.insert( buffer.end(), asBytes, asBytes + LENGTH_INT );
}

int32_t BitHelpers::decodeIntBE( const std::vector<Octet>& buffer, size_t index )
	throw( EncoderException )
{
	if( index + LENGTH_INT > buffer.size() )
		throw EncoderException( L"Insufficient data in buffer to decode value" );

	char asBytes[LENGTH_INT];
	copyMemoryBE( asBytes, &buffer[index], LENGTH_INT );

	return *((int32_t*)asBytes);
}

void BitHelpers::encodeIntLE( int32_t value, char* buffer, size_t offset )
{
	char* asChars = (char*)&value;
	copyMemoryLE( buffer + offset, asChars, LENGTH_INT );
}

int32_t BitHelpers::decodeIntLE( const char* buffer, size_t offset )
{
	char asBytes[LENGTH_INT];
	const char* offsetPtr = buffer + offset;

	copyMemoryLE( asBytes, offsetPtr, LENGTH_INT );

	return *((int32_t*)asBytes);
}

void BitHelpers::encodeIntLE( int32_t value, std::vector<Octet>& buffer )
{
    char asBytes[LENGTH_INT];
    copyMemoryLE( asBytes, (char*)&value, LENGTH_INT );
    buffer.insert( buffer.end(), asBytes, asBytes + LENGTH_INT );
}

int32_t BitHelpers::decodeIntLE( const std::vector<Octet>& buffer, size_t index )
	throw( EncoderException )
{
	if( index + LENGTH_INT > buffer.size() )
		throw EncoderException( L"Insufficient data in buffer to decode value" );

	char asBytes[LENGTH_INT];
	copyMemoryLE( asBytes, &buffer[index], LENGTH_INT );

	return *((int32_t*)asBytes);
}

////////////////////////////////////////////////////////////
// Long Helpers
////////////////////////////////////////////////////////////
void BitHelpers::encodeLongBE( int64_t value, char* buffer, size_t offset )
{
	char* asChars = (char*)&value;
	copyMemoryBE( buffer + offset, asChars, LENGTH_LONG );
}

int64_t BitHelpers::decodeLongBE( const char* buffer, size_t offset )
{
	char asBytes[LENGTH_LONG];
	const char* offsetPtr = buffer + offset;

	copyMemoryBE( asBytes, offsetPtr, LENGTH_LONG );

	return *((int64_t*)asBytes);
}

void BitHelpers::encodeLongBE( int64_t value, std::vector<Octet>& buffer )
{
    char asBytes[LENGTH_LONG];
    copyMemoryBE( asBytes, (char*)&value, LENGTH_LONG );
    buffer.insert( buffer.end(), asBytes, asBytes + LENGTH_LONG );
}

int64_t BitHelpers::decodeLongBE( const std::vector<Octet>& buffer, size_t index )
	throw( EncoderException )
{
	if( index + LENGTH_LONG > buffer.size() )
		throw EncoderException( L"Insufficient data in buffer to decode value" );

	char asBytes[LENGTH_LONG];
	copyMemoryBE( asBytes, &buffer[index], LENGTH_LONG );

	return *((int64_t*)asBytes);
}

void BitHelpers::encodeLongLE( int64_t value, char* buffer, size_t offset )
{
	char* asChars = (char*)&value;
	copyMemoryLE( buffer + offset, asChars, LENGTH_LONG );
}

int64_t BitHelpers::decodeLongLE( const char* buffer, size_t offset )
{
	char asBytes[LENGTH_LONG];
	const char* offsetPtr = buffer + offset;

	copyMemoryLE( asBytes, offsetPtr, LENGTH_LONG );

	return *((int64_t*)asBytes);
}

void BitHelpers::encodeLongLE( int64_t value, std::vector<Octet>& buffer )
{
    char asBytes[LENGTH_LONG];
    copyMemoryLE( asBytes, (char*)&value, LENGTH_LONG );
    buffer.insert( buffer.end(), asBytes, asBytes + LENGTH_LONG );
}

int64_t BitHelpers::decodeLongLE( const std::vector<Octet>& buffer, size_t index )
	throw( EncoderException )
{
	if( index + LENGTH_LONG > buffer.size() )
		throw EncoderException( L"Insufficient data in buffer to decode value" );

	char asBytes[LENGTH_LONG];
	copyMemoryLE( asBytes, &buffer[index], LENGTH_LONG );

	return *((int64_t*)asBytes);
}

////////////////////////////////////////////////////////////
// Unicode String Helpers
////////////////////////////////////////////////////////////
size_t BitHelpers::getEncodedLength( const std::wstring& value )
{
	return BitHelpers::LENGTH_INT +
		   BitHelpers::LENGTH_SHORT +
		   (BitHelpers::LENGTH_SHORT * value.size() );
}

void BitHelpers::encodeUnicodeString( const std::wstring& value, char* buffer, size_t offset )
{
	size_t len = value.size();
	size_t headerSize = BitHelpers::LENGTH_INT + BitHelpers::LENGTH_SHORT;

	char* data = buffer + offset;

	// Write out string length (include the BOM in this figure)
	BitHelpers::encodeIntBE( 1 + len, data, 0 );

	// Write out the BOM
	BitHelpers::encodeShortBE( UNICODE_BOM, data, BitHelpers::LENGTH_INT );

	for( size_t i = 0 ; i < len ; ++i )
	{
		BitHelpers::encodeShortBE( (int16_t)value.at(i),
								   data,
								   headerSize + (i * BitHelpers::LENGTH_SHORT) );
	}
}

IEEE1516E_NS_END
