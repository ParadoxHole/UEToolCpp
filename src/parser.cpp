#include "parser.h"
#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

void showProjectInfo(const std::string& uprojectPath) {
    std::ifstream file(uprojectPath);
    if (!file) {
        std::cerr << "Error: Unable to open " << uprojectPath << std::endl;
        return;
    }

    json project;
    try {
        file >> project;
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to parse the JSON file. Exception: " << e.what() << std::endl;
        return;
    }

    // 🔹 1. Display the GAME NAME (Using the main module's name)
    std::cout << "Game Name: ";
    if (project.contains("Modules") && project["Modules"].is_array() && !project["Modules"].empty()) {
        std::cout << project["Modules"][0]["Name"].get<std::string>() << std::endl;
    } else {
        std::cout << "Game name not found." << std::endl; // Case where the project is not in C++
    }

    // 🔹 2. Display the Unreal Engine VERSION USED
    std::cout << "Unreal Engine Version: ";
    if (project.contains("EngineAssociation") && project["EngineAssociation"].is_string()) {
        std::string engineVersion = project["EngineAssociation"].get<std::string>();
        std::cout << engineVersion;

        // Check if Unreal Engine is compiled "From Source"
        if (engineVersion.find("{") != std::string::npos) {
            std::cout << " (From Source)";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Not specified." << std::endl;
    }

    // 🔹 3. Display the PLUGINS USED
    std::cout << "Plugins used:" << std::endl;
    if (project.contains("Plugins") && project["Plugins"].is_array()) {
        for (const auto& plugin : project["Plugins"]) {
            if (plugin.contains("Name") && plugin["Name"].is_string()) {
                std::cout << "- " << plugin["Name"].get<std::string>() << std::endl;
            }
        }
    } else {
        std::cout << "No plugins detected." << std::endl;
    }
}