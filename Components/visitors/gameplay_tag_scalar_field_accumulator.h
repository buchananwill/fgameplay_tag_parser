//
// Created by thele on 22/07/2025.
//

#ifndef GAMEPLAY_TAG_SCALAR_FIELD_ACCUMULATOR_H
#define GAMEPLAY_TAG_SCALAR_FIELD_ACCUMULATOR_H
#include "flags.h"
#include "gameplay_tag_to_fragment_visitor.h"
#include "../tag_node_tree_visitor.h"


class gameplay_tag_scalar_field_accumulator : public gameplay_tag_to_fragment_visitor {
public:
	gameplay_tag_scalar_field_accumulator(const fs::path &input_path, const std::string &_output_unit)
	: gameplay_tag_to_fragment_visitor(
		input_path
		, _output_unit
		, "TagToScalarFieldFragmentDispatch.h"
		, "ScalarFieldTagList.h"
		, "ScalarFieldFragments.h"
		, "FieldAccumulator"
		)
	{
		flags = {visitor::flags::scalar_field};
	}

protected:

	std::string if_accumulator_branches_buffer;
	std::string if_field_emitters_branches_buffer;

	void buffer_node_strings(const TagNode &node) override;
	void write_fragments_file(const TagNode &node, std::ofstream &stream) const override;
	void write_processor_header_file(const TagNode &node, std::ofstream &stream) const override;
	void write_processor_cpp_file(const TagNode &node, std::ofstream &stream) const override;

	std::string format_canonical_list(const std::string &underscore_list_buffer) const override;
	std::string format_tag_to_dispatch() const override;
};



#endif //GAMEPLAY_TAG_SCALAR_FIELD_ACCUMULATOR_H
