#pragma once
#include "lex.h"
#include <stdarg.h>


/* 语法分析 构建抽象语法树 */

enum Op
{	
    UNKOWN = END,
	PRIMARY_EXPRESSION,
	POSTFIX_EXPRESSION,
	ARGUMENT_EXPRESSION_LIST,
	LOGICAL_OR_EXPRESSION,
	CONDITIONAL_EXPRESSION,
	EXPRESSION,
    DECLARATOR,
    DECLARATION,
    POINTER,
    STRUCT_SPECIFIER,
    ENUM_SPECIFIER,
    TYPE_SPECIFIER,
    TYPE_QUALIFIER_LIST,
    TYPE_QUALIFIER,
	BLOCK_ITEM_LIST,
	STRUCT_DECLARATION_LIST,
	COMPOUND_STATEMENT,
	STORAGE_CLASS_SPECIFIER,
	DESIGNATION,
	DIRECT_DECLARATOR,
	DESIGNATOR,
	DESIGNATOR_LIST,
	INITIALIZER,
	INITIALIZER_LIST,
	INIT_DECLARATOR_LIST,
	DECLARATION_LIST,
	INIT_DECLARATOR,
    DECLARATION_SPECIFIERS,
	EXTERNAL_DECLARATION,
	FUNCTION_DEFINITION,
    TRANSLATE_UNIT
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

Node * newNode(Op op);
Node * newAttrNode(Token t);
void addChild(Node * p, Node * c);
void match(Source * s,Tag t);

//收集的信息
struct Collection
{
	
};



#define FIRST(X) int first_##X(Source * s) //求某个符号的first集
#define IN_FIRST(X) (!eos(s) && first_##X(s) )
#define CHECK_FIRST(X) \
    while(!IN_FIRST(X) && !eos(s)) {\
        printf("error in "#X"\n"); \
		next(s); \
    } \
	if (eos(s)) return NULL;

#define P(X) \
Node * X(Source * s);\
FIRST(X)

#define P_(X) Node * X##_	(Node * first, Source * s)
#define NODE(X) X(s)
#define X(C)  match(s,C);
#define error(f,...) \
	{	\
	printf(f" at :\n",##__VA_ARGS__); \
		char * p = &s->code[s->cur]; \
		while (*p != '\n' && *p != EOF) {\
			printf("%c",*p); \
			p++; \
		} \
		printf("\n");\
	}	
#define expected(X) \
	error(#X);
/*
 * 一下产生式来自c语言文档n1548
 */

/* 表达式 */

P( primary_expression			);
P( postfix_expression			);
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
P( declaration					);
P( declaration_specifiers		);
P( init_declarator_list			);
P( init_declarator				);
P( storage_class_specifier      );
P( type_specifier				);
P( struct_specifier				);
P( struct_declaration_list		);
P( struct_declaration			);
P( specifier_qualifier_list		);
P( struct_declarator			);
P( enum_specifier				);
P( enumerator_list				);
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


#undef P
#define P(X) Node * X(Source * s)


P_(expression);
