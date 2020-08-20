#include "lex.h"
#include "parser.h"

P(type_name)
{
	CHECK_FIRST(type_name);
	Node * ret = newNode(TYPE_NAME);
	addChild(ret, NODE(specifier_qualifier_list));
	if (IN_FIRST(declarator)) {
		addChild(ret, NODE(declarator));
	}
	return ret;
}

P(specifier_qualifier_list)
{
	CHECK_FIRST(specifier_qualifier_list);
	Node * ret = newNode(SPECIFIER_QUALIFIER_LIST);
	if (IN_FIRST(type_qualifier)) {
		addChild(ret, NODE(type_qualifier));
	}else {
		addChild(ret, NODE(type_specifier));
	}
	while (IN_FIRST(specifier_qualifier_list)) {
		Node * t = newNode(SPECIFIER_QUALIFIER_LIST);
		if (IN_FIRST(type_qualifier)) {
			addChild(ret, NODE(type_qualifier));
		}else {
			addChild(ret, NODE(type_specifier));
		}
		addChild(ret, t);
	}
	return ret;
}

LEFT_RECURSIVE(struct_declarator_list, declarator, STRUCT_DECLARATOR_LIST, ',')
P(struct_declaration)
{
	Node * sd = newNode(STRUCT_DECLARATION);
	addChild(sd, NODE(specifier_qualifier_list));
	if (IN_FIRST(struct_declarator_list)) {
		addChild(sd, NODE(struct_declarator_list));
	}
	X(';');
	addChild(sd, newAttrNode(next(s)));
	return sd;
}
P(struct_declaration_list)
{
	CHECK_FIRST(struct_declaration);
	Node * sdl = newNode(STRUCT_DECLARATION_LIST);
	addChild(sdl, NODE(struct_declaration));

	while (IN_FIRST(struct_declaration)) {
		Node * t = newNode(STRUCT_DECLARATION_LIST);
		addChild(t, sdl);
		addChild(t, NODE(struct_declaration));
		sdl = t;
	}
	return sdl;
}

P(struct_specifier)
{
	CHECK_FIRST(struct_specifier);
    Node * ss = newNode(STRUCT_SPECIFIER);
    addChild(ss, newAttrNode(next(s)));

	if (look(s).tag == Id) {
		addChild(ss, newAttrNode(next(s)));
		if (look(s).tag == '{') {
			addChild(ss, newAttrNode(next(s)));
			if (IN_FIRST(struct_declaration_list))
				addChild(ss, NODE(struct_declaration_list));
			X('}');
			addChild(ss, newAttrNode(next(s)));
		}
	}else {
		X('{');
		addChild(ss, newAttrNode(next(s)));
		if (IN_FIRST(struct_declarator_list))
			addChild(ss, NODE(struct_declaration_list));
		X('}');
		addChild(ss, newAttrNode(next(s)));
	}
    return  ss;
}

P(enumerator)
{
	CHECK_FIRST(enumerator);
	Node * e = newNode(ENUMERATOR);
	addChild(e, newAttrNode(next(s)));
	if (look(s).tag == '=') {
		addChild(e, newAttrNode(next(s)));
		Node * ce = NODE(constant_expression);
		if (!ce) {
			expected(Constant_expression);
		}
		addChild(e, ce);
	}
	return e;
}

LEFT_RECURSIVE(enumerator_list, enumerator, ENUMERATOR_LIST, ',');
P(enum_specifier)
{
	CHECK_FIRST(enum_specifier);
    Node * ss = newNode(ENUM_SPECIFIER);
    addChild(ss, newNode(next(s).tag));
	if (look(s).tag == Id) {
		addChild(ss, newAttrNode(next(s)));
	}

	if (look(s).tag == '{') {
		addChild(ss, newAttrNode(next(s)));
		addChild(ss, NODE(enumerator_list));
		X('}');
		addChild(ss, newAttrNode(next(s)));
	}
    return  ss;
}

