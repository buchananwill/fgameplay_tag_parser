//
// Created by thele on 22/07/2025.
//

#include "gameplay_tag_scalar_field_accumulator.h"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "flags.h"
#include "../helpers.h"

#include "ScalarFieldFormatTemplates.h"

using namespace Templates::ScalarField;


void gameplay_tag_scalar_field_accumulator::buffer_node_strings(const TagNode& node) {
	includes_buffer.append(std::format(include_wrapper, node.Name));
	includes_buffer.append("\n");

	const auto underscore_name = helpers::join(segments, "_");
	const auto dot_name = helpers::join(segments, ".");

	if_emitter_branches_buffer.append(std::format(if_emitter_template, underscore_name, node.Name));
	if_accumulator_branches_buffer.append(std::format(if_accumulator_template, underscore_name, node.Name));
	if_field_branches_buffer.append(std::format(if_field_template, underscore_name, node.Name));

	if (!underscore_list_buffer.empty()) {
		underscore_list_buffer.append(",\n            ");
	}
	underscore_list_buffer.append(underscore_name);
}

void gameplay_tag_scalar_field_accumulator::write_fragments_file(const TagNode& node, std::ofstream& stream) const {
	stream << std::format(fragments_template, node.Name);
}

void gameplay_tag_scalar_field_accumulator::write_processor_header_file(const TagNode& node,
                                                                        std::ofstream& stream) const {
	stream << std::format(header_template, node.Name);
}

void gameplay_tag_scalar_field_accumulator::write_processor_cpp_file(const TagNode& node, std::ofstream& stream) const {
	stream << std::format(cpp_template, node.Name);
}

std::string gameplay_tag_scalar_field_accumulator::format_canonical_list(
	const std::string& underscore_list_buffer) const {
	return std::format(canonical_scalar_fields, underscore_list_buffer);
}

std::string gameplay_tag_scalar_field_accumulator::format_tag_to_dispatch() const {
	return std::format(tag_to_fragment_dispatch, includes_buffer, if_accumulator_branches_buffer,
	                   if_field_branches_buffer, if_emitter_branches_buffer);
}
