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


#ifndef _HelperFunctions_
#define _HelperFunctions_

#include <xsi_application.h>
#include <xsi_context.h>
#include <xsi_pluginregistrar.h>
#include <xsi_status.h>

#include <xsi_icenodecontext.h>
#include <xsi_icenodedef.h>
#include <xsi_command.h>
#include <xsi_factory.h>
#include <xsi_longarray.h>
#include <xsi_doublearray.h>
#include <xsi_math.h>
#include <xsi_vector2f.h>
#include <xsi_vector3f.h>
#include <xsi_vector4f.h>
#include <xsi_matrix3f.h>
#include <xsi_matrix4f.h>
#include <xsi_rotationf.h>
#include <xsi_quaternionf.h>
#include <xsi_color4f.h>
#include <xsi_shape.h>
#include <xsi_icegeometry.h>
#include <xsi_iceportstate.h>
#include <xsi_indexset.h>
#include <xsi_dataarray.h>
#include <xsi_dataarray2D.h>
#include <xsi_ppglayout.h>

#include "KP_PointCacheCore.h"


using namespace XSI;
using namespace MATH;

double LinearInterpolate(
   double y1,double y2,
   double mu);

double CosineInterpolate(
   double y1,double y2,
   double mu);

double CubicInterpolate(
   double y0,double y1,
   double y2,double y3,
   double mu);

double BezierInterpolate(
   double y0,double y1,
   double y2,double y3,
   double mu);

double HermiteInterpolate(
   double y0,double y1,
   double y2,double y3,
   double mu,
   double tension,
   double bias);

void ReadFrameData(PointCacheCore *pCacheCore, int frameNumber, bool useYUp, int numDataPoints, CVector3Array & returnArray);

void CalculatePosArray(PointCacheCore *pCacheCore, double currentFrame, double timeWarp, double offset, CVector3Array &posArray, bool useYUp, int interpolation, double hermite_tension, double hermite_bias);


#endif