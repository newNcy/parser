#include "parser.h"

FIRST(primary_expression)
{
	switch(look(s).tag) {
		case Id:
		case ConstChar:
		case ConstInt:
		case ConstFloat:
		case ConstLong:
		case ConstDouble:
		case ConstStr:
		case '(':
			return 1;
		default:
			return 0;
	}

}

FIRST(postfix_expression) 
{
    return IN_FIRST(primary_expression);
}

FIRST(unary_expression)
{
	switch(look(s).tag) {
		/* first set of unary expr*/
		case SPlus:
		case SSub:
		case '&':
		case '*':
		case '+':
		case '-':
		case '~':
		case '!':
		case Sizeof:
			return 1;
		/* first set of primary expr*/
        default:
	        return IN_FIRST(postfix_expression);
	}

}

FIRST(enum_specifier)
{
    if (look(s).tag == Enum) return 1;
    return 0;
}
FIRST(struct_specifier)
{
    if (look(s).tag == Struct) return 1;
    return 0;
}
FIRST(type_specifier)
{
    switch(look(s).tag) {
        case Void:
        case Char:
        case Short:
        case Int:
        case Long:
        case Float:
        case Double:
        case Unsigned:
            return 1;
        default:
            return IN_FIRST(struct_specifier) || IN_FIRST(enum_specifier);
    }
}

FIRST(type_qualifier)
{
    if (look(s).tag == Const) return 1;
    return 0;
}
FIRST(storage_class_specifier)
{
    switch(look(s).tag) {
    case Extern:
    case Static:
        return 1;
    default:
        return 0;
    }
}

FIRST(declarator)
{
    return IN_FIRST(pointer) || IN_FIRST(direct_declarator);
}
FIRST(type_qualifier_list)
{
    return IN_FIRST(type_qualifier);
}

FIRST(pointer)
{
    if (look(s).tag == '*') return 1;
    return 0;
}

FIRST(specifier_qualifier_list)
{
    return IN_FIRST(type_specifier) || IN_FIRST(type_qualifier);
}

FIRST(type_name) 
{
    return IN_FIRST(specifier_qualifier_list);
}


