//
// Created by thele on 07/07/2025.
//

#include "gameplay_tag_supplier_processor.h"

#include <format>
#include <fstream>
#include <iostream>

#include "SupplierAttributeFormatTemplates.h"
#include "../helpers.h"

namespace fs = std::filesystem;

using namespace Templates::Product;

void gameplay_tag_supplier_processor::buffer_node_strings(const TagNode& node) {
	includes_buffer.append(std::format(include_wrapper, node.Name));
	includes_buffer.append("\n");

	const auto underscore_name = helpers::join(segments, "_");
	const auto dot_name = helpers::join(segments, ".");

	if_attribute_branches_buffer.append(std::format(if_attribute_template, underscore_name, node.Name));
	if_archetypes_branches_buffer.append(std::format(if_archetypes_template, underscore_name, node.Name));

	if (!underscore_list_buffer.empty()) {
		underscore_list_buffer.append(",\n            ");
	}
	underscore_list_buffer.append(underscore_name);
}

void gameplay_tag_supplier_processor::write_fragments_file(const TagNode& node, std::ofstream& stream) const {
	stream << std::format(fragments_template, node.Name);
}

void gameplay_tag_supplier_processor::write_processor_header_file(const TagNode& node, std::ofstream& stream) const {
	stream << std::format(header_template, node.Name);
}

void gameplay_tag_supplier_processor::write_processor_cpp_file(const TagNode& node, std::ofstream& stream) const {
	stream << std::format(cpp_template, node.Name);
}

std::string gameplay_tag_supplier_processor::format_canonical_list(const std::string& underscore_list_buffer) const {
	return std::format(canonical_supplier_attributes, underscore_list_buffer);
}

std::string gameplay_tag_supplier_processor::format_tag_to_dispatch() const {
	return std::format(tag_to_fragment_dispatch, includes_buffer, if_attribute_branches_buffer,
	                   if_archetypes_branches_buffer);
}
