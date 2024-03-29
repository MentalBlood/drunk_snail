#include <string.h>
#include <stdbool.h>
#include <sys/types.h>

#include "../modules/prefix_tree/include/prefix_tree.h"

#include "../include/Line.h"
#include "../include/Other.h"
#include "../include/Template.h"
#include "../include/templates.h"
#include "../include/params_macros.h"

#include "../include/render.h"



void render(
	RenderResult *render_result,
	char *template_name,
	size_t template_name_length,
	char **output_end,
	size_t depth,
	size_t *buffer_size,
	OtherPointerList *other,
	size_t other_left_length,
	size_t other_right_length,
	DRUNK_TYPE params,
	Tree *templates_stack
)
{

	Template *template = dictionaryLookupUnterminated(templates, template_name, template_name_length);
	if ((template == NULL) || (!template->lines.length && template->length)) {
		if (render_result->result) {
			render_result->result = NULL;
		}
		drunk_malloc_one_render_(render_result->message, sizeof(char) * (template_name_length + 1));
		memcpy(render_result->message, template_name, template_name_length);
		render_result->message[template_name_length] = 0;
		return;
	}

	if (templates_stack) {
		if (dictionaryLookupUnterminated(templates_stack, template_name, template_name_length)) {
			if (render_result->result) {
				render_result->result = NULL;
			}
			drunk_malloc_one_render_(render_result->message, sizeof(char) * (template_name_length + 1));
			memcpy(render_result->message, template_name, template_name_length);
			render_result->message[template_name_length] = 0;
			return;
		}
		treeInsert(templates_stack, template_name, template);
	}

	bool alloc_error = false;

	if (!depth) {

		buffer_size = &template->buffer_size;
		drunk_malloc_one_render_(render_result->result, sizeof(char) * (*buffer_size));
		*output_end = render_result->result;

		other = malloc(sizeof(OtherPointerList));
		if (!other) {
			exit_render_();
		}
		listCreate(*other, Other*, 16, alloc_error);
		if (alloc_error) {
			exit_render_();
		}

	}

	char *value;
	DRUNK_LIST_LENGTH_TYPE j;
	DRUNK_LIST_LENGTH_TYPE list_size;
	DRUNK_STRING_LENGTH_TYPE value_size;
	DRUNK_TYPE values;

	size_t i;
	size_t required_buffer_size;
	Expression *expression;

	Line* line = template->lines.start;
	Line* end = line + template->lines.length;
	for (; line != end; line++) {
		renderLine((*line));
	}

	if (!depth) {
		listFree(*other);
		free(other);
	}

	if (templates_stack) {
		treeDetach(templates_stack, template_name);
	}

};