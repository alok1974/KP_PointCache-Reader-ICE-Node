// ===========================================================================
//
//	KP_PointCacheCore.cpp
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <assert.h>

#include "KP_PointCacheCore.h"

//////////////////////////////////////////////////////////////////////////////
//
// PointCacheCore constructor
//
//////////////////////////////////////////////////////////////////////////////
PointCacheCore::PointCacheCore()
{
	header = NULL;
	buffer = NULL;
	dataSize = 0;
	pcType = 0;
	bufferAddress = 0;
	strcpy(cacheFilename, "");
}

//////////////////////////////////////////////////////////////////////////////
//
// Opens a cache file with the set loadOption
//
//////////////////////////////////////////////////////////////////////////////
int PointCacheCore::OpenCache(char* filename, int loadOption)
{
	if ( strcmp(filename, "") == 0 )
	{
		strcpy(cacheFilename, "");
		return -1;
	}

    if ( ( strcmp(filename, cacheFilename) != 0 ) )
	{
		if ( buffer )
			delete buffer;

		buffer = NULL;
		strcpy(cacheFilename, "");
		if ( infile.is_open() ) 
			infile.close();
	}

	if ( buffer && loadOption < 2 )
	{
		delete buffer;
		buffer = NULL;
	}

	if ( buffer && loadOption == 2 )
		return 1;

	if ( infile.is_open() && loadOption == 0 )
		return 1;

	if ( loadOption == 2 && ! buffer )
	{
		if ( infile.is_open() )
			infile.close();
	}

	if ( ! infile.is_open() )
	{
		infile.clear();
		infile.open(filename, ios::in | ios::binary);
	}
	else return 1;
	if ( ! infile.is_open() )
	{
	  strcpy(cacheFilename, "");
	  return -1;
	}

	strcpy(cacheFilename, filename);

	this->pcType = 0;
	infile.seekg (0, ios::beg);
	
	char cacheSignature[12];
	infile.read( (char*)&cacheSignature, 12);

	if ( strcmp(cacheSignature, "POINTCACHE2" ) == 0 )
	  this->pcType = 1;
	
	infile.seekg(0, ios::end);
	dataSize = infile.tellg();
	infile.seekg (0, ios::beg);
	
	if ( this->pcType == 0 ) // max 8 default point cache modifier compatible header (pts)
	{
		// init header
		header = new PointCacheHeader();
		infile.read( (char*) &(header->version), SIZE_INT );
		
		int sRate;
		infile.read( (char*) &sRate, SIZE_INT );
		
		header->sampleRate = (float) 1.0 / sRate;
		infile.read( (char*) &(header->numSamples), SIZE_INT );
		
		infile.seekg(dataSize-4);
		infile.read( (char*) &(header->numPoints), SIZE_INT );
		
		infile.seekg(12);
	}
	if ( this->pcType == 1 ) // point cache 2 [Max9 point cache] header (pc2)
	{
		header = new PointCacheHeader();
		infile.read( (char*)&header->signature, 12); // todo make dynamic
		infile.read( (char*) &(header->version), SIZE_INT );
		
		infile.read( (char*) &(header->numPoints), SIZE_INT );
		
		infile.read( (char*) &(header->startTime), SIZE_FLOAT );
		
		infile.read( (char*) &(header->sampleRate), SIZE_FLOAT );
		
		infile.read( (char*) &(header->numSamples), SIZE_INT );		
	}

	if ( loadOption == LOAD_OPT_BUFFER ) // load cache in memory
	{
		filebuf *pbuf;
		pbuf = infile.rdbuf();
		buffer = new char[dataSize];
		pbuf->sgetn (buffer,dataSize);
		infile.close();

	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////////
//
// depending on the loadOption reads a float value from file or buffer
//
//////////////////////////////////////////////////////////////////////////////
int PointCacheCore::ReadFloat(float& data)
{
	if ( buffer )
	{
		data = *(reinterpret_cast<float*>(&buffer[bufferAddress]));
		bufferAddress += 4;
		return 1;
	}

	infile.read( (char*) &data, SIZE_FLOAT );
	return 1;
}

//////////////////////////////////////////////////////////////////////////////
//
// depending on the loadOption reads an int value from file or buffer
//
//////////////////////////////////////////////////////////////////////////////
int PointCacheCore::ReadInt(int& data)
{
	if ( buffer )
	{
		data = *(reinterpret_cast<int*>(&buffer[bufferAddress]));
		bufferAddress += 4;
		return 1;
	}
	
	infile.read( (char*) &data, SIZE_INT );
	return 1;
}


//////////////////////////////////////////////////////////////////////////////
//
// depending on the loadOption reads a double value from file or buffer
//
//////////////////////////////////////////////////////////////////////////////
int PointCacheCore::ReadDouble(double& data)
{
	if ( buffer )
	{
		data = *(reinterpret_cast<float*>(&buffer[bufferAddress]));
		bufferAddress += SIZE_FLOAT;
		return 1;
	}

	float v;
	infile.read( (char*) &v, SIZE_FLOAT );
	data = v;
	return 1;
}

//////////////////////////////////////////////////////////////////////////////
//
// write a cache file header with the specified attributes
//
//////////////////////////////////////////////////////////////////////////////
int PointCacheCore::WriteCache(char* filename, int numPoints, float startFrame, float sampleRate, int numSamples, int pcType)
{
	if ( IsWritingCache() )
		return 0;

	outfile.open(filename, ios::binary);
	

	if ( ! outfile.is_open() )
	  return -1;

	this->pcType = pcType;
	if ( pcType == 0 ) // max 8 (pts) default point cache modifier compatible header
	{
		// init header
		header = new PointCacheHeader();
		header->numPoints = numPoints;
		header->startTime = startFrame;
		header->sampleRate = sampleRate;
		header->numSamples = numSamples;
		header->version = -100;

		int sRate = (int) sampleRate; // max pts sample rates are in the interval [1;10]
		outfile.write( (char*) &(header->version), SIZE_INT );
		outfile.write( (char*) &sRate, SIZE_INT );
		outfile.write( (char*) &(header->numSamples), SIZE_INT );
	}
	if ( pcType == 1  ) // john buernett (pc2/max9 point cache) compatible point cache header
	{
		// init header
		header = new PointCacheHeader();
		header->numPoints = numPoints;
		header->startTime = startFrame;
		header->sampleRate = sampleRate;
		header->numSamples = numSamples;
		header->signature = "POINTCACHE2";
		header->version = 1;

		// write header
		char  cacheSignature[12] = "POINTCACHE2";
		outfile.write( (char*) cacheSignature, 12); // todo make dynamic
		outfile.write( (char*) &(header->version), SIZE_INT );
		outfile.write( (char*) &(header->numPoints), SIZE_INT );
		outfile.write( (char*) &(header->startTime), SIZE_FLOAT );
		outfile.write( (char*) &(header->sampleRate), SIZE_FLOAT );
		outfile.write( (char*) &(header->numSamples), SIZE_INT );
	}
	return 1;
}

//////////////////////////////////////////////////////////////////////////////
//
// writes a float value to file
//
//////////////////////////////////////////////////////////////////////////////
int PointCacheCore::Write(float data)
{
	if ( ! IsWritingCache() )
		return 0;
	outfile.write((char*)&data, SIZE_FLOAT);
	return 1;
}

//////////////////////////////////////////////////////////////////////////////
//
// writes an int value to file
//
//////////////////////////////////////////////////////////////////////////////
int PointCacheCore::Write(int data)
{
	if ( ! IsWritingCache() )
		return 0;
	outfile.write((char*)&data, SIZE_INT);
	return 1;
}

//////////////////////////////////////////////////////////////////////////////
//
// writes a double value to file
//
//////////////////////////////////////////////////////////////////////////////
int PointCacheCore::Write(double data)
{
	if ( ! IsWritingCache() )
		return 0;
	float v = (float) data ;
		outfile.write((char*)&v, SIZE_FLOAT);

	return 1;
}

//////////////////////////////////////////////////////////////////////////////
//
// checks if file is opened for reading or writing, closes it and cleans up buffers
//
//////////////////////////////////////////////////////////////////////////////
int PointCacheCore::Close()
{
	if ( IsWritingCache() )
	{
		if ( this->pcType == 0 )
		{
			for ( int i=0; i < header->numSamples; i++ )
			{
				outfile.write( (char*) &(header->numPoints), SIZE_INT );
			}
		}
		outfile.flush();
		outfile.close();
		return 1;
	}

	if ( IsReadingCache() )
	{
		if ( buffer )
		{
			delete buffer;
			buffer = NULL;
		}
		infile.close();
		return 1;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////
//
// retrieves point cache version number
//
//////////////////////////////////////////////////////////////////////////////
int PointCacheCore::GetVersion(int& value)
{
	value = header->version;
	return 1;
}

//////////////////////////////////////////////////////////////////////////////
//
// retrieves cached geometry point count
//
//////////////////////////////////////////////////////////////////////////////
int PointCacheCore::GetNumPoints(int& value)
{
	value = header->numPoints;
	return 1;
}

//////////////////////////////////////////////////////////////////////////////
//
// retrieves number of first frame of the cached interval
//
//////////////////////////////////////////////////////////////////////////////
int PointCacheCore::GetStartTime(double& value)
{
	if ( ! header )
		return 0;

	value = header->startTime;

	return 1;
}

//////////////////////////////////////////////////////////////////////////////
//
// retrieves number of samples per frame
//
//////////////////////////////////////////////////////////////////////////////
int PointCacheCore::GetSampleRate(double& value)
{
	if ( ! header )
		return 0;

	value = header->sampleRate;

	return 1;
}

//////////////////////////////////////////////////////////////////////////////
//
// retrieves number of samples stored in cache file
//
//////////////////////////////////////////////////////////////////////////////
int PointCacheCore::GetNumSamples(int& value)
{
	if ( ! header )
		return 0;
	
	value = header->numSamples;

	return 1;
}

//////////////////////////////////////////////////////////////////////////////
//
// checks if file is open
//
//////////////////////////////////////////////////////////////////////////////
bool PointCacheCore::IsReadingCache()
{
	return ( infile.is_open() );
}

//////////////////////////////////////////////////////////////////////////////
//
// checks if file is open for writing
//
//////////////////////////////////////////////////////////////////////////////
bool PointCacheCore::IsWritingCache()
{
  return ( outfile && outfile.is_open() );
}

//////////////////////////////////////////////////////////////////////////////
//
// seeks to the position in the file defined by the frame (frame defined in samples) attribute 
//
//////////////////////////////////////////////////////////////////////////////
int PointCacheCore::AddressData(int frame, int points)
{
	int header = 12;
	if ( pcType == 1 )
		header = 32;

	int bytesPerVertex = 12;

	// casting to __int64 for files more than 2gb on win64
	__int64 header_ =  static_cast<__int64>(header);
	__int64 bytesPerVertex_ =  static_cast<__int64>(bytesPerVertex);
	__int64 points_ =  static_cast<__int64>(points);
	__int64 frame_ =  static_cast<__int64>(frame);
	__int64 fp =  static_cast<__int64>(header_ + (frame_ * points_ * bytesPerVertex_));
	
	if ( buffer )
		this->bufferAddress = ((frame * points * bytesPerVertex));
	else
		infile.seekg(fp);
	return 1;
}

//////////////////////////////////////////////////////////////////////////////
//
// returns the point cache type currently used, 0 = pts, 1 = pc2
//
//////////////////////////////////////////////////////////////////////////////
int PointCacheCore::GetPCType()
{
	return this->pcType;
}

//////////////////////////////////////////////////////////////////////////////
//
// deletes the memory buffer
//
//////////////////////////////////////////////////////////////////////////////
void PointCacheCore::DeleteBuffer()
{
	if ( this->buffer )
	{
		delete ( buffer );
	}
	this->buffer = NULL;
}