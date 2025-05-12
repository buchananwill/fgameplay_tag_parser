// GameplayTagGenerator.cpp
// A utility class to parse an indentation‑based text file describing gameplay tags
// and emit UE_DECLARE_GAMEPLAY_TAG_EXTERN / UE_DEFINE_GAMEPLAY_TAG_COMMENT macros
// into companion .h and .cpp files.
//

#include <fstream>
#include <iostream>

#include "Components/gameplay_tag_generator.h"

// Optional: simple command‑line entry point when building as a standalone tool
int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cout << "Usage: GameplayTagGenerator <EnvVarName> [OutputUnit]" << std::endl;
		return 1;
	}
	gameplay_tag_generator gen;
	if (!gen.generate_from_env(argv[1], argc > 2 ? argv[2] : "")) {
		return 2;
	}
	return 0;
}
