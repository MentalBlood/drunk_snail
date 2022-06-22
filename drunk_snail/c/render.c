
/* #line 1 "compileComprehension_preprocessed.rl" */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <Python.h>

#include "List.h"
#include "Other.h"
#include "Template.h"
#include "templates.h"
#include "prefix_tree.h"
#include "RenderResult.h"




/* #line 21 "compileComprehension.c" */
static const int render_start = 0;
static const int render_first_final = 0;
static const int render_error = -1;

static const int render_en_main = 0;


/* #line 20 "compileComprehension_preprocessed.rl" */




#define render__empty(target, LINE, LINE_length) {\
	while ((*target - render_result->result) + (LINE_length+1+1) + subarrays_length >= *buffer_size) {\
		(*buffer_size) *= 2;\
		new_result = (char*)realloc(render_result->result, sizeof(char) * (*buffer_size));\
		*target = new_result + (*target - render_result->result);\
		render_result->result = new_result;\
	}\
	for (i = 0; i < depth; i++) {\
		memcpy(*target, (*other)[i].left.start, (*other)[i].left.length); *target += (*other)[i].left.length;\
	}\
	memcpy(*target, LINE, LINE_length); *target += LINE_length;\
	for (i = depth; i > 0; i--) {\
		memcpy(*target, (*other)[i-1].right.start, (*other)[i-1].right.length); *target += (*other)[i-1].right.length;\
	}\
	memcpy(*target, "\n", 1); *target += 1;\
};

#define render__arg(target, OTHER_LEFT, OTHER_LEFT_length, ARG, ARG_length, OTHER_RIGHT, OTHER_RIGHT_length) {\
	while ((*target - render_result->result) + (OTHER_LEFT_length+ARG_length+OTHER_RIGHT_length+1) + subarrays_length >= *buffer_size) {\
		(*buffer_size) *= 2;\
		new_result = (char*)realloc(render_result->result, sizeof(char) * (*buffer_size));\
		*target = new_result + (*target - render_result->result);\
		render_result->result = new_result;\
	}\
	memcpy(*target, OTHER_LEFT, OTHER_LEFT_length); *target += OTHER_LEFT_length;\
	memcpy(*target, ARG, ARG_length); *target += ARG_length;\
	memcpy(*target, OTHER_RIGHT, OTHER_RIGHT_length); *target += OTHER_RIGHT_length;\
};

#define render__param(target, OTHER_LEFT, OTHER_LEFT_length, ARG, ARG_length, OTHER_RIGHT, OTHER_RIGHT_length) {\
	while ((*target - render_result->result) + (OTHER_LEFT_length+ARG_length+OTHER_RIGHT_length+1+1) + subarrays_length >= *buffer_size) {\
		(*buffer_size) *= 2;\
		new_result = (char*)realloc(render_result->result, sizeof(char) * (*buffer_size));\
		*target = new_result + (*target - render_result->result);\
		render_result->result = new_result;\
	}\
	for (i = 0; i < depth; i++) {\
		memcpy(*target, (*other)[i].left.start, (*other)[i].left.length); *target += (*other)[i].left.length;\
	}\
	render__arg(target, OTHER_LEFT, OTHER_LEFT_length, ARG, ARG_length, OTHER_RIGHT, OTHER_RIGHT_length);\
	for (i = depth; i > 0; i--) {\
		memcpy(*target, (*other)[i-1].right.start, (*other)[i-1].right.length); *target += (*other)[i-1].right.length;\
	}\
	memcpy(*target, "\n", 1); *target += 1;\
};



