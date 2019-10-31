#include "lex.h"
#include "parser.h"


P(struct_declarator)
{
	CHECK_FIRST(struct_declarator);
	Node * sd = newNode(STRUCT_DECLARATOR);
	if (look(s).tag == ':') {
		addChild(sd, newAttrNode(next(s)));
		Node * ce = NODE(constant_expression);
		if (!ce) {
			expected(Constant_expression);
		}
		addChild(sd, ce);
	}
	return sd;
}
P(struct_declaration_list)
{
	CHECK_FIRST(struct_declarator);
	Node * sdl = newNode(STRUCT_DECLARATION_LIST);
	addChild(sdl, NODE(struct_declarator));
	while (look(s).tag == ',') {
		next(s);
		if (!IN_FIRST(struct_declarator)) {
			expected(Struct_declarator);
			break;
		}
		Node * t = newNode(STRUCT_DECLARATION_LIST);
		addChild(t, sdl);
		addChild(t, newNode(','));
		addChild(t, NODE(struct_declarator));
		sdl = t;
	}
	return sdl;
}

P(struct_specifier)
{
	CHECK_FIRST(struct_specifier);
    Node * ss = newNode(STRUCT_SPECIFIER);
    addChild(ss, newNode(next(s).tag));
	if (look(s).tag == Id) {
		addChild(ss, newAttrNode(next(s)));
	}
	if (look(s).tag == '{') {
		addChild(ss, newAttrNode(next(s)));
		addChild(ss, NODE(struct_declaration_list));
		if (look(s).tag != '}') {
			expected("}");
		}
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

P(enumerator_list)
{
	CHECK_FIRST(enumerator_list);
	Node * el = newNode(ENUMERATOR_LIST);
	addChild(el ,el);
	while (look(s).tag == ',') {
		next(s);
		Node * et = NODE(enumerator);
		if (!et) {
			return el;
		}
		Node * t = newNode(ENUMERATOR_LIST);
		addChild(t, el);
		addChild(t, newNode(',')); 
		addChild(t, et);
		el = t;
	}
	return el;
}

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
		if (look(s).tag != '}') {
			expected("}");
		}
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
	printf("block item\n");
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
	addChild(cs,NODE(block_item_list));
	if (look(s).tag != '}') {
		expected("}");
	}else {
		addChild(cs, newAttrNode(next(s)));
	}
	return cs;
	
}

P(init_declarator)
{
	CHECK_FIRST(init_declarator);
	Node * id = newNode(INIT_DECLARATOR);
	addChild(id, NODE(declarator));
	if (look(s).tag != '=') return id;

	addChild(id, newAttrNode(next(s)));
	Node * it = NODE(initializer);
	if (!it) {
		expected(Initializer);
	}
	addChild(id, it);
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
}

P_(declaration_list)
{
	if (!IN_FIRST(declaration)) return first;
	Node * dl = newNode(DECLARATION_LIST);
	addChild(dl, first);
	addChild(dl, NODE(declaration));
	return declaration_list_(dl, s);
}

P(declaration_list)
{
	CHECK_FIRST(declaration_list);
	Node * dl = newNode(DECLARATION_LIST);
	addChild(dl, NODE(declaration));
	return declaration_list_(dl, s);
}

P(parameter_declaration)
{
	CHECK_FIRST(parameter_declaration);
	Node * pd = newNode(PARAMETER_DECLARATION);
	addChild(pd, NODE(declaration_specifiers));

}

P(parameter_list)
{
	CHECK_FIRST(parameter_list);
	Node * pl = newNode(PARAMETER_LIST);
	addChild(pl, NODE(parameter_declaration));
	while (look(s).tag == ',') {
		next(s);
		Node * pd = NODE(parameter_declaration);
		if (!pd) {
			expected(parameter_declaration);
			return pl;
		}

		Node * t = newNode(PARAMETER_LIST);
		addChild(t, pl);
		addChild(t, newNode(','));
		addChild(t, pd);
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
			if (!dr) {
				expected(Declarator);
			}
			addChild(dd, dr);
			X(')');
			addChild(dd, newNode(')'));
			break;
	}

	while (look(s).tag == '(') {
		Node * t = newNode(dd->op);
		addChild(t, dd);
		addChild(t,newAttrNode(next(s)));
		addChild(t, NODE(parameter_list));
		X(')');
		addChild(t, newAttrNode(next(s)));

		dd = t;
	}
	return dd;
}

P(declarator)
{
    CHECK_FIRST(declarator);
    Node * dc = newNode(DECLARATOR);
	if (IN_FIRST(pointer)) {
		addChild(dc, NODE(pointer));
	}
	Node * dd = NODE(direct_declarator);
	if (!dd) {
		expected(Direct_declarator);
	}
	addChild(dc, dd);
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

P_(init_declarator_list)
{
	if (eos(s) || look(s).tag != ',') return first;

	Node * initl = newNode(INIT_DECLARATOR_LIST);
	addChild(initl, first);
	addChild(initl, NODE(init_declarator));
	return init_declarator_list_(initl, s);
}

P(initializer_list)
{
	CHECK_FIRST(initializer_list);
	Node * initerl = newNode(INITIALIZER_LIST);
	if (IN_FIRST(designation)) {
		addChild(initerl, NODE(designation));
	}

}

P(designator)
{
	Node * dt = newNode(DESIGNATOR);
	switch(look(s).tag) {
		case '[':
			addChild(dt, newNode(next(s).tag));
			addChild(dt, NODE(constant_expression));
			match(s,']');
			addChild(dt, newNode(']'));
			break;
		case '.':
			addChild(dt, newNode(next(s).tag));
			Token t = next(s);
			if (t.tag != Id) {
				expected(Id);
			}
			Node * id = newNode(Id);
			id->attr = t.attr;
			addChild(dt, id);
			break;
	}
}

P_(designator_list)
{
	if (eos(s) || !IN_FIRST(designator)) return first;
	Node * dl = newNode(DESIGNATOR_LIST);
	addChild(dl, first);
	addChild(dl, NODE(designator));
	return designator_list_(dl, s);
}

P(designation)
{
	CHECK_FIRST(designation);
	Node * des = newNode(DESIGNATION);
	Node * dr = NODE(designator);
	Node * dl = newNode(DESIGNATOR_LIST);
	addChild(dl, dr);
	addChild(des, designator_list_(dl,s));
	match(s, '=');
	addChild(des,newNode('='));
	return des;
}

P(initializer)
{
	CHECK_FIRST(initializer);
	Node * initer = newNode(INITIALIZER);
	if (look(s).tag == '{' ) {
		addChild(initer, newNode(next(s).tag));
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
		addChild(d, newNode(next(s).tag));
		addChild(n , d);
		return n;
	}
	//他俩下一个都是
	if (!IN_FIRST(declarator)) {
		expected(Declarator);
		return NULL;
	}

	Node * dr = NODE(declarator);
	if (look(s).tag == ';') { //类上

		Node * initd = newNode(INIT_DECLARATOR);
		addChild(initd, dr);

		Node * initdl = newNode(INIT_DECLARATOR_LIST);
		addChild(initdl, initd);

		Node * d = newNode(DECLARATION);
		addChild(d, declspec);
		addChild(d, initdl);
		addChild(d, newNode(next(s).tag));
		addChild(n, d);
	}else if (look(s).tag == '=') {
		Node * initd = newNode(INIT_DECLARATOR);
		addChild(initd, dr);
		addChild(initd, newNode(next(s).tag));
		addChild(initd, NODE(initializer)); 

		Node * d = newNode(DECLARATION);
		addChild(d, declspec);
		Node * initl = newNode(INIT_DECLARATOR_LIST);
		addChild(initl, initd);
		addChild(d,init_declarator_list_(initl, s));
		addChild(n, d);
		X(';');
		addChild(d, newNode(';'));
	}else if (IN_FIRST(declaration_list) || IN_FIRST(compound_statement) ){//function
		Node * funcd = newNode(FUNCTION_DEFINITION);
		addChild(funcd, declspec);
		addChild(funcd, dr);

		if (IN_FIRST(declaration_list)) { 
			addChild(funcd, NODE(declaration_list));
			addChild(funcd, NODE(compound_statement));
		}else if (IN_FIRST(compound_statement)) {
			addChild(funcd, NODE(compound_statement));
		}   
		addChild(n, funcd);
	}
	return n;
}

/*
 * 改写产生式
 * A    ->  Aa
 *      |   a
 * =>
 * A    ->  aA'
 * A'   ->  aA' | 
 */

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

