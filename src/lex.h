#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum Tag
{
	//类型
	Void = 256, Char, Short, Int, Long, Float, Double, Unsigned, Enum, Struct,
	Sizeof,
	Id,

	If, Else, Switch, Case, Default, While, For, Continue, Break, Return,
	Extern, Static, Const, Inline, 

	ConstChar,ConstInt,ConstFloat,ConstLong,ConstDouble,ConstStr,
	//括号
	//符号
	
	//只有两个字符才要表示 单个字符直接返回
	Pto, // ->
	SPlus, SSub, // ++ --
	Lsft, Rsft, //<< >>
	
	Aeq, Seq, Teq, Deq, Meq,  // += -= *= /= %= 
	AndEq, OrEq, XorEq, // &= |= ^=
	LsEq, RsEq, //>>= <<=

	Eq, Neq, Geq, Leq, And, Or, // == != >= <= && ||
	END,
	NoLook
}Tag;

static const char * tagName[] = 
{
	"Void ", "Char", "Short", "Int", "Long", "Float", "Double", "Unsigned", "Enum", "Struct",
	"Sizeof",
	"Id",

	"If", "Else", "Switch", "Case", "Default", "While", "For", "Continue", "Break", "Return",
	"Extern", "Static", "Const", "Inline", 

	"ConstChar","ConstInt","ConstFloat","ConstLong","ConstDouble","ConstStr",
	//括号
	//符号
	
	//只有两个字符才要表示 单个字符直接返回
	"Pto", // ->
	"SPlus", "SSub",// ++ --
	"Lsft", "Rsft", //<< >>
	
	"Aeq", "Seq", "Teq", "Deq", "Meq",  // += -= *= /= %= 
	"AndEq", "OrEq", "XorEq", // &= |= ^=
	"LsEq", "RsEq", //>>= <<=

	"Eq", "Neq", "Geq", "Leq", "And", "Or", // == != >= <= && ||
	"END"
};

struct MapEntry
{
	char text[32];
	long tag;
};

typedef struct MapEntry MapEntry;

static MapEntry keyMap [] =
{
	{"void",	Void},
	{"char",	Char},
	{"short",	Short},
	{"int",		Int},
	{"float",	Float},
	{"long",	Long},
	{"double",	Double},
	{"struct",	Struct},
	{"enum",	Enum},
	{"sizeof",	Sizeof},
    {"unsigned",Unsigned},
    {"const",   Const},

    {"static",  Static},
    {"extern",  Extern},
    {"inline",  Inline},

	{"if",		If},
	{"else",	Else},
	{"switch",	Switch},
	{"case",	Case},
	{"default",	Default},
	{"while",	While},
	{"for",		For},
	{"continue",Continue},
	{"break",	Break},
	{"return",	Return}
};



typedef struct Token
{
	Tag tag;
	long attr;
}Token ;

struct StringPool
{
	int size;
	int use;
	char * pool;
};
typedef struct StringPool StringPool;

struct _Source 
{
	char fileName[256];
	int len;
	char * code;
	int cur;
	StringPool stringPool;
	Token look;
};

typedef struct _Source Source;

static void * new(unsigned int size)
{
	static unsigned int sum = 0;
	void * ret = malloc(size);
	memset(ret, 0, size);
	if (ret) {
		sum += size;
		return ret;
	}
	return NULL;
}

void deleteSource(Source ** s);
Source * newSource(unsigned int size);

Source * sourceFromFile(const char * filePath);

/* 从文件中读取源代码 */
int eos(Source * s);


/* 从字符串字面量中读取代码 */
Source * sourceFromStr(const char * code);



Token parseNum(Source * s);

Token parseIdOrKeyword(Source * s);

Token next(Source * s);
/*
 * 向前看
 */
inline static Token look(Source * s)
{
	//向前看缓冲没有
	if (s->look.tag == NoLook) {
		s->look = next(s);
	}
	return s->look;
}

Token next(Source * s);
