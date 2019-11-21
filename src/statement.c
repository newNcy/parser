#include "lex.h"
#include "parser.h"


P(labeled_statement)
{
	//不用检查了 刚检查过 下同
	Node * ret = newAttrNode(next(s));
	switch(ret->op) {
		case Case:
			addChild(ret, NODE(constant_expression));
		case Default:
			X(':');
			addChild(ret, NODE(statement));
			break;
	}
	return ret;
}

P(selection_statement)
{
	Node * ret = newAttrNode(next(s));
	switch (ret->op) {
		case If:
			X('(');
			addChild(ret, NODE(expression));
			X(')');
			addChild(ret, NODE(statement));
			if (look(s).tag == Else) {
				addChild(ret, newAttrNode(next(s)));
				addChild(ret, NODE(statement));
			}
			break;
		case Switch:
			X('(');
			addChild(ret, NODE(expression));
			X(')');
			addChild(ret, NODE(statement));
			break;
			
	}
	return ret;
}

P(iteration_statement)
{
	Node * ret = newNode(ITERATION_STATEMENT);
	if (look(s).tag == While) {
		ret->op = next(s).tag;
		X('(');
		addChild(ret, NODE(expression));
		X(')');
		addChild(ret, NODE(statement));
	}else if (look(s).tag == For) {
		ret->op = next(s).tag;
		X('(');
		for (int i = 0 ; i < 3; i++) {
			if (IN_FIRST(declaration)) {
				addChild(ret, NODE(declaration));
			}else if (IN_FIRST(expression)) {
				addChild(ret, NODE(expression));
			}
			if (i < 2) {
				X(';');
			}
		}
		
		X(')');
		addChild(ret, NODE(statement));
	}

	return ret;
}


P(jump_statement)
{
	switch (look(s).tag) {
		case Continue:
		case Break:
			return newAttrNode(next(s));
		case Return:
			{
				Node * ret = newAttrNode(next(s));
				if (IN_FIRST(expression)) {
					addChild(ret, NODE(expression));
				}
				X(';');
				return ret;
			}

	}
}

P(expression_statement)
{
	Node * expr = NULL; 
	if (IN_FIRST(expression)) {
		expr = NODE(expression);
	}
	X(';');
	return expr;
}

P(compound_statement)
{
	CHECK_FIRST(compound_statement);
	next(s);
	Node * bil =  NULL;
	if (IN_FIRST(block_item_list)) {
		bil = NODE(block_item_list);
	}
	X('}');
	return bil;

}
P(statement)
{
	CHECK_FIRST(statement);
	if (IN_FIRST(labeled_statement)) {
		return NODE(labeled_statement);
	}else if (IN_FIRST(compound_statement)) {
		return NODE(compound_statement);
	}else if (IN_FIRST(selection_statement)) {
		return NODE(selection_statement);
	}else if (IN_FIRST(iteration_statement)) {
		return NODE(iteration_statement);
	}else if (IN_FIRST(jump_statement)) {
		return NODE(jump_statement);
	}else {
		return NODE(expression_statement);
	}
}
