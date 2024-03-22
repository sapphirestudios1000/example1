/*
 *   Copyright 2008 The Portico Project
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
#ifndef PARAMETERHANDLEVALUEPAIRSETTEST_H_
#define PARAMETERHANDLEVALUEPAIRSETTEST_H_

#include "../common/Common.h"

class ParameterHandleValuePairSetTest : public CppUnit::TestFixture
{
	//----------------------------------------------------------
	//                    STATIC VARIABLES
	//----------------------------------------------------------

	//----------------------------------------------------------
	//                   INSTANCE VARIABLES
	//----------------------------------------------------------
    private:
        Test13Federate *defaultFederate;
        RTI::ParameterHandleValuePairSet *theSet;

	//----------------------------------------------------------
	//                      CONSTRUCTORS
	//----------------------------------------------------------
	public:
		ParameterHandleValuePairSetTest();
		virtual ~ParameterHandleValuePairSetTest();

	//----------------------------------------------------------
	//                    INSTANCE METHODS
	//----------------------------------------------------------
	public:
		void setUp();
		void tearDown();
	
	protected:
		void testSize();
		void testGetHandle();
		void testGetHandleWithInvalidIndex();
		void testAdd();
		void testGetValueLength();
		void testGetValueLengthWithInvalidIndex();
		void testGetValue();
		void testGetValueWithInvalidIndex();
		void testGetValuePointer();
		void testGetValuePointerWithInvalidIndex();
		void testRemove();
		void testEmpty();

	//----------------------------------------------------------
	//                     STATIC METHODS
	//----------------------------------------------------------
	CPPUNIT_TEST_SUITE( ParameterHandleValuePairSetTest );
		CPPUNIT_TEST( testSize );
		CPPUNIT_TEST( testGetHandle );
		CPPUNIT_TEST( testGetHandleWithInvalidIndex );
		CPPUNIT_TEST( testAdd );
		CPPUNIT_TEST( testGetValueLength );
		CPPUNIT_TEST( testGetValueLengthWithInvalidIndex );
		CPPUNIT_TEST( testGetValue );
		CPPUNIT_TEST( testGetValueWithInvalidIndex );
		CPPUNIT_TEST( testGetValuePointer );
		CPPUNIT_TEST( testGetValuePointerWithInvalidIndex );
		CPPUNIT_TEST( testRemove );
		CPPUNIT_TEST( testEmpty );
	CPPUNIT_TEST_SUITE_END();

};

#endif /*PARAMETERHANDLEVALUEPAIRSETTEST_H_*/
