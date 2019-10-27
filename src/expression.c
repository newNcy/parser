#include "parser.h"


/* 先搞表达式 */
P( primary_expression )
{
    switch(look(s).tag) {
    case Id:
    case ConstChar:
    case ConstInt:
    case ConstFloat:
    case ConstLong:
    case ConstDouble:
    case ConstStr:
        next(s);
        break;
    case '(':
        next(s);
        NODE( expression );
        X(')');
        break;
    default:
        printf("错误\n");
        return NULL;
    }

}

/*
 * assignment_expression    ->  conditional_expression
 *                          |   unary_expression assignment_operaNODEr assignment_expression
 */
P(assignment_expression)
{
    if (look(s).tag == '(') {
        next(s);
        Node * first = newNode('(');
        if (IN_FIRST(type_name)) { //cast_expression

        }else { //unary_expression

        }
    }
}


P_(argument_expression_list)
{
    if (look(s).tag == ',') {
        Node * expr = newNode(ARGUMENT_EXPRESSION_LIST);
        addChild(expr, first);
        addChild(expr, newNode(','));
        addChild(expr, NODE(assignment_expression));
        return expression_(expr, s);
    }else {
        return first;
    }
}

P(argument_expression_list)
{
    return argument_expression_list_(assignment_expression(s), s);
}


Node* postfix_expression_(Node * post_expr_, Source * s)
{
    Node * post_expr = newNode(POSTFIX_EXPRESSION);
    addChild(post_expr, post_expr_);

    switch(look(s).tag) {
    case '[':
        next(s);
        addChild(post_expr, newNode('['));
        addChild(post_expr, NODE(expression));
        X(']');
        addChild(post_expr, newNode(']'));
        return postfix_expression_(post_expr, s);
    case '(':
        next(s);
        addChild(post_expr, newNode('('));
        if (look(s).tag != ')') {
            addChild(post_expr, NODE(argument_expression_list));
        }
        X(')');
        addChild(post_expr, newNode(')'));
        return postfix_expression_(post_expr, s);
    case '.':
    case Pto:
        addChild(post_expr, newNode(look(s).tag));
        next(s);
        if (look(s).tag != Id) {
            printf("error\n");
        }
        X(Id);
        break;
    case SPlus:
    case SSub:
        next(s);
    default:
        return post_expr_;
    }
}
P( postfix_expression) 
{
    return postfix_expression_(NODE(primary_expression), s);
}


P_(expression) 
{
    if (look(s).tag == ',') {
        Node * expr = newNode(EXPRESSION);
        addChild(expr, first);
        addChild(expr, newNode(','));
        addChild(expr, NODE(assignment_expression));
        return expression_(expr, s);
    }else {
        return first;
    }
}

P(expression)
{
    return expression_(NODE(assignment_expression), s);
}

