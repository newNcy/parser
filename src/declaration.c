#include "env.h"
#include "lex.h"
#include "map.h"
#include "parser.h"
#include "vector.h"


Type* type_name(Env * env, Source * s)
{
	Type * base = declaration_specifiers(env, s);
	if (IN_FIRST(direct_declarator)) {
		Node * n = direct_declarator(env, s);
		makeBase(n, base);
		base = n->type;
	}
	return base;
}

Node * declarator (Env * env, Source * s);

Type * storage_class_specifier(Env * env, Source * s)
{
	Tag t = next(s).tag;
	Type * type = NULL;

	if (IN_FIRST(declaration_specifiers)) type = declaration_specifiers(env, s);
	else type = new(Type, TypeNone);

	if (type->storageClass != StorageAuto) {
		error("多余的存储限定符")
	}else {
		type->storageClass = StorageTypedef + t - Typedef;
	}
	return type;
}

Type * type_qualifier(Env * env, Source * s)
{
	Tag t = next(s).tag;
	Type * type = NULL;

	if (IN_FIRST(declaration_specifiers)) type = declaration_specifiers(env, s);
	else type = new(Type, TypeNone);

	if (t == Const) {
		if (type->isConst) {
			printf("重复的const\n");
		}else {
			type->isConst = true;
		}
	}
	return type;
}



Vector * enumerator_list(Env * env, Source * s)
{
	CHECK_FIRST(enumerator_list);
	Vector * enumerators = new(Vector);
	long enumValue = 0;
	while (look(s).tag == Id) {
		char * attrName = s->stringPool.pool + next(s).attr;
		if (look(s).tag == '=') {
			next(s);
			Node * valueNode = constant_expression(env, s);
			enumValue = evalExpr(valueNode);
		}
		if (look(s).tag == ',') next(s);

		long * pre = mapSearch(env->enumValues, attrName);
		if (pre) {
			error("对枚举常量 %s 重复定义",attrName);
		}else {
			char * name = (char*)_new(SYMBOL_LEN_MAX);
			strncpy(name, attrName, SYMBOL_LEN_MAX);
			vectorPushBack(enumerators, name);
			mapInsert(env->enumValues, name, enumValue);
		}
		enumValue ++;
	}
	return enumerators;
}

Type* enum_specifer(Env * env, Source * s)
{
	next(s);
	Type * ret = NULL;
	char * name = NULL;
	//id
	if (look(s).tag == Id) {
		name = s->stringPool.pool + next(s).attr;
	}

	//定义
	Type * def = NULL;
	if (look(s).tag == '{') {
		next(s);
		def = new(Type, TypeEnum);
		def->info = enumerator_list(env, s);
		X('}');
	}

	//先前定义
	Type ** pre = NULL;
	if (name) {
		pre = mapSearch(env->types, name);
		if (pre) {
			error("对%s重复定义",name);
		}else {
			mapInsert(env->types, name, def);
		}
	}else if (!def) {
		Env * scope = env->parent;
		while (scope)
	}

	if (def) {
		ret = def;
	}else {

	}
	return ret;
}

Type * type_specifier(Env * env, Source * s)
{

	switch(look(s).tag) {
		case Void:
		case Char:
		case Short:
		case Int:
		case Float:
		case Long:
		case Double:
			{
				TypeKind kind = next(s).tag - Void + TypeVoid;
				if (!IN_FIRST(declaration_specifiers))
					return new(Type, kind);
				else  {
					Type * type = declaration_specifiers(env, s);
					if (type->kind != TypeNone) {
						error("重复的类型限定符");
					}
					type->kind = kind;
				}
			}
			break;
		case Enum:
			return enum_specifer(env, s);
		case Struct:
			break;
		case Union:
			break;
		default:
			return NULL;
	}
}

Type * declaration_specifiers(Env * env, Source * s)
{
	while (true) {
		if (IN_FIRST(storage_class_specifier)) {
			return storage_class_specifier(env, s);
		}else if (IN_FIRST(type_qualifier)) {
			return type_qualifier(env, s);
		}else if (IN_FIRST(type_specifier)) {
			return type_specifier(env, s);
		}else {
			error("声明限定符为空");
			return NULL;
		}
	}
}

Type * pointer(Env * env, Source * s)
{
	next(s);
	Type * type = NULL;
	if (IN_FIRST(type_qualifier)) {
		type = type_qualifier(env, s);
	}else type = new(Type, TypePointer);

	if (look(s).tag == '*') {
		Type * t = pointer(env, s);
		t->base = type;
		type = t;
	}
	return type;
}


