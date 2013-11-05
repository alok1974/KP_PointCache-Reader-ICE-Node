//#########################################################################################
//#########################################################################################
//                                                                                       ##
//  KP_PointCache-Reader-ICE-Node V 1.0 (c) 2013 Alok Gandhi (alok.gandhi2002@gmail.com) ##
//                                                                                       ##
//                                                                                       ##
//  This file is part of KP_PointCache-Reader-ICE-Node.                                  ##
//                                                                                       ##
//  KP_PointCache-Reader-ICE-Node is free software: you can redistribute it and/or modify##
//  it under the terms of the GNU General Public License, Version 3, 29 June 2007        ##
//  as published by the Free Software Foundation,                                        ##
//                                                                                       ##
//  KP_PointCache-Reader-ICE-Node is distributed in the hope that it will be useful,     ##
//  but WITHOUT ANY WARRANTY; without even the implied warranty of                       ##
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                        ##
//  GNU General Public License for more details.                                         ##
//                                                                                       ##
//  You should have received a copy of the GNU General Public License                    ##
//  along with KP_PointCache-Reader-ICE-Node.  If not, see <http://www.gnu.org/licenses/>##
//	                                                                                     ##
//	The original Code for KP_PointCache Core is written by Kai Wolter. This work is just ##
//	an extension / reuse of his code as is.                                              ##
//	                                                                                     ##
//	Kai Wolter's original Licence can be found in the other files of this code as        ##
//	and when                                                                             ##
//	needed.                                                                              ##
//                                                                                       ##
//                                                                                       ##
//#########################################################################################
//#########################################################################################



#include "HelperFunctions.h"

using namespace XSI;
using namespace MATH;

enum IDs
{
	ID_IN_File_Name = 0,
	ID_IN_Offset = 1,
	ID_IN_TimeWarp = 2,
	ID_IN_UseCustomPlayback = 3,
	ID_IN_Frame = 4,
	ID_IN_Interpolation = 5,
	ID_IN_HermiteTension = 6,
	ID_IN_HermiteBias = 7,
	ID_IN_LoadOption = 8,
	ID_IN_UseYup = 9,
	ID_G_100 = 100,
	ID_OUT_Points = 200,
	ID_TYPE_CNS = 400,
	ID_STRUCT_CNS,
	ID_CTXT_CNS,
	ID_UNDEF = ULONG_MAX
};

XSI::CStatus RegisterReadPC2( XSI::PluginRegistrar& in_reg );



SICALLBACK XSILoadPlugin( PluginRegistrar& in_reg )
{
	in_reg.PutAuthor(L"Alok Gandhi");
	in_reg.PutName(L"ReadPC2");
	in_reg.PutVersion(1,0);

	RegisterReadPC2( in_reg );

	return CStatus::OK;
}

SICALLBACK XSIUnloadPlugin( const PluginRegistrar& in_reg )
{
	CString strPluginName;
	strPluginName = in_reg.GetName();
	Application().LogMessage(strPluginName + L" has been unloaded.",siVerboseMsg);
	return CStatus::OK;
}


