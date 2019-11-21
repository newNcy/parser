#include "lex.h"
#include "parser.h"


#define P(X)  Node * X(Env * env, Source *s)
#define NODE(X) X(env, s)


P(constant_expression);
P(conditional_expression);
P(expression);
/* 先搞表达式 */
P( primary_expression )
{
	CHECK_FIRST(primary_expression);
	Node * ret = NULL;
    switch(look(s).tag) {
    case Id:
    case ConstChar:
    case ConstInt:
    case ConstFloat:
    case ConstLong:
    case ConstDouble:
    case ConstStr:
		ret = newAttrNode(next(s));
        break;
    case '(':
		next(s);
		ret = expression(env, s);	
        X(')');
        break;
    }
	return ret;
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
		addChild(ae, NODE(assignment_expression));
	}
	return ae;
}


LEFT_RECURSIVE(argument_expression_list, assignment_expression, ARGUMENT_EXPRESSION_LIST, ',');





P( postfix_expression) 
{
	CHECK_FIRST(postfix_expression);
	Node * pe = newNode(POSTFIX_EXPRESSION);
	addChild(pe , NODE(primary_expression));

	int set[] = {'[', '(', '.', Pto, SPlus, SSub};
	while (INSET(set)) {
		Node * t = newNode(POSTFIX_EXPRESSION);
		addChild(t, pe);
		switch (look(s).tag) {
			case '[':
				addChild(t, newAttrNode(next(s)));
				addChild(t, NODE(expression));
				X(']');
				addChild(t, newAttrNode(next(s)));
				break;
			case '(':
				addChild(t, newAttrNode(next(s)));
				if (IN_FIRST(argument_expression_list)) {
					addChild(t, NODE(argument_expression_list));
				}
				X(')');
				addChild(t, newAttrNode(next(s)));
				break;
			case '.':
			case Pto:
				addChild(t, newAttrNode(next(s)));
				if (look(s).tag != Id) {
					expected(Id);
				}
				addChild(t, newAttrNode(next(s)));
				break;
			case SPlus:
			case SSub:
				addChild(t, newAttrNode(next(s)));
				break;
		}
		pe = t;
	}
	return pe;
}

P(unary_expression)
{
	CHECK_FIRST(unary_expression);
	Node * ret = newNode(UNARY_EXPRESSION);
	if (IN_FIRST(postfix_expression)) {
		addChild(ret, NODE(postfix_expression));
	}else if (IN_FIRST(unary_operator)) {
		Node * uo = newNode(UNARY_OPERATOR);
		addChild(uo, newAttrNode(next(s)));
		addChild(ret, uo);
		addChild(ret, NODE(unary_expression));
	}else {
		int isSizeof = 0;
		switch(look(s).tag) {
			case Sizeof:
				isSizeof = 1;
			case SPlus:
			case SSub:
				addChild(ret, newAttrNode(next(s)));
				if (isSizeof) {
					if(look(s).tag == '(') {
						addChild(ret, newAttrNode(next(s)));
						addChild(ret, NODE(type_name));
						X(')');
						addChild(ret, newAttrNode(next(s)));
					}else {
						addChild(ret, NODE(unary_expression));
					}
				}
				break;
		}
	}
}

P(cast_expression)
{
	CHECK_FIRST(cast_expression);
	Node * ret = newNode(CAST_EXPRESSION);
	if (look(s).tag == '(') {
		addChild(ret, newAttrNode(next(s)));
		if (IN_FIRST(type_name)) {
			addChild(ret,NODE(type_name));
			X(')');
			addChild(ret, newAttrNode(next(s)));
			if (!eos(s)) addChild(ret, NODE(cast_expression));
		}else if (IN_FIRST(expression)) {
			//推导树最底部构建,算是变相规约
			//primary_expression
			Node * pri = newNode(PRIMARY_EXPRESSION);
			addChild(ret, newAttrNode(next(s)));
			addChild(pri, NODE(expression));
			X(')');
			addChild(ret, newAttrNode(next(s)));

			//postfix_expression
			Node * post = newNode(POSTFIX_EXPRESSION);
			addChild(post, pri);
			//unary_expression
			Node * unary = newNode(UNARY_EXPRESSION);
			addChild(unary, post);

			addChild(ret, unary);

		}
	}else {
		addChild(ret, NODE(unary_expression));
	}
	return ret;
}

LEFT_RECURSIVE(multiplicative_expression, cast_expression, MULTIPLICATIVE_EXPRESSION, '*', '/','%');
LEFT_RECURSIVE(additive_expression, multiplicative_expression, ADDITIVE_EXPRESSION, '+', '-');
LEFT_RECURSIVE(shift_expression, additive_expression, SHIFT_EXPRESSION, Lsft, Rsft);
LEFT_RECURSIVE(relational_expression, shift_expression, RELATIONAL_EXPRESSION, '<','>',Leq,Geq);
LEFT_RECURSIVE(equality_expression, relational_expression, EQUALITY_EXPRESSION, Eq, Neq);
LEFT_RECURSIVE(AND_expression,equality_expression, AND_EXPRESSION, '&');
LEFT_RECURSIVE(exclusive_OR_expression, AND_expression, EXCLUSIVE_OR_EXPRESSION, '^');
LEFT_RECURSIVE(inclusive_OR_expression, exclusive_OR_expression, INCLUSIVE_OR_EXPRESSION, '|');
LEFT_RECURSIVE(logical_AND_expression, inclusive_OR_expression, LOGICAL_AND_EXPRESSION, And);
LEFT_RECURSIVE(logical_OR_expression, logical_AND_expression, LOGICAL_OR_EXPRESSION, Or);

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

LEFT_RECURSIVE(expression, assignment_expression, EXPRESSION, ',');

P(constant_expression)
{
	CHECK_FIRST(constant_expression);
	return NODE(conditional_expression);
}

long evalExpr(Node * node)
{
	switch(node->op) {
	}
	return 0;
}