#define ACTION_END_LINE(state) {\
\
	if ((state).tokens.name.end && (state).tokens.expression.end) {\
\
		if ((state).action == ACTION_PARAM) {\
\
			if ((state).tokens.name.end - (state).tokens.name.start + 1 > *name_buffer_size) {\
				*name_buffer_size = (state).tokens.name.end - (state).tokens.name.start + 1;\
				*name_buffer = realloc(*name_buffer, sizeof(char) * (*name_buffer_size));\
			}\
			memcpy(*name_buffer, (state).tokens.name.start, (state).tokens.name.end - (state).tokens.name.start);\
			(*name_buffer)[(state).tokens.name.end - (state).tokens.name.start] = 0;\
\
			param_values = PyDict_GetItemString(params, *name_buffer);\
			if (param_values) {\
				if ((state).flags.strict || PyList_Check(param_values)) {\
					list_size = PyList_Size(param_values);\
					for (j = 0; j < list_size; j++) {\
						item = PyList_GetItem(param_values, j);\
						if (!PyUnicode_Check(item)) {\
							item = PyObject_Str(item);\
						}\
						value = PyUnicode_AsUTF8AndSize(item, &value_size);\
						render__param(\
							output_end,\
							(state).tokens.line.start, (state).tokens.expression.start - (state).tokens.line.start,\
							value, value_size,\
							(state).tokens.expression.end, (state).tokens.line.end - (state).tokens.expression.end\
						);\
					}\
				} else {\
					if (!PyUnicode_Check(param_values)) {\
						item = PyObject_Str(param_values);\
					} else {\
						item = param_values;\
					}\
					value = PyUnicode_AsUTF8AndSize(item, &value_size);\
					render__param(\
						output_end,\
						(state).tokens.line.start, (state).tokens.expression.start - (state).tokens.line.start,\
						value, value_size,\
						(state).tokens.expression.end, (state).tokens.line.end - (state).tokens.expression.end\
					);\
				}\
			} else if (!(state).flags.optional) {\
				render__param(\
					output_end,\
					(state).tokens.line.start, (state).tokens.expression.start - (state).tokens.line.start,\
					"", 0,\
					(state).tokens.expression.end, (state).tokens.line.end - (state).tokens.expression.end\
				);\
			}\
\
		}\
		else if ((state).action == ACTION_REF) {\
\
			if (depth >= *other_size) {\
				*other_size = depth * 2;\
				*other = realloc(*other, sizeof(Other) * (*other_size));\
			}\
			(*other)[depth].left.start = (state).tokens.line.start;\
			(*other)[depth].left.length = (state).tokens.expression.start - (state).tokens.line.start;\
			(*other)[depth].right.start = (state).tokens.expression.end;\
			(*other)[depth].right.length = (state).tokens.line.end - (state).tokens.expression.end;\
\
			if ((state).tokens.name.end - (state).tokens.name.start + 1 > *name_buffer_size) {\
				*name_buffer_size = (state).tokens.name.end - (state).tokens.name.start + 1;\
				*name_buffer = realloc(*name_buffer, sizeof(char) * (*name_buffer_size));\
			}\
			memcpy(*name_buffer, (state).tokens.name.start, (state).tokens.name.end - (state).tokens.name.start);\
			(*name_buffer)[(state).tokens.name.end - (state).tokens.name.start] = 0;\
\
			ref_values = PyDict_GetItemString(params, *name_buffer);\
			if (ref_values) {\
				if ((state).flags.strict || PyList_Check(ref_values)) {\
					list_size = PyList_Size(ref_values);\
					for (j = 0; j < list_size; j++) {\
						render_(\
							render_result,\
							(state).tokens.name.start,\
							(state).tokens.name.end - (state).tokens.name.start,\
							output_end,\
							depth + 1,\
							buffer_size,\
							other,\
							other_size,\
							name_buffer,\
							name_buffer_size,\
							subarrays_length + (*other)[depth].left.length + (*other)[depth].right.length,\
							PyList_GetItem(ref_values, j)\
						);\
					}\
				} else {\
					render_(\
						render_result,\
						(state).tokens.name.start,\
						(state).tokens.name.end - (state).tokens.name.start,\
						output_end,\
						depth + 1,\
						buffer_size,\
						other,\
						other_size,\
						name_buffer,\
						name_buffer_size,\
						subarrays_length + (*other)[depth].left.length + (*other)[depth].right.length,\
						ref_values\
					);\
				}\
			} else if (!(state).flags.optional) {\
				render_(\
					render_result,\
					(state).tokens.name.start,\
					(state).tokens.name.end - (state).tokens.name.start,\
					output_end,\
					depth + 1,\
					buffer_size,\
					other,\
					other_size,\
					name_buffer,\
					name_buffer_size,\
					subarrays_length + (*other)[depth].left.length + (*other)[depth].right.length,\
					empty_dict\
				);\
			}\
\
		}\
\
	}\
