#pragma once

#include "lex.h"
#include "map.h"
#include "vector.h"

#define SYMBOL_LEN_MAX 256
struct Type;
typedef struct Type Type;


typedef struct 
{
	char name[SYMBOL_LEN_MAX];
	Type * type;
}StructDeclarator;


//收集的信息
typedef struct Env 
{	
	struct Env * parent;
	RBTree * enumValues; 
	RBTree * types; //存储自定义类型 <name, Vector*<StructDeclarator*>>
	RBTree * symbols; //<name, Symbol*>
}Env;


typedef enum 
{
	StorageAuto,
	StorageTypedef,
	StorageExtern,
	StorageStatic,
}StorageClass;

typedef enum 
{
	TypeNone,
	TypeVoid,
	TypeChar, 
	TypeShort, 
	TypeInt, 
	TypeFloat, 
	TypeLong, 
	TypeDouble,
	TypeEnum, 

	TypeStruct, 
	TypeUnion, 
	TypePointer,
	TypeArray,

	TypeFunc
}TypeKind;

static const char * debugTypeKind [] = 
{
	"void",
	"char", 
	"short", 
	"int", 
	"float", 
	"long", 
	"double",
	"enum", 

	"struct", 
	"union", 
	"pointer to",
	"array",

	"func"
};

struct ArrayInfo
{
	Type * base;
	unsigned len;
};

struct FunctionType;
typedef struct 
{
	char name[SYMBOL_LEN_MAX];
	Type * type;
}Symbol;



struct Type 
{
	TypeKind kind;
	struct Type * base; //指针指向类型/数组元素类型/函数返回值/null
	StorageClass storageClass;
	int isInline;//函数
	int isUnsigned;
	int isConst;
	Vector * info;//union/struct成员列表/enum常量列表/function参数列表
};


Type * newType(TypeKind);
Symbol * newSymbol();
Env * newEnv(Env * parent);
void dumpType(Type *);
