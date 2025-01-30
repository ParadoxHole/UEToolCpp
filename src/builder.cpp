#include "builder.h"
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <algorithm>

#ifdef _WIN32
    #define BUILD_SCRIPT "Engine/Build/BatchFiles/Build.bat"
    #define PLATFORM "Win64"
#else
    #define BUILD_SCRIPT "Engine/Build/BatchFiles/Linux/Build.sh"
    #define PLATFORM "Mac/Linux"
#endif

namespace fs = std::filesystem;

void buildProject(const std::string& uprojectPath) {
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

    // Build the full path to the correct Build script
    fs::path buildScript = unrealEnginePath / BUILD_SCRIPT;

    // Verify that Build.bat or Build.sh exists
    if (!fs::exists(buildScript)) {
        std::cerr << "Error: Build script not found at " << buildScript << std::endl;
        return;
    }

    // ✅ Convert paths for Windows or macOS/Linux
    std::string buildScriptStr = buildScript.string();
    std::string projectPathStr = uprojectPath;

#ifdef _WIN32
    // Windows requires backslashes
    std::replace(buildScriptStr.begin(), buildScriptStr.end(), '/', '\\');
    std::replace(projectPathStr.begin(), projectPathStr.end(), '/', '\\');

    // Windows build command
    std::string command = buildScriptStr + " MyProjectEditor " PLATFORM " Development -project=" + projectPathStr;
#else
    // macOS/Linux uses forward slashes
    std::replace(buildScriptStr.begin(), buildScriptStr.end(), '\\', '/');
    std::replace(projectPathStr.begin(), projectPathStr.end(), '\\', '/');

    // macOS/Linux build command (executed via bash)
    std::string command = "bash " + buildScriptStr + " MyProjectEditor " PLATFORM " Development -project=" + projectPathStr;
#endif

    // Debug output
    std::cout << "Executing command: " << command << std::endl;

    // Run the command
    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "Build succeeded!" << std::endl;
    } else {
        std::cerr << "Build failed!" << std::endl;
    }
}
