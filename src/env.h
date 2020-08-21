#pragma once
//收集的信息
//
 
struct StringPool
{
	int size;
	int use;
	char * pool;
};
typedef struct StringPool StringPool;


/* 装符号表啥的 
 */
struct Env 
{
	StringPool stringPool;
};
typedef struct Env Env;



void init_env(Env * env);
