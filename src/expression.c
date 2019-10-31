#include "lex.h"
#include "parser.h"
#include <pthread.h>


/* 先搞表达式 */
P( primary_expression )
{
	CHECK_FIRST(primary_expression);
    switch(look(s).tag) {
    case Id:
    case ConstChar:
    case ConstInt:
    case ConstFloat:
    case ConstLong:
    case ConstDouble:
    case ConstStr:
        next(s);
        break;
    case '(':
        next(s);
        NODE( expression );
        X(')');
        break;
    }
}


P(assignment_operator)
{
	CHECK_FIRST(assignment_operator);
	Node * ao = newNode(ASSIGNMENT_OPERATOR);
	addChild(ao, newAttrNode(next(s)));
	return ao;
}

/*
 * assignment_expression    ->  conditional_expression
 *                          |   unary_expression assignment_operaNODEr assignment_expression
 */
P(assignment_expression)
{
	CHECK_FIRST(assignment_expression);
	Node * ae = newNode(ASSIGNMENT_EXPRESSION);
	addChild(ae, NODE(conditional_expression));
	if (IN_FIRST(assignment_operator)) {
		addChild(ae, NODE(assignment_operator));
		Node * as = NODE(assignment_operator);
		if (!as) {
			expected(Assignment_expression);
			return ae;
		}
		addChild(ae, as);
	}
	return ae;
}


P_(argument_expression_list)
{
    if (look(s).tag == ',') {
        Node * expr = newNode(ARGUMENT_EXPRESSION_LIST);
        addChild(expr, first);
        addChild(expr, newNode(','));
        addChild(expr, NODE(assignment_expression));
        return expression_(expr, s);
    }else {
        return first;
    }
}

P(argument_expression_list)
{
}


P_(postfix_expression)
{
	if (eos(s)) return first;
    
	Node * post_expr = newNode(POSTFIX_EXPRESSION);
    addChild(post_expr, first);

    switch(look(s).tag) {
    case '[':
        addChild(post_expr, newNode(next(s).tag));
        addChild(post_expr, NODE(expression));
        X(']');
        addChild(post_expr, newNode(next(s).tag));
        return postfix_expression_(post_expr, s);
    case '(':
        next(s);
        addChild(post_expr, newNode('('));
        if (look(s).tag != ')') {
            addChild(post_expr, NODE(argument_expression_list));
        }
        X(')');
        addChild(post_expr, newNode(')'));
        return postfix_expression_(post_expr, s);
    case '.':
    case Pto:
        addChild(post_expr, newNode(look(s).tag));
        next(s);
        if (look(s).tag != Id) {
            printf("error\n");
        }
        X(Id);
        break;
    case SPlus:
    case SSub:
        next(s);
    }
	return post_expr;
}
P( postfix_expression) 
{
	CHECK_FIRST(postfix_expression);
	if (eos(s)) return NULL;
	Node * pe = newNode(POSTFIX_EXPRESSION);
	addChild(pe , NODE(primary_expression));
    return postfix_expression_(pe, s);
}




P(logical_AND_expression)
{

}



P_(logical_OR_expression)
{
	if (eos(s) || look(s).tag != Or) return first;
	Node * lor = newNode(LOGICAL_OR_EXPRESSION);
	addChild(lor, first);
	addChild(lor, newNode(next(s).tag));
	if (!IN_FIRST(logical_AND_expression)) {
		printf("error want exp\n");
	}
	addChild(lor, NODE(logical_AND_expression));
	return logical_OR_expression_(lor,s);
}

P(logical_OR_expression)
{
	CHECK_FIRST(logical_OR_expression);
	Node * lor = newNode(LOGICAL_OR_EXPRESSION);
	addChild(lor, NODE(logical_AND_expression));
	return logical_OR_expression_(lor, s);
}

P(conditional_expression)
{
	CHECK_FIRST(conditional_expression);
	Node * cexp = newNode(CONDITIONAL_EXPRESSION);
	addChild(cexp, NODE(logical_OR_expression));
	if (eos(s) || look(s).tag != '?') return cexp;

	addChild(cexp, newNode(next(s).tag));
	addChild(cexp, NODE(expression));
	X(':');
	addChild(cexp, newNode(next(s).tag));
	addChild(cexp, NODE(constant_expression));
	return cexp;
}

P_(expression) 
{
	if (eos(s) || look(s).tag != ',') return first;
	Node * expr = newNode(EXPRESSION);
	addChild(expr, first);
	addChild(expr, newNode(next(s).tag));
	addChild(expr, NODE(assignment_expression));
	return expression_(expr, s);
}

P(expression)
{
	CHECK_FIRST(expression);
	Node * exp = newNode(EXPRESSION);
	addChild(exp, NODE(assignment_expression));
	return expression_(exp, s);
}

P(constant_expression)
{
	CHECK_FIRST(constant_expression);
	return NODE(conditional_expression);
}
