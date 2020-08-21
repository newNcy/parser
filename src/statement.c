#include "lex.h"
#include "parser.h"


P(labeled_statement)
{
	//不用检查了 刚检查过 下同
	Node * ret = newNode(LABELED_STATEMENT);
	switch(look(s, env).tag) {
		case Case:
			addChild(ret, newAttrNode(next(s, env)));
			addChild(ret, NODE(constant_expression));
			X(':');
			addChild(ret, newAttrNode(next(s, env)));
			addChild(ret, NODE(statement));
			break;
		case Default:
			addChild(ret, newAttrNode(next(s, env)));
			X(':');
			addChild(ret, newAttrNode(next(s, env)));
			addChild(ret, NODE(statement));
			break;
	}
	return ret;
}

P(selection_statement)
{
	Node * ret = newNode(SELECTION_STATEMENT);
	switch (look(s, env).tag) {
		case If:
			addChild(ret, newAttrNode(next(s, env)));
			X('(');
			addChild(ret, newAttrNode(next(s, env)));
			addChild(ret, NODE(expression));
			X(')');
			addChild(ret, newAttrNode(next(s, env)));
			addChild(ret, NODE(statement));
			if (look(s, env).tag == Else) {
				addChild(ret, newAttrNode(next(s, env)));
				addChild(ret, NODE(statement));
			}
			break;
		case Switch:
			addChild(ret, newAttrNode(next(s, env)));
			X('(');
			addChild(ret, newAttrNode(next(s, env)));
			addChild(ret, NODE(expression));
			X(')');
			addChild(ret, newAttrNode(next(s, env)));
			addChild(ret, NODE(statement));
			break;
			
	}
	return ret;
}

P(iteration_statement)
{
	Node * ret = newNode(ITERATION_STATEMENT);
	if (look(s, env).tag == While) {
		addChild(ret, newAttrNode(next(s, env)));
		X('(');
		addChild(ret, newAttrNode(next(s, env)));
		addChild(ret, NODE(expression));
		X(')');
		addChild(ret, newAttrNode(next(s, env)));
		addChild(ret, NODE(statement));
	}else if (look(s, env).tag == For) {
		addChild(ret, newAttrNode(next(s, env)));
		X('(');
		addChild(ret, newAttrNode(next(s, env)));
		for (int i = 0 ; i < 3; i++) {
			if (IN_FIRST(declaration)) {
				addChild(ret, NODE(declaration));
			}else if (IN_FIRST(expression)) {
				addChild(ret, NODE(expression));
			}
			if (i < 2) {
				X(';');
				addChild(ret, newAttrNode(next(s, env)));
			}
		}
		
		X(')');
		addChild(ret, newAttrNode(next(s, env)));
		addChild(ret, NODE(statement));
	}

	return ret;
}


P(jump_statement)
{
	Node * ret = newNode(JUMP_STATEMENT);
	switch (look(s, env).tag) {
		case Continue:
		case Break:
			addChild(ret, newAttrNode(next(s, env)));
			break;
		case Return:
			addChild(ret, newAttrNode(next(s, env)));
			if (IN_FIRST(expression)) {
				addChild(ret, NODE(expression));
			}
			X(';');
			addChild(ret, newAttrNode(next(s, env)));

	}
	return ret;
}

P(expression_statement)
{
	Node * ret = newNode(EXPRESSION_STATEMENT);
	if (IN_FIRST(expression)) {
		addChild(ret, NODE(expression));
	}
	X(';');
	addChild(ret, newAttrNode(next(s, env)));
	return ret;
}


P(statement)
{
	CHECK_FIRST(statement);
	Node * ret = newNode(STATEMENT);
	if (IN_FIRST(labeled_statement)) {
		addChild(ret, NODE(labeled_statement));
	}else if (IN_FIRST(compound_statement)) {
		addChild(ret, NODE(compound_statement));
	}else if (IN_FIRST(selection_statement)) {
		addChild(ret, NODE(selection_statement));
	}else if (IN_FIRST(iteration_statement)) {
		addChild(ret, NODE(iteration_statement));
	}else if (IN_FIRST(jump_statement)) {
		addChild(ret, NODE(jump_statement));
	}else {
		addChild(ret, NODE(expression_statement));
	}
	return ret;
}
