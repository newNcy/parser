#pragma once
#include "lex.h"
#include <stdarg.h>


/* 语法分析 构建抽象语法树 */
enum Op{
	UNKOWN = END,
	PRIMARY_EXPRESSION			,
	POSTFIX_EXPRESSION			,
	ARGUMENT_EXPRESSION_LIST		,
	UNARY_EXPRESSION				,
	UNARY_OPERATOR				,
	CAST_EXPRESSION				,
	MULTIPLICATIVE_EXPRESSION	,
	ADDITIVE_EXPRESSION			,
	SHIFT_EXPRESSION				,
	RELATIONAL_EXPRESSION		,
	EQUALITY_EXPRESSION			,
	AND_EXPRESSION				,
	EXCLUSIVE_OR_EXPRESSION		,
	INCLUSIVE_OR_EXPRESSION		,
	LOGICAL_AND_EXPRESSION		,
	LOGICAL_OR_EXPRESSION		,
	CONDITIONAL_EXPRESSION		,
	ASSIGNMENT_EXPRESSION		,
	ASSIGNMENT_OPERATOR			,
	EXPRESSION					,
	CONSTANT_EXPRESSION			,

	/* 声明 */
	DECLARATION					,
	DECLARATION_SPECIFIERS		,
	INIT_DECLARATOR_LIST			,
	INIT_DECLARATOR				,
	STORAGE_CLASS_SPECIFIER      ,
	TYPE_SPECIFIER				,
	STRUCT_SPECIFIER				,
	STRUCT_DECLARATION_LIST		,
	STRUCT_DECLARATION			,
	SPECIFIER_QUALIFIER_LIST		,
	STRUCT_DECLARATOR			,
	ENUM_SPECIFIER				,
	ENUMERATOR_LIST				,
	ENUMERATOR					,
	TYPE_QUALIFIER				,
	FUNCTION_SPECIFIER			,
	DECLARATOR					,
	DIRECT_DECLARATOR			,
	DIRECT_ABSTRACT_DECLARATOR	,
	POINTER						,
	TYPE_QUALIFIER_LIST			,
	PARAMETER_TYPE_LIST			,
	PARAMETER_LIST				,
	PARAMETER_DECLARATION		,
	IDENTIFIER_LIST				,
	TYPE_NAME					,
	INITIALIZER					,
	INITIALIZER_LIST				,
	DESIGNATION					,
	DESIGNATOR_LIST				,
	DESIGNATOR					,

	/* 语句 */
	STATEMENT					,
	LABELED_STATEMENT			,
	COMPOUND_STATEMENT			,
	BLOCK_ITEM_LIST				,
	BLOCK_ITEM					,
	EXPRESSION_STATEMENT			,
	SELECTION_STATEMENT			,
	ITERATION_STATEMENT			,
	JUMP_STATEMENT				,

	/* 定义 */

	TRANSLATION_UNIT				,
	EXTERNAL_DECLARATION			,
	FUNCTION_DEFINITION			,
	DECLARATION_LIST				,

};

const static char * nodeName[] = 
{
	"PRIMARY_EXPRESSION",
	"POSTFIX_EXPRESSION",
	"ARGUMENT_EXPRESSION_LIST",
	"UNARY_EXPRESSION",
	"UNARY_OPERATOR",
	"CAST_EXPRESSION",
	"MULTIPLICATIVE_EXPRESSION",
	"ADDITIVE_EXPRESSION",
	"SHIFT_EXPRESSION",
	"RELATIONAL_EXPRESSION",
	"EQUALITY_EXPRESSION",
	"AND_EXPRESSION",
	"EXCLUSIVE_OR_EXPRESSION",
	"INCLUSIVE_OR_EXPRESSION",
	"LOGICAL_AND_EXPRESSION",
	"LOGICAL_OR_EXPRESSION",
	"CONDITIONAL_EXPRESSION",
	"ASSIGNMENT_EXPRESSION",
	"ASSIGNMENT_OPERATOR",
	"EXPRESSION",
	"CONSTANT_EXPRESSION",

	/* 声明 */
	"DECLARATION",
	"DECLARATION_SPECIFIERS",
	"INIT_DECLARATOR_LIST",
	"INIT_DECLARATOR",
	"STORAGE_CLASS_SPECIFIER      ",
	"TYPE_SPECIFIER",
	"STRUCT_SPECIFIER",
	"STRUCT_DECLARATION_LIST",
	"STRUCT_DECLARATION",
	"SPECIFIER_QUALIFIER_LIST",
	"STRUCT_DECLARATOR",
	"ENUM_SPECIFIER",
	"ENUMERATOR_LIST",
	"ENUMERATOR",
	"TYPE_QUALIFIER",
	"FUNCTION_SPECIFIER",
	"DECLARATOR",
	"DIRECT_DECLARATOR",
	"DIRECT_ABSTRACT_DECLARATOR",
	"POINTER",
	"TYPE_QUALIFIER_LIST",
	"PARAMETER_TYPE_LIST",
	"PARAMETER_LIST",
	"PARAMETER_DECLARATION",
	"IDENTIFIER_LIST",
	"TYPE_NAME",
	"INITIALIZER",
	"INITIALIZER_LIST",
	"DESIGNATION",
	"DESIGNATOR_LIST",
	"DESIGNATOR",

	/* 语句 */
	"STATEMENT",
	"LABELED_STATEMENT",
	"COMPOUND_STATEMENT",
	"BLOCK_ITEM_LIST",
	"BLOCK_ITEM",
	"EXPRESSION_STATEMENT",
	"SELECTION_STATEMENT",
	"ITERATION_STATEMENT",
	"JUMP_STATEMENT",

	/* 定义 */

	"TRANSLATION_UNIT",
	"EXTERNAL_DECLARATION",
	"FUNCTION_DEFINITION",
	"DECLARATION_LIST",

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
static void println(Source * s)
{
	char * p = &s->code[s->cur]; 
	while (*p != '\n' && *p != EOF) {
		printf("%c",*p); 
		p++; 
	} 
	printf("\n");
}
#define CHECK_FIRST(X) \
	printf("first of "#X" at %s:%d\n",__FILE__,__LINE__); \
	while(!eos(s) && !IN_FIRST(X)) {\
		println(s); \
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
	printf(f" at %s:%d:%d\n",__FILE__,__LINE__,##__VA_ARGS__); \
	println(s);\
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
