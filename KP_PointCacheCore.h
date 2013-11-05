// ===========================================================================
//
//	KP_PointCacheCore.h
//
//	Copyright (C) 2007-2008, Kai Wolter
//	http://www.kai-wolter.com
//	mail@kai-wolter.com
//
//	Created: 2008/01/20 (y/m/d)
//
//	--------------------------------------------------------------------------
//
//	This program is free software: you can redistribute it and/or modify
//	it under the terms of the GNU General Public License as published by
//   	the Free Software Foundation, either version 3 of the License, or
//	(at your option) any later version.
//
//	This program is distributed in the hope that it will be useful,
//    	but WITHOUT ANY WARRANTY; without even the implied warranty of
//    	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    	GNU General Public License for more details.
//
//	You should have received a copy of the GNU General Public License
//	along with this program; if not, see <http://www.gnu.org/licenses/>.
//
// ===========================================================================

#ifndef _PointCacheCore_
#define _PointCacheCore_

#include <fstream>
using namespace std;

#define LOAD_OPT_STREAM 0 
#define LOAD_OPT_FRAME 1
#define LOAD_OPT_BUFFER 2

#define SIZE_FLOAT 4
#define SIZE_INT 4

struct PointCacheHeader
{
	char* signature;	// cache file type
    int version;		// version number
	int numPoints;		// number of stored points (vertices)
	float startTime;	// first recorded frame
	float sampleRate;   // samples per frame
	int numSamples;		// number of recorded samples
};

class PointCacheCore
{
private:
	PointCacheHeader* header;

	ifstream infile;	
	ofstream outfile;	
	int dataSize;
	char * buffer;
	int pcType;
	int bufferAddress;
	char cacheFilename[500];

public:
	PointCacheCore();
	int OpenCache(char* filename, int loadOption = 0);

	int ReadFloat(float& value);
	int ReadInt(int& value);
	int ReadDouble(double& value);

	int WriteCache(char* filename, int numPoints, float startFrame, float sampleRate, int numSamples, int pcType = 0);

	int Write(float data);
	int Write(int data);
	int Write(double data);

	int Close();

	int AddressData(int frame, int points);

	int GetVersion(int& value);
	int GetNumPoints(int& value);
	int GetStartTime(double& value);
	int GetSampleRate(double& value);
	int GetNumSamples(int& value);
	
	void DeleteBuffer();
	
	bool IsReadingCache();
	bool IsWritingCache();
	
	int GetPCType();
};

#endif