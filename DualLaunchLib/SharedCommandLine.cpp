#include "pch.h"
#include "SharedCommandLine.h"
#include <fstream>
#include <sstream>
#include <filesystem>
namespace fs = std::filesystem;

fs::path GetRoamingSnatch() {
	auto path = std::filesystem::temp_directory_path()
		.parent_path()
		.parent_path();

	path /= "Roaming";
	path /= "DualLaunch";

	if (!std::filesystem::exists(path))
		std::filesystem::create_directories(path);
	return path;
}
bool duallaunch::cmdline::Exists() {
	fs::path path = GetRoamingSnatch();
	path /= "cmdLine.txt";
	return fs::exists(path);
}

std::string duallaunch::cmdline::Get()
{
	fs::path path = GetRoamingSnatch();
	path /= "cmdLine.txt";
	std::ifstream t(path);
	std::stringstream buffer;
	buffer << t.rdbuf();
	t.close();
	_unlink(path.string().c_str());
	return buffer.str();
}

void duallaunch::cmdline::Set(const char* cmdLine)
{
	fs::path path = GetRoamingSnatch();
	path /= "cmdLine.txt";
	_unlink(path.string().c_str());
	std::ofstream myfile(path);
	myfile << cmdLine;
	myfile.close();


}

