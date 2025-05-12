//
// Created by thele on 12/05/2025.
//

#ifndef TAGNODE_H
#define TAGNODE_H
#include <string>
#include <vector>

struct TagNode {
	std::string Name; // Tag name token
	std::string Comment; // Raw comment text (optional)
	std::vector<std::shared_ptr<TagNode>> Children; // Child nodes
	std::shared_ptr<TagNode> Parent = nullptr; // Parent pointer for convenience
};

#endif //TAGNODE_H
