#include <iostream>
#include "parser.h"
#include "builder.h"
#include "packager.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage : UEToolCpp [CHEMIN DU UPROJECT] [ACTION]\n";
        return 1;
    }

    std::string uprojectPath = argv[1];
    std::string action = argv[2];

    if (action == "show-infos") {
        showProjectInfo(uprojectPath);
    } else if (action == "build") {
        buildProject(uprojectPath);
    } else if (action == "package") {
        if (argc < 4) {
            std::cerr << "Usage : UEToolCpp [CHEMIN DU UPROJECT] package [CHEMIN DU PACKAGE]\n";
            return 1;
        }
        std::string packagePath = argv[3];
        packageProject(uprojectPath, packagePath);
    } else {
        std::cerr << "Action inconnue : " << action << std::endl;
    }

    return 0;
}