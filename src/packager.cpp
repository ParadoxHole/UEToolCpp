#include "packager.h"
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <algorithm>

#ifdef _WIN32
    #define UAT_SCRIPT "Engine/Build/BatchFiles/RunUAT.bat"
#else
    #define UAT_SCRIPT "Engine/Build/BatchFiles/RunUAT.sh"
#endif

namespace fs = std::filesystem;

void packageProject(const std::string& uprojectPath, const std::string& packagePath) {
    // Get the directory containing the .uproject file
    fs::path projectDir = fs::path(uprojectPath).parent_path();

    // Search upwards for the "Engine" directory
    fs::path unrealEnginePath;
    fs::path currentPath = projectDir;

    while (!currentPath.empty()) {
        fs::path potentialEnginePath = currentPath / "Engine";
        if (fs::exists(potentialEnginePath) && fs::is_directory(potentialEnginePath)) {
            unrealEnginePath = currentPath;
            break;
        }
        currentPath = currentPath.parent_path();
    }

    // If we didn't find the Engine folder, return an error
    if (unrealEnginePath.empty()) {
        std::cerr << "Error: Could not find Unreal Engine directory from " << uprojectPath << std::endl;
        return;
    }

    // Build the full path to the correct RunUAT script
    fs::path uatScript = unrealEnginePath / UAT_SCRIPT;

    // Verify the script exists
    if (!fs::exists(uatScript)) {
        std::cerr << "Error: RunUAT script not found at " << uatScript << std::endl;
        return;
    }

    // ✅ Convert paths for Windows or macOS/Linux
    std::string uatScriptStr = uatScript.string();
    std::string projectPathStr = uprojectPath;
    std::string packagePathStr = packagePath;

#ifdef _WIN32
    // Windows uses backslashes
    std::replace(uatScriptStr.begin(), uatScriptStr.end(), '/', '\\');
    std::replace(projectPathStr.begin(), projectPathStr.end(), '/', '\\');
    std::replace(packagePathStr.begin(), packagePathStr.end(), '/', '\\');

    // Correct Windows packaging command
    std::string command = uatScriptStr + " BuildCookRun -project=" + projectPathStr +
                          " -noP4 -platform=Win64 -clientconfig=Development -cook -allmaps -build -stage -pak -archive -archivedirectory=" + packagePathStr;
#else
    // macOS/Linux uses forward slashes
    std::replace(uatScriptStr.begin(), uatScriptStr.end(), '\\', '/');
    std::replace(projectPathStr.begin(), projectPathStr.end(), '\\', '/');
    std::replace(packagePathStr.begin(), packagePathStr.end(), '\\', '/');

    // Correct macOS/Linux packaging command
    std::string command = "bash " + uatScriptStr + " BuildCookRun -project=" + projectPathStr +
                          " -noP4 -platform=Mac -clientconfig=Development -cook -allmaps -build -stage -pak -archive -archivedirectory=" + packagePathStr;
#endif

    // Debug output
    std::cout << "Executing command: " << command << std::endl;

    // Run the command
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "Packaging succeeded!" << std::endl;
    } else {
        std::cerr << "Packaging failed!" << std::endl;
    }
}
