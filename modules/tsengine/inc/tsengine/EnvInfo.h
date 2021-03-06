/*
	System Information
*/

#pragma once

#include <tscore/types.h>
#include <string>

namespace ts
{
	enum ECpuVendorID : uint8
	{
		eCpuUnknown	= 0,
		eCpuIntel	= 1,
		eCpuAMD		= 2
	};

	enum ECPUArchitecture : uint8
	{
		eCPUarchX86 = 1,
		eCPUarchAMD64 = 2,
		eCPUarchARM = 3
	};

	struct SSystemInfo
	{
		std::string userName;
		std::string osName;
		//uint32 osVersion;
		
		std::string cpuName;
		//std::string cpuVendorName;
		ECpuVendorID cpuVendorID;
		ECPUArchitecture cpuArchitecture;
		uint32 cpuProcessorCount;
		//uint32 cpuFrequency; //Measured in MHz
		
		uint32 numDisplays;
		//uint32 numDisplayAdapters;
	};
	
	struct SSystemMemoryInfo
	{
		//Measured in bytes
		uint64 mCapacity;
		uint64 mUsed;
	};
	
	struct SDisplayInfo
	{
		uint32 height;
		uint32 width;
		uint8 colourDepth;
	};
}
