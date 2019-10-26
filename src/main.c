#include <stdio.h>
#include "lex.h"

/* 语法分析 构建抽象语法树 */

enum Op
{	
	PRIMARY_EXPRESSION = END,
	POSTFIX_EXPRESSION,
	ARGUMENT_EXPRESSION_LIST,
	EXPRESSION
};
typedef enum Op Op;
struct Node
{
	Op op;
	long attr;
	struct Node * child;
	struct Node * sbling;
};

typedef struct Node Node;
Node * newNode(Op op)
{
	Node * node = (Node*)new(sizeof(Node));
	node->op = op;
	node->attr = -1;
	node->child = NULL;
	node->sbling = NULL;
	return node;
}
void addChild(Node * p, Node * c)
{
	if (p->child == NULL) {
		p->child = p;
	}
	Node * n = p->child;
	while (n->sbling) {
		n = n->sbling;
	}
	n->sbling = c; 
}

void match(Source * s,Tag t)
{
	if (look(s).tag != t) printf("需要%c\n",t);
	next(s);
}

#define P(X) Node * X		(Source * s)
#define P_(X) Node * X##_	(Node * first, Source * s)
#define TO(X) X(s)
#define X(C)  match(s,C);
/*
 * 一下产生式来自c语言文档n1548
 */

/* 表达式 */

P( primary_expression			);
P( postfix_expression			);
P( argument_expression_list		);
P( argument_expression_list		);
P( unary_expression				);
P( unary_operator				);
P( cast_expression				);
P( multiplicative_expression	);
P( additive_expression			);
P( shift_expression				);
P( relational_expression		);
P( equality_expression			);
P( AND_expression				);
P( exclusive_OR_expression		);
P( inclusive_OR_expression		);
P( logical_AND_expression		);
P( logical_OR_expression		);
P( conditional_expression		);
P( assignment_expression		);
P( assignment_operator			);
P( expression					);
P( constant_expression			);

/* 声明 */
P( declareation					);
P( declaration_specifiers		);
P( init_declarator_list			);
P( init_declarator				);
P( type_specifier				);
P( struct_specifier				);
P( struct_declaration_list		);
P( struct_declaration			);
P( specifier_qualifier_list		);
P( struct_declarator_list		);
P( struct_declarator			);
P( enum_specifier				);
P( enumerator_list				);
P( enumerator					);
P( enumerator					);
P( type_qualifier				);
P( function_specifier			);
P( declarator					);
P( direct_declarator			);
P( pointer						);
P( type_qualifier_list			);
P( parameter_type_list			);
P( parameter_list				);
P( parameter_declaration		);
P( identifier_list				);
P( type_name					);
P( initializer					);
P( initializer_list				);
P( designation					);
P( designator_list				);
P( designator					);

/* 语句 */
P( statement					);
P( labeled_statement			);
P( compound_statement			);
P( compound_statement			);
P( block_item_list				);
P( block_item					);
P( expression_statement			);
P( selection_statement			);
P( iteration_statement			);
P( jump_statement				);

/* 定义 */

P( translation_unit				);
P( external_declaration			);
P( function_definition			);
P( declaration_list				);



/* 辅助过程 xx_*/
P_(expression);

#define FIRST(X) int firstOf##X(Source * s)
#define FIRSTOF(X) firstOf##X(s)
FIRST(primary_expression)
{
	switch(look(s).tag) {
		case Id:
		case ConstChar:
		case ConstInt:
		case ConstFloat:
		case ConstLong:
		case ConstDouble:
		case ConstStr:
		case '(':
			return 1;
		default:
			return 0;
	}

}

FIRST(unary_expression)
{
	switch(look(s).tag) {
		/* first set of unary expr*/
		case SPlus:
		case SSub:
		case '&':
		case '*':
		case '+':
		case '-':
		case '~':
		case '!':
		case Sizeof:
			return 1;
		/* first set of primary expr*/
	}
	return FIRSTOF(postfix_expression);

}

#undef FIRST

#define FIRST(X) firstOf##X(look(s).tag)


/* 先搞表达式 */
P( primary_expression )
{
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
			TO( expression );
			X(')');
			break;
		default:
			printf("错误\n");
			return NULL;
	}

}


P(assignment_expression)
{
	switch(look(s).tag) {
		/* first set of unary expr*/
		case SPlus:
		case SSub:
		case '&':
		case '*':
		case '+':
		case '-':
		case '~':
		case '!':
		case Sizeof:
		
		/* first set of primary expr*/
	}
}


P_(argument_expression_list)
{
	if (look(s).tag == ',') {
		Node * expr = newNode(ARGUMENT_EXPRESSION_LIST);
		addChild(expr, first);
		addChild(expr, newNode(','));
		addChild(expr, TO(assignment_expression));
		return expression_(expr, s);
	}else {
		return first;
	}
}

P(argument_expression_list)
{
	return argument_expression_list_(assignment_expression(s), s);
}


Node* postfix_expression_(Node * post_expr_, Source * s)
{
	Node * post_expr = newNode(POSTFIX_EXPRESSION);
	addChild(post_expr, post_expr_);

	switch(look(s).tag) {
		case '[':
			next(s);
			addChild(post_expr, newNode('['));
			addChild(post_expr, TO(expression));
			X(']');
			addChild(post_expr, newNode(']'));
			return postfix_expression_(post_expr, s);
		case '(':
			next(s);
			addChild(post_expr, newNode('('));
			if (look(s).tag != ')') {
				addChild(post_expr, TO(argument_expression_list));
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
		default:
			return post_expr_;
	}
}
P( postfix_expression) 
{
	return postfix_expression_(TO(primary_expression), s);
}


P_(expression) 
{
	if (look(s).tag == ',') {
		Node * expr = newNode(EXPRESSION);
		addChild(expr, first);
		addChild(expr, newNode(','));
		addChild(expr, TO(assignment_expression));
		return expression_(expr, s);
	}else {
		return first;
	}
}

P(expression)
{
	return expression_(TO(assignment_expression), s);
}

int main(int argc, char *argv[])
{
	Source * test = sourceFromFile("test.c");
	if (!test) return -1;

	while (!eos(test)) {
		expression(test);
		match(test,';');
	}
	return 0;
}