\
	if ((state).action == ACTION_NONE) {\
		render__empty(output_end, (state).tokens.line.start, (state).tokens.line.end - (state).tokens.line.start);\
	}\
}


PyObject *empty_dict;


void render_(
	RenderResult *render_result,
	char *template_name,
	size_t template_name_length,
	char **output_end,
	size_t depth,
	size_t *buffer_size,
	Other **other,
	size_t *other_size,
	char **name_buffer,
	size_t *name_buffer_size,
	size_t subarrays_length,
	PyObject *params
)
{

	Template *template = dictionaryLookupUnterminated(templates, template_name, template_name_length);
	if (template == NULL) {
		render_result->message = malloc(sizeof(char) * (template_name_length + 1));
		memcpy_s(render_result->message, template_name_length, template_name, template_name_length);
		render_result->message[template_name_length] = 0;
		return;
	}

	if (!depth) {
		buffer_size = &template->buffer_size;
		render_result->result = malloc(sizeof(char) * (*buffer_size));
		*output_end = render_result->result;
	}

	char *p = template->text;
	char *pe = template->text + template->length;
	char *eof = pe;
	size_t cs;
	char *new_result;
	char *value;
	Py_ssize_t value_size;
	PyObject *param_values;
	PyObject *ref_values;
	PyObject *item;

	size_t i;
	RenderState *state = NULL;
	size_t i_template = 0;
	Py_ssize_t j;
	Py_ssize_t list_size;

	if (template->render_states.length == 0) {

		state = malloc(sizeof(RenderState) * 1);
		resetRenderState(*state);

		
/* #line 273 "compileComprehension.c" */
	{
	cs = render_start;
	}

/* #line 278 "compileComprehension.c" */
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr1:
/* #line 264 "compileComprehension_preprocessed.rl" */
	{ state->tokens.line.start = p; }
/* #line 265 "compileComprehension_preprocessed.rl" */
	{

				state->tokens.line.end = p;
				listPush(template->render_states, state);

				ACTION_END_LINE(*state);

				state = malloc(sizeof(RenderState) * 1);
				resetRenderState(*state)

			}
	goto st0;
tr4:
/* #line 265 "compileComprehension_preprocessed.rl" */
	{

				state->tokens.line.end = p;
				listPush(template->render_states, state);

				ACTION_END_LINE(*state);

				state = malloc(sizeof(RenderState) * 1);
				resetRenderState(*state)

			}
	goto st0;
tr32:
/* #line 289 "compileComprehension_preprocessed.rl" */
	{ state->tokens.expression.end = p; }
/* #line 265 "compileComprehension_preprocessed.rl" */
	{

				state->tokens.line.end = p;
				listPush(template->render_states, state);

				ACTION_END_LINE(*state);

				state = malloc(sizeof(RenderState) * 1);
				resetRenderState(*state)

			}
	goto st0;
st0:
	if ( ++p == pe )
		goto _test_eof0;
case 0:
/* #line 334 "compileComprehension.c" */
	switch( (*p) ) {
		case 10: goto tr1;
		case 60: goto tr2;
	}
	goto tr0;
tr0:
/* #line 264 "compileComprehension_preprocessed.rl" */
	{ state->tokens.line.start = p; }
	goto st1;
tr31:
/* #line 289 "compileComprehension_preprocessed.rl" */
	{ state->tokens.expression.end = p; }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
/* #line 352 "compileComprehension.c" */
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
	}
	goto st1;
tr2:
/* #line 264 "compileComprehension_preprocessed.rl" */
	{ state->tokens.line.start = p; }
/* #line 285 "compileComprehension_preprocessed.rl" */
	{
				if (!(state->tokens.expression.start && state->tokens.name.end))
					state->tokens.expression.start = p;
			}
	goto st2;
tr5:
/* #line 285 "compileComprehension_preprocessed.rl" */
	{
				if (!(state->tokens.expression.start && state->tokens.name.end))
					state->tokens.expression.start = p;
			}
	goto st2;
tr33:
/* #line 285 "compileComprehension_preprocessed.rl" */
	{
				if (!(state->tokens.expression.start && state->tokens.name.end))
					state->tokens.expression.start = p;
			}
/* #line 289 "compileComprehension_preprocessed.rl" */
	{ state->tokens.expression.end = p; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
/* #line 387 "compileComprehension.c" */
	switch( (*p) ) {
		case 10: goto tr4;
		case 33: goto st3;
		case 60: goto tr5;
	}
	goto st1;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 10: goto tr4;
		case 45: goto st4;
		case 60: goto tr5;
	}
	goto st1;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 10: goto tr4;
		case 45: goto st5;
		case 60: goto tr5;
	}
	goto st1;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	switch( (*p) ) {
		case 10: goto tr4;
		case 32: goto st5;
		case 40: goto st6;
		case 60: goto tr5;
	}
	goto st1;