P(type_qualifier)
{
    CHECK_FIRST(type_qualifier);
    Node * tq = newNode(TYPE_QUALIFIER);
    addChild(tq, newNode(next(s).tag));
    return tq;
}

P(type_specifier)
{
    CHECK_FIRST(type_specifier);
    Node * ts = newNode(TYPE_SPECIFIER);
    if(IN_FIRST(struct_specifier)) {
        addChild(ts, NODE(struct_specifier));
    }else if (IN_FIRST(enum_specifier)) {
        addChild(ts, NODE(enum_specifier));
    }else {
        addChild(ts,newNode(next(s).tag));
    }
    return ts;
}

P(type_qualifier_list)
{
    CHECK_FIRST(type_qualifier_list);
    Node * tql = newNode(TYPE_QUALIFIER_LIST);
    addChild(tql, NODE(type_qualifier));
    
    return tql;
}

P(pointer)
{
    CHECK_FIRST(pointer);
    Node * p = newNode(POINTER);
    addChild(p, newNode(next(s).tag));
    if (IN_FIRST(type_qualifier_list)) {
        addChild(p, NODE(type_qualifier_list));
    }
    return p;
}


P(block_item) 
{
	CHECK_FIRST(block_item);
	Node * ret = newNode(BLOCK_ITEM);
	if (IN_FIRST(declaration)) {
		addChild(ret, NODE(declaration));
	}else if (IN_FIRST(statement)) {
		addChild(ret, NODE(statement));
	}
	return ret;
}

P(block_item_list)
{
	CHECK_FIRST(block_item_list);
	Node * bil = newNode(BLOCK_ITEM_LIST);
	addChild(bil, NODE(block_item));
	while (IN_FIRST(block_item)) {
		Node * n = newNode(BLOCK_ITEM_LIST);
		addChild(n, bil);
		addChild(n, NODE(block_item));
		bil = n;
	}
	return bil;
}

P(compound_statement)
{
	CHECK_FIRST(compound_statement);
	Node * cs = newNode(COMPOUND_STATEMENT);
	addChild(cs, newAttrNode(next(s)));
	if (IN_FIRST(block_item_list)) {
		addChild(cs,NODE(block_item_list));
	}
	X('}');
	addChild(cs, newAttrNode(next(s)));
	return cs;
	
}

P(init_declarator)
{
	CHECK_FIRST(init_declarator);
	Node * id = newNode(INIT_DECLARATOR);
	addChild(id, NODE(declarator));
	if (look(s).tag == '=') {
		addChild(id, newAttrNode(next(s)));
		addChild(id, NODE(initializer));
	}
	return id;
}


P(init_declarator_list)
{
	CHECK_FIRST(init_declarator_list);
	Node * idl = newNode(INIT_DECLARATOR_LIST); 
	addChild(idl, NODE(init_declarator));
	//这回不用辅助过程了,显式迭代
	while (!eos(s) && look(s).tag == ',') {
		Node * n = newNode(INIT_DECLARATOR_LIST);
		addChild(n,idl);
		addChild(n, newAttrNode(next(s)));
		Node * id = NODE(init_declarator);
		if (!id) {
			expected(Init_declarator);
		}
		addChild(n, id);
		idl = n;
	}
	return idl;
}

P(declaration)
{
	CHECK_FIRST(declaration);
	Node * d = newNode(DECLARATION);
	Node * ds = NODE(declaration_specifiers);
	if (!ds) {
		expected(Declaration_specifiers);
	}
	addChild(d, ds);
	Node * idl = NODE(init_declarator_list);
	if (idl) addChild(d, idl);
	if (look(s).tag == ';') {
		addChild(d, newAttrNode(next(s)));
	}else {
		expected(;);
	}
	return d;
}

P_(declaration_list)
{
	if (!IN_FIRST(declaration)) return first;
	Node * dl = newNode(DECLARATION_LIST);
	addChild(dl, first);
	addChild(dl, NODE(declaration));
	return declaration_list_(env,dl, s);
}

