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



/// linear interpolation implementation
double LinearInterpolate(
   double y1,double y2,
   double mu)
{
   return(y1*(1-mu)+y2*mu);
}

/// cosine interpolation implementation
double CosineInterpolate(
   double y1,double y2,
   double mu)
{
   double mu2;

   mu2 = (1-cos(mu*PI))/2;
   return(y1*(1-mu2)+y2*mu2);
}

/// cubic interpolation implementation
double CubicInterpolate(
   double y0,double y1,
   double y2,double y3,
   double mu)
{
   double a0,a1,a2,a3,mu2;

   mu2 = mu*mu;
   a0 = y3 - y2 - y0 + y1;
   a1 = y0 - y1 - a0;
   a2 = y2 - y0;
   a3 = y1;

   return(a0*mu*mu2+a1*mu2+a2*mu+a3);
}

/// bezier interpolation implementation
double BezierInterpolate(
   double y0,double y1,
   double y2,double y3,
   double mu)
{
   double mum1,mum13,mu3;


   mum1 = 1 - mu;
   mum13 = mum1 * mum1 * mum1;
   mu3 = mu * mu * mu;

   return ( mum13*y0 + 3*mu*mum1*mum1*y1 + 3*mu*mu*mum1*y2 + mu3*y3 );
}

/// hermite interpolation implementation
double HermiteInterpolate(
   double y0,double y1,
   double y2,double y3,
   double mu,
   double tension,
   double bias)
{
   double m0,m1,mu2,mu3;
   double a0,a1,a2,a3;

	mu2 = mu * mu;
	mu3 = mu2 * mu;
   m0  = (y1-y0)*(1+bias)*(1-tension)/2;
   m0 += (y2-y1)*(1-bias)*(1-tension)/2;
   m1  = (y2-y1)*(1+bias)*(1-tension)/2;
   m1 += (y3-y2)*(1-bias)*(1-tension)/2;
   a0 =  2*mu3 - 3*mu2 + 1;
   a1 =    mu3 - 2*mu2 + mu;
   a2 =    mu3 -   mu2;
   a3 = -2*mu3 + 3*mu2;

   return(a0*y1+a1*m0+a2*m1+a3*y2);
}

/// addresses data and reads from file
void ReadFrameData(PointCacheCore *pCacheCore, int frameNumber, bool useYUp, int numDataPoints, CVector3Array & returnArray)
{
	pCacheCore->AddressData(frameNumber, numDataPoints );	
	for (int i=0; i<numDataPoints; ++i )
	{	
			double x,y,z, helper;
			pCacheCore->ReadDouble(x);
			pCacheCore->ReadDouble(y);
			pCacheCore->ReadDouble(z);

			if ( ! useYUp )
			{
				helper = z;
				z = -y;
				y = helper;
			}

			returnArray[i].Set( x , y, z );
	}
}

