//
// Created by thele on 07/07/2025.
//

#ifndef GAMEPLAY_TAG_SUPPLIER_PROCESSOR_H
#define GAMEPLAY_TAG_SUPPLIER_PROCESSOR_H

#include "flags.h"
#include "gameplay_tag_to_fragment_visitor.h"
#include "../tag_node_tree_visitor.h"

class gameplay_tag_supplier_processor : public gameplay_tag_to_fragment_visitor {
public:
	gameplay_tag_supplier_processor(const fs::path &input_path, const std::string &_output_unit)
		: gameplay_tag_to_fragment_visitor(
			input_path
			, _output_unit
			, ""
			, "Fragments.h"
			, "TagToFragmentDispatch.h"
			, "SupplierAttributeTagList.h"
			) {
		flags = { visitor::flags::supplier_attribute};
	}

protected:
	void buffer_node_strings(const TagNode& node) override;

	void write_fragments_file(const TagNode& node, std::ofstream& stream) const override;

	void write_processor_header_file(const TagNode& node, std::ofstream& stream) const override;

	void write_processor_cpp_file(const TagNode& node, std::ofstream& stream) const override;

	std::string format_canonical_list(const std::string& underscore_list_buffer) const override;

	std::string format_tag_to_dispatch() const override;


private:

	std::string if_attribute_branches_buffer;
	std::string if_archetypes_branches_buffer;
};

#endif //GAMEPLAY_TAG_SUPPLIER_PROCESSOR_H