P(declaration_list)
{
	CHECK_FIRST(declaration_list);
	Node * dl = newNode(DECLARATION_LIST);
	addChild(dl, NODE(declaration));
	return declaration_list_(env, dl, s);
}

P(parameter_declaration)
{
	CHECK_FIRST(parameter_declaration);
	Node * pd = newNode(PARAMETER_DECLARATION);
	addChild(pd, NODE(declaration_specifiers));
	if (IN_FIRST(declarator)) {
		addChild(pd, NODE(declarator));
	}
	return pd;
}

P(parameter_list)
{
	CHECK_FIRST(parameter_list);
	Node * pl = newNode(PARAMETER_LIST);
	addChild(pl, NODE(parameter_declaration));
	while (look(s).tag == ',') {
		Node * t = newNode(PARAMETER_LIST);
		addChild(t, pl);
		addChild(t, newAttrNode(next(s)));
		addChild(t, NODE(parameter_declaration));
		pl = t;
	}
	return pl;
}
P(direct_declarator)
{
	CHECK_FIRST(direct_declarator);
	Node * dd = newNode(DIRECT_ABSTRACT_DECLARATOR);
	switch(look(s).tag) {
		case Id:
			addChild(dd, newAttrNode(next(s)));
			dd->op = DIRECT_DECLARATOR;
			break;
		case '(':
			addChild(dd, newNode(next(s).tag));
			Node * dr  = NODE(declarator);
			dd->op = dr->op == DECLARATOR?DIRECT_DECLARATOR:ABSTRACT_DECLARATOR;
			addChild(dd, dr);
			X(')');
			addChild(dd, newAttrNode(next(s)));
			break;
	}

	for (;;){
		int first [] = {'[', '('};
		if (!INSET(first)) break;
		Node * dd_temp = newNode(dd->op);
		addChild(dd_temp, dd);
		Token l = look(s);
		addChild(dd_temp, newAttrNode(next(s)));
		switch (l.tag) {
			case '[':
				X(']');
				break;
			case '(':
				if (IN_FIRST(parameter_list))
					addChild(dd_temp, NODE(parameter_list));
				X(')');
				break;
		}
		addChild(dd_temp, newAttrNode(next(s)));
		dd = dd_temp;
	}
	return dd;
}

P(declarator)
{
    CHECK_FIRST(declarator);
    Node * dc = newNode(ABSTRACT_DECLARATOR);
	if (IN_FIRST(pointer)) {
		addChild(dc, NODE(pointer));
	}
	if (IN_FIRST(direct_declarator)) {
		Node * dd = NODE(direct_declarator);
		dc->op = dd->op == DIRECT_DECLARATOR? DECLARATOR: ABSTRACT_DECLARATOR;
		addChild(dc, dd);
	}
	return dc;

}
P(storage_class_specifier)
{
	CHECK_FIRST(storage_class_specifier);
	Node * scs = newNode(STORAGE_CLASS_SPECIFIER);
	addChild(scs, newNode(next(s).tag));
	return scs;
}

P(declaration_specifiers)
{
	CHECK_FIRST(declaration_specifiers);
    Node * declSpec = newNode(DECLARATION_SPECIFIERS);
    if (IN_FIRST(storage_class_specifier)) {
        addChild(declSpec, NODE(storage_class_specifier));
    }else if (IN_FIRST(type_specifier)) {
        addChild(declSpec, NODE(type_specifier));
    }else if (IN_FIRST(type_qualifier)) {
        addChild(declSpec, NODE(type_qualifier));
    }
	if (IN_FIRST(declaration_specifiers)) {
		addChild(declSpec, NODE(declaration_specifiers));
	}
    return declSpec;
}


