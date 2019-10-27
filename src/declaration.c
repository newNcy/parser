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

P(external_declaration)
{
    //左公因子
    Node * n = newNode(UNKOWN);
    Node * declspec = NODE(declaration_specifiers);
    addChild(n, declspec);
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

