#include "lex.h"
#include "parser.h"



P(struct_specifier)
{
    Node * ss = newNode(STRUCT_SPECIFIER);
    addChild(ss, newNode(next(s).tag));
    return  ss;
}

P(enum_specifier)
{
    Node * ss = newNode(ENUM_SPECIFIER);
    addChild(ss, newNode(next(s).tag));
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


P(declarator)
{
    CHECK_FIRST(declarator);
    Node * dc = newNode(DECLARATOR);
    if (look(s).tag == '*') {

    }

}
P(storage_class_specifier)
{

}

P(declaration_specifiers)
{
    Node * declSpec = newNode(DECLARATION_SPECIFIERS);
    if (IN_FIRST(storage_class_specifier)) {
        addChild(declSpec, NODE(storage_class_specifier));
    }else if (IN_FIRST(type_specifier)) {
        addChild(declSpec, NODE(type_specifier));
    }else if (IN_FIRST(type_qualifier)) {
        addChild(declSpec, NODE(type_qualifier));
    }
    return declSpec;
}

P_(init_declarator_list)
{
	Node * initl = newNode(INIT_DECLARATOR_LIST);
	addChild(initl, first);
	if (eos(s)) return initl;
	
	Node * ret = newNode(0);
}

P(initializer_list)
{
	CHECK_FIRST(initializer_list);
	Node * initerl = newNode(INITIALIZER_LIST);
	if (IN_FIRST(designation)) {
		addChild(initerl, NODE(designation));
	}

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
		printf("error\n");
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
		Node * initl = newNode(INIT_DECLARATOR_LIST);
		addChild(initl, initd);
		addChild(d,init_declarator_list_(initl, s));
		addChild(n, d);
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
P_(translation_unit)
{
	if (eos(s)) return first;
	Node * n = newNode(TRANSLATE_UNIT);
	addChild(n, first);
	CHECK_FIRST(external_declaration);
	addChild(n, NODE(external_declaration));
	return translation_unit_(n, s);
}

P(translation_unit)
{
	if(eos(s)) return NULL;
	Node * tu = newNode(TRANSLATE_UNIT);
	CHECK_FIRST(external_declaration);
	addChild(tu, NODE(external_declaration));
	return translation_unit_(tu, s);
}