tr22:
/* #line 279 "compileComprehension_preprocessed.rl" */
	{ state->flags.optional = true; }
	goto st6;
tr39:
/* #line 277 "compileComprehension_preprocessed.rl" */
	{ state->action = ACTION_PARAM; }
	goto st6;
tr44:
/* #line 278 "compileComprehension_preprocessed.rl" */
	{ state->action = ACTION_REF; }
	goto st6;
tr52:
/* #line 280 "compileComprehension_preprocessed.rl" */
	{ state->flags.strict = true; }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
/* #line 445 "compileComprehension.c" */
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 111: goto st7;
		case 112: goto st21;
		case 114: goto st27;
		case 115: goto st31;
	}
	goto st1;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 112: goto st8;
	}
	goto st1;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 116: goto st9;
	}
	goto st1;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 105: goto st10;
	}
	goto st1;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 111: goto st11;
	}
	goto st1;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 110: goto st12;
	}
	goto st1;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 97: goto st13;
	}
	goto st1;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 108: goto st14;
	}
	goto st1;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 10: goto tr4;
		case 41: goto st15;
		case 60: goto tr5;
	}
	goto st1;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 10: goto tr4;
		case 40: goto tr22;
		case 60: goto tr5;
		case 95: goto tr23;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr23;
	} else if ( (*p) >= 65 )
		goto tr23;
	goto st1;
tr23:
/* #line 279 "compileComprehension_preprocessed.rl" */
	{ state->flags.optional = true; }
/* #line 282 "compileComprehension_preprocessed.rl" */
	{ state->tokens.name.start = p; }
	goto st16;
tr40:
/* #line 277 "compileComprehension_preprocessed.rl" */
	{ state->action = ACTION_PARAM; }
/* #line 282 "compileComprehension_preprocessed.rl" */
	{ state->tokens.name.start = p; }
	goto st16;
tr45:
/* #line 278 "compileComprehension_preprocessed.rl" */
	{ state->action = ACTION_REF; }
/* #line 282 "compileComprehension_preprocessed.rl" */
	{ state->tokens.name.start = p; }
	goto st16;
tr53:
/* #line 280 "compileComprehension_preprocessed.rl" */
	{ state->flags.strict = true; }
/* #line 282 "compileComprehension_preprocessed.rl" */
	{ state->tokens.name.start = p; }
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
/* #line 579 "compileComprehension.c" */
	switch( (*p) ) {
		case 10: goto tr4;
		case 32: goto tr24;
		case 45: goto tr25;
		case 60: goto tr5;
		case 95: goto st16;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st16;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st16;
	} else
		goto st16;
	goto st1;
tr24:
/* #line 283 "compileComprehension_preprocessed.rl" */
	{ state->tokens.name.end = p; }
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
/* #line 604 "compileComprehension.c" */
	switch( (*p) ) {
		case 10: goto tr4;
		case 32: goto st17;
		case 45: goto st18;
		case 60: goto tr5;
	}
	goto st1;
