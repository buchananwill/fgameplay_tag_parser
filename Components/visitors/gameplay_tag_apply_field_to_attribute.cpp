//
// Created by thele on 23/07/2025.
//

#include "gameplay_tag_apply_field_to_attribute.h"

#include "FieldAttributeProcessorTemplates.h"
#include "../helpers.h"

void gameplay_tag_apply_field_to_attribute::buffer_node_strings(const TagNode& node) {
}

void gameplay_tag_apply_field_to_attribute::write_fragments_file(const TagNode& node, std::ofstream& stream) const {

}

void gameplay_tag_apply_field_to_attribute::write_processor_header_file(const TagNode& node,
                                                                        std::ofstream& stream) const {
	stream << std::format(Templates::FieldAttribute::header_template, node.Name);
}

void gameplay_tag_apply_field_to_attribute::write_processor_cpp_file(const TagNode& node, std::ofstream& stream) const {
	const auto tag_full_name = helpers::join(segments, "_");
	stream << std::format(Templates::FieldAttribute::cpp_template, node.Name, tag_full_name);
}

std::string gameplay_tag_apply_field_to_attribute::format_canonical_list(
	const std::string& underscore_list_buffer) const {
	return "";
}

std::string gameplay_tag_apply_field_to_attribute::format_tag_to_dispatch() const {
	return "";
}
