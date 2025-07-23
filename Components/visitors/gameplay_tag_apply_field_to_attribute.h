//
// Created by thele on 23/07/2025.
//

#ifndef GAMEPLAY_TAG_APPLY_FIELD_TO_ATTRIBUTE_H
#define GAMEPLAY_TAG_APPLY_FIELD_TO_ATTRIBUTE_H
#include "flags.h"
#include "gameplay_tag_to_fragment_visitor.h"


class gameplay_tag_apply_field_to_attribute : public gameplay_tag_to_fragment_visitor {
public:
	gameplay_tag_apply_field_to_attribute(
		const fs::path& input_path
		, const std::string& _output_unit
		)
		: gameplay_tag_to_fragment_visitor(
			input_path
			, _output_unit
			, "FieldAttribute"
			) {
		flags = { visitor::flags::supplier_attribute, visitor::flags::scalar_field};
	}

protected:
	void buffer_node_strings(const TagNode& node) override;

	void write_fragments_file(const TagNode& node, std::ofstream& stream) const override;

	void write_processor_header_file(const TagNode& node, std::ofstream& stream) const override;

	void write_processor_cpp_file(const TagNode& node, std::ofstream& stream) const override;

	std::string format_canonical_list(const std::string& underscore_list_buffer) const override;

	std::string format_tag_to_dispatch() const override;
};



#endif //GAMEPLAY_TAG_APPLY_FIELD_TO_ATTRIBUTE_H