tr25:
/* #line 283 "compileComprehension_preprocessed.rl" */
	{ state->tokens.name.end = p; }
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
/* #line 620 "compileComprehension.c" */
	switch( (*p) ) {
		case 10: goto tr4;
		case 45: goto st19;
		case 60: goto tr5;
	}
	goto st1;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 62: goto st20;
	}
	goto st1;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	switch( (*p) ) {
		case 10: goto tr32;
		case 60: goto tr33;
	}
	goto tr31;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 97: goto st22;
	}
	goto st1;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 114: goto st23;
	}
	goto st1;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 97: goto st24;
	}
	goto st1;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 109: goto st25;
	}
	goto st1;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 10: goto tr4;
		case 41: goto st26;
		case 60: goto tr5;
	}
	goto st1;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 10: goto tr4;
		case 40: goto tr39;
		case 60: goto tr5;
		case 95: goto tr40;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr40;
	} else if ( (*p) >= 65 )
		goto tr40;
	goto st1;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 101: goto st28;
	}
	goto st1;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 102: goto st29;
	}
	goto st1;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 10: goto tr4;
		case 41: goto st30;
		case 60: goto tr5;
	}
	goto st1;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 10: goto tr4;
		case 40: goto tr44;
		case 60: goto tr5;
		case 95: goto tr45;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr45;
	} else if ( (*p) >= 65 )
		goto tr45;
	goto st1;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 116: goto st32;
	}
	goto st1;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 114: goto st33;
	}
	goto st1;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 105: goto st34;
	}
	goto st1;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 99: goto st35;
	}
	goto st1;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	switch( (*p) ) {
		case 10: goto tr4;
		case 60: goto tr5;
		case 116: goto st36;
	}
	goto st1;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
	switch( (*p) ) {
		case 10: goto tr4;
		case 41: goto st37;
		case 60: goto tr5;
	}
	goto st1;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
	switch( (*p) ) {
		case 10: goto tr4;
		case 40: goto tr52;
		case 60: goto tr5;
		case 95: goto tr53;
	}
	if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr53;
	} else if ( (*p) >= 65 )
		goto tr53;
	goto st1;
	}
	_test_eof0: cs = 0; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 1: 
	case 2: 
	case 3: 
	case 4: 
	case 5: 
	case 6: 
	case 7: 
	case 8: 
	case 9: 
	case 10: 
	case 11: 
	case 12: 
	case 13: 
	case 14: 
	case 15: 
	case 16: 
	case 17: 
	case 18: 
	case 19: 
	case 21: 
	case 22: 
	case 23: 
	case 24: 
	case 25: 
	case 26: 
	case 27: 
	case 28: 
	case 29: 
	case 30: 
	case 31: 
	case 32: 
	case 33: 
	case 34: 
	case 35: 
	case 36: 
	case 37: 
/* #line 265 "compileComprehension_preprocessed.rl" */
	{

				state->tokens.line.end = p;
				listPush(template->render_states, state);

				ACTION_END_LINE(*state);

				state = malloc(sizeof(RenderState) * 1);
				resetRenderState(*state)

			}
	break;
	case 20: 
/* #line 289 "compileComprehension_preprocessed.rl" */
	{ state->tokens.expression.end = p; }
/* #line 265 "compileComprehension_preprocessed.rl" */
	{

				state->tokens.line.end = p;
				listPush(template->render_states, state);

				ACTION_END_LINE(*state);

				state = malloc(sizeof(RenderState) * 1);
				resetRenderState(*state)

			}
	break;
/* #line 943 "compileComprehension.c" */
	}
	}

	}

/* #line 314 "compileComprehension_preprocessed.rl" */


		free(state);

	} else {

		for (i_template = 0; i_template < template->render_states.length; i_template++) {
			state = template->render_states.start[i_template];
			ACTION_END_LINE(*state);
		}

	}

	if (!depth) {
		**output_end = 0;
	}

};


PyObject *render (
	PyObject *self,
	PyObject *args
) {

	char *name;
	PyObject *params;

	if (!PyArg_ParseTuple(args, "sO!", &name, &PyDict_Type, &params))
		return NULL;

	RenderResult render_result;
	render_result.message = NULL;
	render_result.result = NULL;

	size_t other_size = 16;
	Other *other = malloc(sizeof(Other) * other_size);

	size_t name_buffer_size = 128;
	char *name_buffer = malloc(sizeof(char) * name_buffer_size);

	char *output_end = NULL;

	render_(
		&render_result,
		name,
		strlen(name),
		&output_end,
		0,
		NULL,
		&other,
		&other_size,
		&name_buffer,
		&name_buffer_size,
		0,
		params
	);

	free(other);
	free(name_buffer);

	if (render_result.message) {
		free(render_result.result);
		PyErr_SetString(PyExc_KeyError, render_result.message);
		return NULL;
	}

	return PyUnicode_FromStringAndSize(render_result.result, output_end-render_result.result);

}