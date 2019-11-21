#pragma once
#include "lex.h"
#include "env.h"
#include <stdarg.h>


/* 语法分析 构建抽象语法树 */
enum Op{
	UNKOWN = END,

	FUNC_BODY					,
	FUNC_NAME					,
	FUNC_CALL					,
	ARRAY_OFFSET				,
	ARRAYTYPE,
	POINTERTYPE,

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
	STRUCT_DECLARATOR_LIST,
	ENUM_SPECIFIER				,
	ENUMERATOR_LIST				,
	ENUMERATOR					,
	TYPE_QUALIFIER				,
	FUNCTION_SPECIFIER			,
	DECLARATOR					,
	ABSTRACT_DECLARATOR			,
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
	"FUNC_BODY",
	"FUNC_NAME",
	"FUNC_CALL",
	"ARRAY_OFFSET",
	"ATTAY",
	"POINTER",
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
	"STRUCT_DECLARATOR_LIST",	
	"ENUM_SPECIFIER",
	"ENUMERATOR_LIST",
	"ENUMERATOR",
	"TYPE_QUALIFIER",
	"FUNCTION_SPECIFIER",
	"DECLARATOR",
	"ABSTRACT_DECLARATOR"			,
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
	Type * type;
};

typedef struct Node Node;

Node * newNode(Op op);
Node * newAttrNode(Token t);
void addChild(Node * p, Node * c);
void printTag(Tag t);
void printNode(Source * s, Node * n);
int match(Source * s,Tag t);



#define FIRST(X) int first_##X(Source * s) //求某个符号的first集
FIRST( primary_expression			);
FIRST( postfix_expression			);
FIRST( argument_expression_list		);
FIRST( unary_expression				);
FIRST( unary_operator				);
FIRST( cast_expression				);
FIRST( multiplicative_expression	);
FIRST( additive_expression			);
FIRST( shift_expression				);
FIRST( relational_expression		);
FIRST( equality_expression			);
FIRST( AND_expression				);
FIRST( exclusive_OR_expression		);
FIRST( inclusive_OR_expression		);
FIRST( logical_AND_expression		);
FIRST( logical_OR_expression		);
FIRST( conditional_expression		);
FIRST( assignment_expression		);
FIRST( assignment_operator			);
FIRST( expression					);
FIRST( constant_expression			);

/* 声明 */
FIRST( declaration					);
FIRST( declaration_specifiers		);
FIRST( init_declarator_list			);
FIRST( init_declarator				);
FIRST( storage_class_specifier      );
FIRST( type_specifier				);
FIRST( struct_specifier				);
FIRST( struct_declaration_list		);
FIRST( struct_declaration			);
FIRST( specifier_qualifier_list		);
FIRST( struct_declarator			);
FIRST( struct_declarator_list		);
FIRST( enum_specifier				);
FIRST( enumerator_list				);
FIRST( enumerator					);
FIRST( type_qualifier				);
FIRST( function_specifier			);
FIRST( declarator					);
FIRST( direct_declarator			);
FIRST( pointer						);
FIRST( type_qualifier_list			);
FIRST( parameter_type_list			);
FIRST( parameter_list				);
FIRST( parameter_declaration		);
FIRST( identifier_list				);
FIRST( type_name					);
FIRST( initializer					);
FIRST( initializer_list				);
FIRST( designation					);
FIRST( designator_list				);
FIRST( designator					);

/* 语句 */
FIRST( statement					);
FIRST( labeled_statement			);
FIRST( compound_statement			);
FIRST( compound_statement			);
FIRST( block_item_list				);
FIRST( block_item					);
FIRST( expression_statement			);
FIRST( selection_statement			);
FIRST( iteration_statement			);
FIRST( jump_statement				);

/* 定义 */

FIRST( translation_unit				);
FIRST( external_declaration			);
FIRST( function_definition			);
FIRST( declaration_list				);


#define IN_FIRST(X) (!eos(s) && first_##X(s) )



///////////////////////////////////////////////
Type * declaration_specifiers(Env * env, Source * s);
Type * type_qualifier(Env * env, Source * s);
Type * type_specifier(Env * env, Source * s);
Node * direct_declarator(Env * env, Source * s);
/////////////////////////////////////////////////////

static void println(Source * s)
{
	char * p = &(s->code[s->cur]); 
    int at = -1;
    while (*p!='\n') {
        -- p;
        ++ at;
    }
    p++;
	if (s->look.tag != NoLook) at --;
	while (*p != '\n' && *p != EOF) {
		printf("%c",*p); 
		p++; 
	} 
    printf("\n");
    if(at >0) {
        while (at --) printf(" ");
    }
	printf("^\n");
}

static int inset(int c, int set[], int len)
{
	for (int i = 0; i < len; i++) {
		if (c == set[i]) return 1;
	}
	fflush(stdout);
	return 0;
}

#define INSET(X) inset(look(s).tag, X,sizeof(X)/sizeof(int) )
#define CHECK_FIRST(X) \
	if (eos(s) || !IN_FIRST(X)) {\
	    printf("first of "#X" at %s:%d\n",__FILE__,__LINE__); \
	    println(s); \
	} \


#define P_(X) Node * X##_	(Env * env, Node * first, Source * s)
#define NODE(X) X(env, s)
#define error(f,...) \
{	\
	printf("error: "f" at %s:%d\n",__FILE__,__LINE__,##__VA_ARGS__); \
}	
#define X(C) match(s,C);
#define expected(X) \
	error(#X);

//左递归
//S -> A | SaA | SbA | ScA的统一定义
//SN为S标号 AN为A标号
// S ->AS'
// S'->aAS'| bAS' | cAS' |空
// ... = {a, b, }
#define LEFT_RECURSIVE(S, A, SN, ...)								\
Node * S(Env * env, Source * s)										\
{																	\
	CHECK_FIRST(S);													\
	/* A*/															\
	Node * ret = A(env, s);											\
																	\
	/* S' */														\
	int _s[] = {__VA_ARGS__};										\
	while (INSET(_s) || (!sizeof(_s) && IN_FIRST(A))) {				\
		/* _s */													\
		if (sizeof(_s))	{											\
			Node * t = newAttrNode(next(s));						\
			addChild(t, ret);										\
			addChild(t, A(env, s));									\
			ret = t;												\
		}else {														\
			Node * last = ret;										\
			while (last->sbling) {									\
				last = last->sbling;								\
			}														\
			last->sbling = A(env, s);								\
		}															\
																	\
		/* A */														\
	}																\
	return ret;														\
}

#define Call(X,...) X(env,s,#__VA_ARGS__)


void makeBase(Node * n, Type * base);