/// Calculates final vertex positions based on frame, timewarp, offset, influence and interpolations parameters
void CalculatePosArray(PointCacheCore *pCacheCore, double currentFrame, double timeWarp, double offset, CVector3Array &posArray, bool useYUp, int interpolation, double hermite_tension, double hermite_bias)
{
	int numDataPoints = (int) posArray.GetCount();

	//pCacheCore->GetNumPoints(numDataPoints);
	double sampleRate;
	pCacheCore->GetSampleRate(sampleRate);
	int numSamples;
	pCacheCore->GetNumSamples(numSamples);
	double startTime;
	pCacheCore->GetStartTime(startTime);

	currentFrame = ( currentFrame - offset ) * timeWarp;

	if ( currentFrame < startTime )
		currentFrame = startTime;

	double frameInBuffer = (currentFrame - startTime) * (1/sampleRate);

	

	if ( frameInBuffer > numSamples -1 )
	  frameInBuffer = numSamples -1;

	int firstFrameInBuffer = (int) floor(frameInBuffer);
	double nextFrameRatio = frameInBuffer - firstFrameInBuffer;

	if ( nextFrameRatio == 0.0 ) // simplest case no interpolation required
	  ReadFrameData(pCacheCore, firstFrameInBuffer, useYUp, numDataPoints, posArray);
	else
	{
		CVector3Array current_posArray(numDataPoints); // current positions
		CVector3Array next_posArray(numDataPoints); // current positions
		CVector3Array before_posArray(numDataPoints); // before positions
		CVector3Array after_posArray(numDataPoints); // after positions

		int nextFrameInBuffer = (int) ceil(frameInBuffer);
		// we need interpolation and maybe need the point positions before and after as well
		if (interpolation > 1 ) //cubic/hermite/bezier
		{
			int beforeFrameInBuffer = max(0, firstFrameInBuffer - 1);
			int afterFrameInBuffer = min(numSamples -1, nextFrameInBuffer  + 1);

			// fill before array
			ReadFrameData(pCacheCore, beforeFrameInBuffer, useYUp, numDataPoints, before_posArray);
			// fill after array
			ReadFrameData(pCacheCore, afterFrameInBuffer, useYUp, numDataPoints, after_posArray);
		}

		ReadFrameData(pCacheCore, firstFrameInBuffer, useYUp, numDataPoints, current_posArray);
		ReadFrameData(pCacheCore, nextFrameInBuffer, useYUp, numDataPoints, next_posArray);
		
		// all needed data acquired, let's do interpolation
		double x,y,z;
		
		if ( interpolation == 0 ) // linear
		{
			for (int i=0; i<numDataPoints; ++i )
			{	
				x = LinearInterpolate(current_posArray[i].GetX(),next_posArray[i].GetX(), nextFrameRatio);
				y = LinearInterpolate(current_posArray[i].GetY(),next_posArray[i].GetY(), nextFrameRatio);
				z = LinearInterpolate(current_posArray[i].GetZ(),next_posArray[i].GetZ(), nextFrameRatio);
				posArray[i].Set( x , y, z );	
			}
		}
		else
		if ( interpolation == 1 ) // cosine
		{
			for (int i=0; i<numDataPoints; ++i )
			{	
				x = CosineInterpolate(current_posArray[i].GetX(),next_posArray[i].GetX(), nextFrameRatio);
				y = CosineInterpolate(current_posArray[i].GetY(),next_posArray[i].GetY(), nextFrameRatio);
				z = CosineInterpolate(current_posArray[i].GetZ(),next_posArray[i].GetZ(), nextFrameRatio);
				posArray[i].Set( x , y, z );	
			}
		}
		else
		if ( interpolation == 2 ) // cubic
		{
			for (int i=0; i<numDataPoints; ++i )
			{
				x = CubicInterpolate(before_posArray[i].GetX(), current_posArray[i].GetX(),next_posArray[i].GetX(), after_posArray[i].GetX(), nextFrameRatio);
				y = CubicInterpolate(before_posArray[i].GetY(), current_posArray[i].GetY(),next_posArray[i].GetY(), after_posArray[i].GetY(), nextFrameRatio);
				z = CubicInterpolate(before_posArray[i].GetZ(), current_posArray[i].GetZ(),next_posArray[i].GetZ(), after_posArray[i].GetZ(), nextFrameRatio);
				posArray[i].Set( x , y, z );	
			}
		}
		else
		if ( interpolation == 3 ) // hermite
		{
			for (int i=0; i<numDataPoints; ++i )
			{
				x = HermiteInterpolate(before_posArray[i].GetX(), current_posArray[i].GetX(),next_posArray[i].GetX(), after_posArray[i].GetX(), nextFrameRatio, hermite_tension, hermite_bias);
				y = HermiteInterpolate(before_posArray[i].GetY(), current_posArray[i].GetY(),next_posArray[i].GetY(), after_posArray[i].GetY(), nextFrameRatio, hermite_tension, hermite_bias);
				z = HermiteInterpolate(before_posArray[i].GetZ(), current_posArray[i].GetZ(),next_posArray[i].GetZ(), after_posArray[i].GetZ(), nextFrameRatio, hermite_tension, hermite_bias);
				posArray[i].Set( x , y, z );
			}
		}
		else
		if ( interpolation == 4 ) // bezier for future use
		{
			for (int i=0; i<numDataPoints; ++i )
			{
				x = BezierInterpolate(before_posArray[i].GetX(), current_posArray[i].GetX(),next_posArray[i].GetX(), after_posArray[i].GetX(),  nextFrameRatio);
				y = BezierInterpolate(before_posArray[i].GetY(), current_posArray[i].GetY(),next_posArray[i].GetY(), after_posArray[i].GetY(),  nextFrameRatio);
				z = BezierInterpolate(before_posArray[i].GetZ(), current_posArray[i].GetZ(),next_posArray[i].GetZ(), after_posArray[i].GetZ(),  nextFrameRatio);
				posArray[i].Set( x , y, z );
			}
		}
	}
}

