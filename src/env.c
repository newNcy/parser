#include "env.h"
#include "lex.h"
#include "map.h"
#include "vector.h"
#include <string.h>

Type * newType(TypeKind kind)
{
	Type * type = (Type*)_new(sizeof(Type));
	type->kind = kind;
	return type;
}


int stringCompare(void * owner, Key a,Key b)
{
	char * stra = keyToPtr(a);
	char * strb = keyToPtr(b);
	if (!a || !b) return 0;
	return strcmp(stra,strb);
}

void stringSave(Env* env,Pair * pair, Key key, Value value)
{
	char * str = keyToPtr(key);
	if (!str) return;
	int len = strlen(str);
	char * skey = _new(len);
	strcpy(skey, str);
	pair->first = ptrToKey(skey);
	pair->second = value;
}

void stringDelete(Env * env, Pair * pair)
{
	delete(pair->first);
}

Env * newEnv(Env * parent)
{
	Env * ret = (Env*)_new(sizeof(Env));
	ret->parent = parent;
	ret->symbols = new(RBTree,ret, stringCompare, (Save)stringSave, (Delete)stringDelete);
	ret->types= new(RBTree,ret, stringCompare, (Save)stringSave, (Delete)stringDelete);
	return ret;
}

Symbol * newSymbol()
{
	Symbol * sym = (Symbol *)_new(sizeof(Symbol));
	return sym;
}
void dumpType(Type * type)
{
	if (!type) return;
	if (type->kind >= TypeVoid && type->kind <= TypeFunc) {
		printf("%s ",debugTypeKind[type->kind-TypeVoid]);
	}
	if (type->kind == TypeFunc) {
		printf("take args: (");
		Vector * v = type->info;
		int m = v->size;
		for (int i = 0 ; i < m ; i++) {
			Symbol * s = vectorAt(v, i);
			printf ("%s:",s->name);
			dumpType(s->type);
		}
		printf(") returns ");
	}
	if (type->base) 
		dumpType(type->base);
}
