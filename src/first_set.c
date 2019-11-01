#include "lex.h"
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


FIRST(unary_operator)
{
	switch(look(s).tag) {
		case '&':
		case '*':
		case '+':
		case '-':
		case '~':
		case '!':
			return 1;
			break;
		default:
			return 0;
	}
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

FIRST(assignment_operator)
{
	switch(look(s).tag) {
		case '=':
		case Teq:
		case Deq:
		case Meq:
		case Aeq:
		case Seq:
		case LsEq:
		case RsEq:
		case AndEq:
		case OrEq:
		case XorEq:
			return 1;
		default:return 0;
	}
}


FIRST(argument_expression_list)
{
	return IN_FIRST(assignment_expression);
}

FIRST(labeled_statement)
{
	return look(s).tag == Case || look(s).tag == Default;
}
FIRST(expression_statement)
{
	return IN_FIRST(expression) || look(s).tag == ';';
}

FIRST(selection_statement)
{
	return look(s).tag == If || look(s).tag == Switch;
}

FIRST(iteration_statement)
{
	return look(s).tag == While || look(s).tag == For;
}

FIRST(jump_statement) 
{
	return look(s).tag == Continue || look(s).tag == Break || look(s).tag == Return;

}
FIRST(statement)
{
	return IN_FIRST(labeled_statement) 
		|| IN_FIRST(compound_statement)
		|| IN_FIRST(expression_statement)
		|| IN_FIRST(selection_statement)
		|| IN_FIRST(iteration_statement)
		|| IN_FIRST(jump_statement);
}

FIRST(block_item)
{
	return IN_FIRST(declaration) || IN_FIRST(statement);
}

FIRST(block_item_list)
{
	return IN_FIRST(block_item);
}

FIRST(compound_statement)
{
	return look(s).tag == '{';
}

FIRST(declaration_list)
{
	return IN_FIRST(declaration);
}

FIRST(designator_list)
{
	return IN_FIRST(designator);
}

FIRST(designator)
{
	return look(s).tag == '[' || look(s).tag == '.';
}

FIRST(cast_expression)
{
	return look(s).tag == '(' || IN_FIRST(unary_expression);
}

FIRST(multiplicative_expression)
{
	return IN_FIRST(cast_expression);
}

FIRST(additive_expression)
{
	return IN_FIRST(multiplicative_expression);
}

FIRST(shift_expression)
{
	return IN_FIRST(additive_expression);
}
FIRST(relational_expression)
{
	return IN_FIRST(shift_expression);
}

FIRST(equality_expression)
{
	return IN_FIRST(relational_expression);
}

FIRST(AND_expression)
{
	return IN_FIRST(equality_expression);
}

FIRST(exclusive_OR_expression)
{
	return IN_FIRST(AND_expression);
}
FIRST(inclusive_OR_expression)
{
	return IN_FIRST(exclusive_OR_expression);
}
FIRST(logical_AND_expression)
{
	return IN_FIRST(inclusive_OR_expression);
}

FIRST(logical_OR_expression)
{
	return IN_FIRST(logical_AND_expression);
}
FIRST(conditional_expression)
{
	return IN_FIRST(logical_OR_expression);
}

FIRST(assignment_expression)
{
	return IN_FIRST(conditional_expression); 
}

FIRST(expression) 
{
	return IN_FIRST(assignment_expression);
}

FIRST(constant_expression)
{
	return IN_FIRST(conditional_expression);
}

FIRST(struct_declarator)
{
	return IN_FIRST(declarator) || look(s).tag == ';';
}

FIRST(enumerator)
{
	return look(s).tag == Id;
}
FIRST(enumerator_list)
{
	return IN_FIRST(enumerator);
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
FIRST(function_specifier)
{
	return look(s).tag == Inline;
}
FIRST(declaration_specifiers)
{
	return IN_FIRST(storage_class_specifier) || IN_FIRST(type_specifier) || IN_FIRST(type_qualifier) || IN_FIRST(function_specifier);
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

FIRST(parameter_declaration)
{
	return IN_FIRST(declaration_specifiers);
}

FIRST(parameter_list)
{
	return IN_FIRST(parameter_declaration);
}

FIRST(direct_declarator)
{
	switch (look(s).tag) {
		case Id:
		case '(':
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

FIRST(designation)
{
	return IN_FIRST(designator_list);
}


FIRST(initializer_list)
{
	return IN_FIRST(designation) || IN_FIRST(initializer);
}



FIRST(initializer)
{
	if (look(s).tag == '{') return 1;
	return IN_FIRST(assignment_expression);
}

FIRST(function_definition)
{
	return IN_FIRST(declaration_specifiers);
}



FIRST(init_declarator)
{
	return IN_FIRST(declarator);
}

FIRST(init_declarator_list)
{
	return IN_FIRST(init_declarator);
}

FIRST(declaration)
{
	return IN_FIRST(declaration_specifiers);
}

FIRST(external_declaration)
{
	return IN_FIRST(function_definition) || IN_FIRST(declaration);
}
