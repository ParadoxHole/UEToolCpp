cmake_minimum_required(VERSION 3.10)
project(UEToolCpp)

set(CMAKE_CXX_STANDARD 17)

# Ajouter le chemin de vcpkg (modifie-le selon ton installation)
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Ajout des fichiers source
add_executable(UEToolCpp src/main.cpp src/parser.cpp src/builder.cpp src/packager.cpp)

# Lier nlohmann/json
find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(UEToolCpp PRIVATE nlohmann_json::nlohmann_json)