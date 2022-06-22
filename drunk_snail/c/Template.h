#pragma once

#include <stdbool.h>
#include <sys/types.h>

#include "List.h"



typedef struct {
	char *start;
	char *end;
} Token;
typedef struct {
	Token line;
	Token expression;
	Token name;
} Tokens;

typedef bool Flag;
typedef struct {
	Flag optional;
	Flag strict;
} Flags;

enum ActionType {
	ACTION_PARAM,
	ACTION_REF,
	ACTION_NONE
};

typedef struct {
	Tokens tokens;
	enum ActionType action;
	Flags flags;
} RenderState;

typedef struct {

	char *text;
	size_t length;
	size_t buffer_size;

	List render_states;

} Template;


#define resetToken(token) {\
	token.start = NULL;\
	token.end = NULL;\
}

#define resetFlag(flag) {\
	flag = false;\
}

#define resetRenderState(state) {\
	resetToken((state).tokens.line);\
	resetToken((state).tokens.expression);\
	resetToken((state).tokens.name);\
	(state).action = ACTION_NONE;\
	resetFlag((state).flags.optional);\
	resetFlag((state).flags.strict);\
}