CStatus RegisterReadPC2( PluginRegistrar& in_reg )
{
	ICENodeDef nodeDef;
	nodeDef = Application().GetFactory().CreateICENodeDef(L"ReadPC2",L"ReadPC2");

	CStatus st;
	st = nodeDef.PutColor(154,188,102);
	st.AssertSucceeded( ) ;


	st = nodeDef.AddPortGroup(ID_G_100);
	st.AssertSucceeded( ) ;

	st = nodeDef.AddInputPort(	ID_IN_File_Name,
								ID_G_100,
								siICENodeDataString,
								siICENodeStructureSingle,
								siICENodeContextAny,
								L"File_Name",
								L"File_Name",
								L"",
								CValue(),
								CValue(),
								ID_UNDEF,
								ID_UNDEF,
								ID_CTXT_CNS);
	st.AssertSucceeded( ) ;

	
	
	st = nodeDef.AddInputPort(	ID_IN_Offset,ID_G_100,
								siICENodeDataFloat,
								siICENodeStructureSingle,
								siICENodeContextAny,
								L"Offset",
								L"Offset",
								0,
								CValue(),
								CValue(),
								ID_UNDEF,
								ID_UNDEF,
								ID_CTXT_CNS);
	st.AssertSucceeded( ) ;

	
	
	st = nodeDef.AddInputPort(	ID_IN_TimeWarp,
								ID_G_100,siICENodeDataFloat,
								siICENodeStructureSingle,
								siICENodeContextAny,
								L"TimeWarp",
								L"TimeWarp",
								1.0f,
								CValue(),
								CValue(),
								ID_UNDEF,
								ID_UNDEF,
								ID_CTXT_CNS);
	st.AssertSucceeded( ) ;

	
	
	st = nodeDef.AddInputPort(	ID_IN_UseCustomPlayback,
								ID_G_100,
								siICENodeDataBool,
								siICENodeStructureSingle,
								siICENodeContextAny,
								L"UseCustomPlayback",
								L"UseCustomPlayback",
								false,
								CValue(),
								CValue(),
								ID_UNDEF,
								ID_UNDEF,
								ID_CTXT_CNS);
	st.AssertSucceeded( ) ;

	
	
	st = nodeDef.AddInputPort(	ID_IN_Frame,
								ID_G_100,
								siICENodeDataFloat,
								siICENodeStructureSingle,
								siICENodeContextAny,
								L"Frame",
								L"Frame",
								0,
								CValue(),
								CValue(),
								ID_UNDEF,
								ID_UNDEF,
								ID_CTXT_CNS);
	st.AssertSucceeded( ) ;

	
	
	st = nodeDef.AddInputPort(	ID_IN_Interpolation,
								ID_G_100,
								siICENodeDataLong,
								siICENodeStructureSingle,
								siICENodeContextAny,
								L"Interpolation",
								L"Interpolation",
								0,
								CValue(),
								CValue(),
								ID_UNDEF,
								ID_UNDEF,
								ID_CTXT_CNS);
	st.AssertSucceeded( ) ;

	
	
	st = nodeDef.AddInputPort(	ID_IN_HermiteTension,
								ID_G_100,
								siICENodeDataFloat,
								siICENodeStructureSingle,
								siICENodeContextAny,
								L"HermiteTension",
								L"HermiteTension",
								0.0f,
								CValue(),
								CValue(),
								ID_UNDEF,
								ID_UNDEF,
								ID_CTXT_CNS);
	st.AssertSucceeded( ) ;

	
	
	st = nodeDef.AddInputPort(	ID_IN_HermiteBias,
								ID_G_100,
								siICENodeDataFloat,
								siICENodeStructureSingle,
								siICENodeContextAny,
								L"HermiteBias",
								L"HermiteBias",
								0.0f,
								CValue(),
								CValue(),
								ID_UNDEF,
								ID_UNDEF,
								ID_CTXT_CNS);
	st.AssertSucceeded( ) ;

	
	
	st = nodeDef.AddInputPort(	ID_IN_LoadOption,
								ID_G_100,
								siICENodeDataLong,
								siICENodeStructureSingle,
								siICENodeContextAny,
								L"LoadOption",
								L"LoadOption",
								1,
								CValue(),
								CValue(),
								ID_UNDEF,
								ID_UNDEF,
								ID_CTXT_CNS);
	st.AssertSucceeded( ) ;

	
	
	st = nodeDef.AddInputPort(	ID_IN_UseYup,
								ID_G_100,
								siICENodeDataBool,
								siICENodeStructureSingle,
								siICENodeContextAny,
								L"UseYup",
								L"UseYup",
								false,
								CValue(),
								CValue(),
								ID_UNDEF,
								ID_UNDEF,
								ID_CTXT_CNS);
	st.AssertSucceeded( ) ;

	st = nodeDef.AddOutputPort(	ID_OUT_Points,
								siICENodeDataVector3,
								siICENodeStructureArray,
								siICENodeContextAny,
								L"Points",
								L"Points",
								ID_UNDEF,
								ID_UNDEF,
								ID_CTXT_CNS);
	st.AssertSucceeded( ) ;

	
	
	PluginItem nodeItem = in_reg.RegisterICENode(nodeDef);
	nodeItem.PutCategories(L"Custom ICENode");

	return CStatus::OK;
}

SICALLBACK ReadPC2_Init( CRef& in_ref )
{
	Context ctxt = in_ref;

	PointCacheCore * pCacheCore = new PointCacheCore();
	ctxt.PutUserData( (CValue::siPtrType)pCacheCore );
	
	return CStatus::OK;
}


SICALLBACK ReadPC2_Term( CRef& in_ref )
{
	Context ctxt = in_ref;
	CValue userData = ctxt.GetUserData( );
	if ( userData.IsEmpty() )
	{
		return CStatus::OK;
	}

	PointCacheCore* pCacheCore = (PointCacheCore*)(CValue::siPtrType)ctxt.GetUserData( );
	if ( pCacheCore )
	{
		delete pCacheCore;
	}

	ctxt.PutUserData( CValue() );
	
	return CStatus::OK;
}

