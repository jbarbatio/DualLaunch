#pragma once
#include <string>
namespace duallaunch::cmdline {
	
		bool Exists();
		std::string Get();
		void Set(const char* cmdLine);

}