Symbol * parameter_declaration(Env * env, Source * s )
{
	CHECK_FIRST(parameter_declaration);
	Type * type = declaration_specifiers(env,s);
	Node * n = declarator(env,s);
	makeBase(n,type);
	Symbol * sym = new(Symbol);
	sym->type = n->type;
	strcpy(sym->name, s->stringPool.pool + n->attr);
	return sym;
}

Vector * parameter_list(Env * env, Source * s)
{
	CHECK_FIRST(declaration_specifiers);
	Vector * parametes = new(Vector);
	Symbol * first = parameter_declaration(env, s);
	if (first) {
		vectorPushBack(parametes, first);
	}
	while (look(s).tag == ',') {
		next(s);
		Symbol * s = parameter_declaration(env, s);
		if (s) {
			vectorPushBack(parametes, s);
		}
	}
	return parametes;
}

Node * direct_declarator(Env * env, Source * s)
{
	Node * ret = NULL;
	if (look(s).tag == Id) {
		ret = new(Node, DECLARATOR);
		ret->attr = next(s).attr;
	}else if (look(s).tag == '(') {
		next(s);
		ret = declarator(env, s);
		X(')');	
	}else {
		error("需要id或者(");
		return ret;
	}

	int finish = 0;
	while (look(s).tag == '[' || look(s).tag == '(' ) {
		Type * t = NULL;
		switch(look(s).tag) {
			case '['://数组声明
				next(s);
				X(']');
				break;
			case  '('://函数声明
				next(s);
				t = new(Type,TypeFunc);
				t->info = parameter_list(env, s);
				X(')');
			default:
				finish = 1;
		}
		if (ret->type) {
			Type * b = ret->type;
			while (b && b->base) b = b->base;
			b->base = t;
		}else {
			ret->type = t;
		}
	}
	return ret;

}
Node * declarator (Env * env, Source * s)
{

	Type * type = NULL;
	if (look(s).tag == '*') {
		type = pointer(env,s);
	}

	if (IN_FIRST(direct_declarator)) {
		Node * dd = NODE(direct_declarator);
		makeBase(dd, type);
		return dd;
	}else {
		Node * n = new(Node, ABSTRACT_DECLARATOR);
		n->type = type;
		return n;
	}

}

Node * init_declarator(Env * env, Source * s, Type * type)
{
}

Node * compound_statement(Env * env, Source * s)
{
}

Node * initializer(Env * env, Source *s)
{
}
Node * external_declaration(Env * env, Source * s)
{
	//左公因子
	Node * n = newNode(EXTERNAL_DECLARATION);
	Type * type = declaration_specifiers(env, s);
	if (look(s).tag == ';') { //external_declaration -> declaration (init_declarator_list) ;
		next(s);
		Node * d = newNode(DECLARATION);
		addChild(n , d);
		return n;
	}
	//他俩下一个都是
	Node * dr = declarator(env, s);
	makeBase(dr, type);

	printf("%s is a ",s->stringPool.pool + dr->attr);
	dumpType(dr->type);
	if (IN_FIRST(compound_statement)) 
	{
		Node * funcd = newNode(FUNCTION_DEFINITION);
		addChild(funcd, dr);
		Node * body = NODE(compound_statement);
		addChild(funcd, body);
		addChild(n, funcd);

	}else {
		Node * d = newNode(DECLARATION);

		Node * initdl = newNode(INIT_DECLARATOR_LIST);
		Node * initd = newNode(INIT_DECLARATOR);
		addChild(initd, dr);
		if (look(s).tag == '=') {
			next(s);
			addChild(initd, NODE(initializer));
		}
		addChild(initdl, initd);
		while (look(s).tag == ',') {
			next(s);
			Node * t = newNode(INIT_DECLARATOR_LIST);
			addChild(t, initdl);
			addChild(t, init_declarator(env, s, type));
			initdl = t;
		}
		addChild(d, initdl);
		X(';');
		addChild(n, d);
	}


	//for typedef 
	return n;
}


Vector * translation_unit(Env * env, Source * s)
{
	CHECK_FIRST(external_declaration);

	Vector * top = new(Vector);
	while (IN_FIRST(external_declaration)) {
		vectorPushBack(top, external_declaration(env, s) );
	}
}