P(initializer_list)
{
	CHECK_FIRST(initializer_list);
	Node * initerl = newNode(INITIALIZER_LIST);
	if (IN_FIRST(designation)) {
		addChild(initerl, NODE(designation));
	}
	addChild(initerl, NODE(initializer));
	
	while (look(s).tag == ',') {
		Node * t = newNode(INITIALIZER_LIST);
		addChild(t, initerl);
		addChild(t, newAttrNode(next(s)));
		if (IN_FIRST(designation)) {
			addChild(t, NODE(designation));
		}
		addChild(t, NODE(initializer));

		initerl = t;
	}
	return initerl;
}

P(designator)
{
	Node * dt = newNode(DESIGNATOR);
	switch(look(s).tag) {
		case '[':
			addChild(dt, newAttrNode(next(s))); 
			addChild(dt, NODE(constant_expression));
			X(']');
			addChild(dt, newAttrNode(next(s)));
			break;
		case '.':
			addChild(dt, newAttrNode(next(s))); 
			if (look(s).tag != Id) {
				expected(Id);
			}
			addChild(dt, newAttrNode(next(s)));
			break;
		default:
			return NULL;
	}
			return NULL;
}

LEFT_RECURSIVE(designator_list, designator, DESIGNATOR_LIST);
P(designation)
{
	CHECK_FIRST(designation);
	Node * des = newNode(DESIGNATION);
	addChild(des, NODE(designator_list));
	X('=');
	addChild(des, newAttrNode(next(s)));
	return des;
}

P(initializer)
{
	CHECK_FIRST(initializer);
	Node * initer = newNode(INITIALIZER);
	if (look(s).tag == '{' ) {
		addChild(initer, newAttrNode(next(s))); 
		addChild(initer, NODE(initializer_list));
		match(s,'}');
		addChild(initer, newNode('}'));
	}else {
		addChild(initer, NODE(assignment_expression));
	}
	return initer;
}



P(external_declaration)
{
    //左公因子
    Node * n = newNode(EXTERNAL_DECLARATION);
    Node * declspec = NODE(declaration_specifiers);
	if (look(s).tag == ';') { //external_declaration -> declaration (init_declarator_list) ;
		Node * d = newNode(DECLARATION);
		addChild(d, declspec);
		addChild(d, newAttrNode(next(s))); 
		addChild(n , d);
		return n;
	}
	//他俩下一个都是
	if (!IN_FIRST(declarator)) {
		expected(Declarator);
		return NULL;
	}

	Node * dr = NODE(declarator);

	if (IN_FIRST(compound_statement)) 
	{
		Node * funcd = newNode(FUNCTION_DEFINITION);
		addChild(funcd, declspec);
		addChild(funcd, dr);
		addChild(funcd, NODE(compound_statement));
		addChild(n, funcd);

	}else {
		Node * d = newNode(DECLARATION);
		addChild(d, declspec);
		
		Node * initdl = newNode(INIT_DECLARATOR_LIST);
		Node * initd = newNode(INIT_DECLARATOR);
		addChild(initd, dr);
		if (look(s).tag == '=') {
			addChild(initd, newAttrNode(next(s)));
			addChild(initd, NODE(initializer));
		}
		addChild(initdl, initd);
		while (look(s).tag == ',') {
			Node * t = newNode(INIT_DECLARATOR_LIST);
			addChild(t, initdl);
			addChild(t, newAttrNode(next(s)));
			addChild(t, NODE(init_declarator));
			initdl = t;
		}
		addChild(d, initdl);
		X(';');
		addChild(d, newAttrNode(next(s)));
		addChild(n, d);
	}
	return n;
}


P(translation_unit)
{
	CHECK_FIRST(external_declaration);
	Node * tu = newNode(TRANSLATION_UNIT);
	addChild(tu, NODE(external_declaration));

	while (IN_FIRST(external_declaration)) {
		Node * t = newNode(TRANSLATION_UNIT);
		addChild(t, tu);
		addChild(t, NODE(external_declaration));
		tu = t;
	}
	return tu;
}

