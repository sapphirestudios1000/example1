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
#pragma once

#include "common.h"
#include "RTI/encoding/BasicDataElements.h"

IEEE1516E_NS_START

/**
 * A helper class that provides routines to encode/decode native types to/from a byte array
 */
class BitHelpers
{
    //----------------------------------------------------------
	//                    STATIC VARIABLES
	//----------------------------------------------------------
    public:
        static const size_t LENGTH_CHAR = 1;
        static const size_t LENGTH_FLOAT = 4;
        static const size_t LENGTH_DOUBLE = 8;
        static const size_t LENGTH_SHORT = 2;
        static const size_t LENGTH_INT = 4;
        static const size_t LENGTH_LONG = 8;
        static const size_t LENGTH_WCHAR = 2;

        static const short UNICODE_BOM = 0xfeff;
    
	//----------------------------------------------------------
	//                   INSTANCE VARIABLES
	//----------------------------------------------------------
    
    //----------------------------------------------------------
	//                      CONSTRUCTORS
	//----------------------------------------------------------
    
    //----------------------------------------------------------
	//                    INSTANCE METHODS
	//----------------------------------------------------------

    //----------------------------------------------------------
	//                     STATIC METHODS
	//----------------------------------------------------------
    private:
        static void copyMemoryBE( char* dest, const char* source, size_t length );
        static void copyMemoryLE( char* dest, const char* source, size_t length );

    public:
        ////////////////////////////////////////////////////////////
        // Float Helpers
        ////////////////////////////////////////////////////////////
        static void  encodeFloatBE( float value, char* buffer, size_t offset );
        static float decodeFloatBE( const char* buffer, size_t offset );
        static void  encodeFloatBE( float value, std::vector<Octet>& buffer );
        static float decodeFloatBE( const std::vector<Octet>& buffer, size_t index )
			throw( EncoderException );
        static void  encodeFloatLE( float value, char* buffer, size_t offset );
		static float decodeFloatLE( const char* buffer, size_t offset );
		static void  encodeFloatLE( float value, std::vector<Octet>& buffer );
		static float decodeFloatLE( const std::vector<Octet>& buffer, size_t index )
			throw( EncoderException );

		////////////////////////////////////////////////////////////
		// Double Helpers
		////////////////////////////////////////////////////////////
		static void   encodeDoubleBE( double value, char* buffer, size_t offset );
		static double decodeDoubleBE( const char* buffer, size_t offset );
		static void   encodeDoubleBE( double value, std::vector<Octet>& buffer );
		static double decodeDoubleBE( const std::vector<Octet>& buffer, size_t index )
			throw( EncoderException );
		static void   encodeDoubleLE( double value, char* buffer, size_t offset );
		static double decodeDoubleLE( const char* buffer, size_t offset );
		static void   encodeDoubleLE( double value, std::vector<Octet>& buffer );
		static double decodeDoubleLE( const std::vector<Octet>& buffer, size_t index )
			throw( EncoderException );

		////////////////////////////////////////////////////////////
		// Short Helpers
		////////////////////////////////////////////////////////////
		static void    encodeShortBE( int16_t value, char* buffer, size_t offset );
		static int16_t decodeShortBE( const char* buffer, size_t offset );
		static void    encodeShortBE( int16_t value, std::vector<Octet>& buffer );
		static int16_t decodeShortBE( const std::vector<Octet>& buffer, size_t index )
			throw( EncoderException );
		static void    encodeShortLE( int16_t value, char* buffer, size_t offset );
		static int16_t decodeShortLE( const char* buffer, size_t offset );
		static void    encodeShortLE( int16_t value, std::vector<Octet>& buffer );
		static int16_t decodeShortLE( const std::vector<Octet>& buffer, size_t index )
			throw( EncoderException );

		////////////////////////////////////////////////////////////
		// Int Helpers
		////////////////////////////////////////////////////////////
		static void    encodeIntBE( int32_t value, char* buffer, size_t offset );
		static int32_t decodeIntBE( const char* buffer, size_t offset );
		static void    encodeIntBE( int32_t value, std::vector<Octet>& buffer );
		static int32_t decodeIntBE( const std::vector<Octet>& buffer, size_t index )
			throw( EncoderException );
		static void    encodeIntLE( int32_t value, char* buffer, size_t offset );
		static int32_t decodeIntLE( const char* buffer, size_t offset );
		static void    encodeIntLE( int32_t value, std::vector<Octet>& buffer );
		static int32_t decodeIntLE( const std::vector<Octet>& buffer, size_t index )
			throw( EncoderException );

		////////////////////////////////////////////////////////////
		// Long Helpers
		////////////////////////////////////////////////////////////
		static void    encodeLongBE( int64_t value, char* buffer, size_t offset );
		static int64_t decodeLongBE( const char* buffer, size_t offset );
		static void    encodeLongBE( int64_t value, std::vector<Octet>& buffer );
		static int64_t decodeLongBE( const std::vector<Octet>& buffer, size_t index )
			throw( EncoderException );
		static void    encodeLongLE( int64_t value, char* buffer, size_t offset );
		static int64_t decodeLongLE( const char* buffer, size_t offset );
		static void    encodeLongLE( int64_t value, std::vector<Octet>& buffer );
		static int64_t decodeLongLE( const std::vector<Octet>& buffer, size_t index )
			throw( EncoderException );

		////////////////////////////////////////////////////////////
		// Unicode String Helpers
		////////////////////////////////////////////////////////////
		static size_t getEncodedLength( const std::wstring& value );
		static void   encodeUnicodeString( const std::wstring& value, char* buffer, size_t offset );
};

IEEE1516E_NS_END