SICALLBACK ReadPC2_BeginEvaluate( ICENodeContext& ctxt )
{
	ctxt.SetAsTimeVarying();

	CValue userData = ctxt.GetUserData( );
	if ( userData.IsEmpty() )
	{
		return CStatus::False;
	}

	PointCacheCore* pCacheCore = (PointCacheCore*)(CValue::siPtrType)ctxt.GetUserData( );
	if ( !pCacheCore )
	{
		return CStatus::False;
	}

	return CStatus::OK;

}


SICALLBACK ReadPC2_Evaluate( ICENodeContext& ctxt )
{


	ULONG out_portID = ctxt.GetEvaluatedOutputPortID( );

	CDataArrayString _strFilepath( ctxt, ID_IN_File_Name );
	CString strFilepath = _strFilepath[0];

	if(strFilepath.Length() == 0)
	{
		Application().LogMessage(CString("No Cache File specified"), siWarningMsg);
		return CStatus::OK;
	}

	CDataArrayFloat _offset( ctxt, ID_IN_Offset );
	double offset = (double) _offset[0];

	CDataArrayFloat _timeWarp( ctxt, ID_IN_TimeWarp );
	double timeWarp = (double) _timeWarp[0];
	
	CDataArrayBool _customPlayback( ctxt, ID_IN_UseCustomPlayback );
	bool customPlayback = (bool) _customPlayback[0];

	CDataArrayFloat _customFrame( ctxt, ID_IN_Frame );
	double customFrame = (double) _customFrame[0];


	CDataArrayLong _interpolation( ctxt, ID_IN_Interpolation );
	int interpolation = (int) _interpolation[0];

	CDataArrayFloat _hermiteTension( ctxt, ID_IN_HermiteTension );
	double hermiteTension = (double) _hermiteTension[0];

	CDataArrayFloat _hermiteBias( ctxt, ID_IN_HermiteBias );
	double hermiteBias = (double) _hermiteBias[0];

	CDataArrayLong _loadOption( ctxt, ID_IN_LoadOption );
	int loadOption = (int) _loadOption[0];

	CDataArrayBool _useYUp( ctxt, ID_IN_UseYup );
	bool useYUp = (bool) _useYUp[0];

	
	int numDataPoints;
	double currentFrame = ctxt.GetTime().GetTime();
	CVector3Array posArray(0);

	const wchar_t *orig = strFilepath.GetWideString();
	size_t origsize = wcslen(orig) + 1;
    const size_t newsize = 500;
    size_t convertedChars = 0;
    char nstring[newsize];
	wcstombs(nstring, orig, origsize);
	

	PointCacheCore* pCacheCore = (PointCacheCore*)(CValue::siPtrType)ctxt.GetUserData( );
	if ( pCacheCore && pCacheCore->OpenCache(nstring, loadOption) > -1 )
	{
		pCacheCore->GetNumPoints(numDataPoints);
		posArray.Resize((LONG)numDataPoints);

		if ( customPlayback )
		  currentFrame = customFrame;
		CalculatePosArray(pCacheCore, currentFrame, timeWarp, offset, posArray, useYUp, interpolation, hermiteTension, hermiteBias);
		if ( loadOption == 1 )
		  pCacheCore->Close();
	}
	else
	{
		Application().LogMessage(CString("Unable to open Cache File <") + strFilepath + ">", siWarningMsg);
		return CStatus::OK;
	}

	switch( out_portID )
	{		
		case ID_OUT_Points :
		{

			if(posArray.GetCount() == 0)
			{
				return CStatus::OK;
			}

	
			CDataArray2DVector3f outData(ctxt);
			CIndexSet indexSet( ctxt );
			for(CIndexSet::Iterator it = indexSet.Begin(); it.HasNext(); it.Next())
			{
				CDataArray2DVector3f::Accessor outDataSubArray = outData.Resize(it, numDataPoints);
				for(int k = 0; k < numDataPoints; k++)
				{
					outDataSubArray[k] = CVector3f(posArray[k].GetX(), posArray[k].GetY(), posArray[k].GetZ());
				}
			
			}
		}
		break;

	};
	
	return CStatus::OK;
}

SICALLBACK ReadPC2_EndEvaluate( ICENodeContext& ctxt )
{

	CValue userData = ctxt.GetUserData( );
	if ( userData.IsEmpty() )
	{
		return CStatus::OK;
	}

	PointCacheCore* pCacheCore = (PointCacheCore*)(CValue::siPtrType)ctxt.GetUserData( );
	if ( pCacheCore )
	{
		pCacheCore->Close();
	}
	
	return CStatus::OK;
}
