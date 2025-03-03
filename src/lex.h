#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "new.h"
#include "env.h"

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
struct _Source 
{
	char fileName[256];
	int len;
	char * code;
	int cur;
	Token look;
};

typedef struct _Source Source;

void deleteSource(Source ** s);
Source * newSource(unsigned int size);

Source * sourceFromFile(const char * filePath);

/* 从文件中读取源代码 */
int eos(Source * s);


/* 从字符串字面量中读取代码 */
Source * sourceFromStr(const char * code);



Token parseNum(Source * s, Env *);

Token parseIdOrKeyword(Source * s, Env * env);

Token next(Source * s,Env * env);
/*
 * 向前看
 */
inline static Token look(Source * s, Env * env)
{
	//向前看缓冲没有
	if (s->look.tag == NoLook) {
		s->look = next(s, env);
	}
	return s->look;
}
