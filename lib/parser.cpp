#include "parser.hpp"
#include "precedence_table.hpp"
#include "llvm/ADT/STLExtras.h"
#include <exception>

namespace HoneyBadger
{
/*
    void Parser::parse(std::vector<Token> tokens) {
        Parser p(tokens);
        p.parse();
    }
    */

Parser::Parser(std::vector<Token> tokens)
{
    this->tokens = tokens;
}

AST::FunctionTable *Parser::parse()
{
    AST::FunctionTable* functions = new AST::FunctionTable();
    while(current_token().type != Token::Type::END_OF_FILE) {
        functions->add_function(parse_function());
    }
    return functions;
}

AST::Node *Parser::parse_primary()
{
    switch (current_token().type)
    {
    default:
        throw std::runtime_error("What is \"" + current_token().value + "\" ?");
    case Token::Type::IDENTIFIER:
        return parse_identifier_expression();
    case Token::Type::FUNCTION_DEFINITION:
        return parse_function();
    case Token::Type::NUMBER:
        return parse_number_expression();
    case Token::Type::OPENING_PARENTHESIS:
        return parse_parenthesis();
    }
}

AST::Node *Parser::parse_binary_op_right_side(int expression_precedence, AST::Node *left)
{
    while (1)
    {
        int token_precedence = current_token_precedence();
        if (token_precedence < expression_precedence)
            return left;

        Token bin_op = current_token();
        next_token(); // 'eat' the bin_op

        // parse expression after operator
        auto right = parse_primary();

        int next_precedence = current_token_precedence();
        if (token_precedence < next_precedence)
        {
            right = parse_binary_op_right_side(token_precedence + 1, right);
        }
        left = new AST::BinaryExpr(bin_op.value.at(0), left, right);
    }
}

AST::FunctionSignature *Parser::parse_function_signature()
{
    // we basically just read whatever there is as function name and then we test if its actually the function name (e.g. a identifier)
    std::string function_name = current_token().value;
    expect(Token::Type::IDENTIFIER);

    expect("(");

    std::vector<std::string> args;
    while (current_token().type == Token::Type::IDENTIFIER)
    {
        args.push_back(current_token().value);
        next_token(); // eat the argument name
        accept(",");  // maybe comma seperated arguments?
    }
    expect(")");
    expect("do");

    return new AST::FunctionSignature(function_name, args);
}

AST::Function *Parser::parse_function()
{
    expect("def");
    auto signature = parse_function_signature();
    auto body = parse_expression();
    expect("end");
    return new AST::Function(signature, body);
}

AST::Node *Parser::parse_expression()
{
    auto left = parse_primary();
    if (current_token().type == Token::Type::END_OF_FILE)
        return left;
    return parse_binary_op_right_side(0, left);
}


// we allow expressions outside of functions. to make this possible we just wrap the expression in a function with no arguments
//AST::Function *Parser::parse_top_level_expression()
//{
//    auto body = parse_expression();
//    auto signature = new AST::FunctionSignature("__TOP_LEVEL__", std::vector<std::string>());
//    return new AST::Function(signature, body);
//}

// ::= number
AST::Node *Parser::parse_number_expression()
{
    auto r = new AST::Number(std::stod(current_token().value));
    next_token();
    return r;
}

// paren ::= '(' expression ')'
AST::Node *Parser::parse_parenthesis()
{
    expect("(");
    auto expr = parse_expression();
    expect(")");
    return expr;
}

// ::= identifier
// ::= identifier '(' expression ')'
AST::Node *Parser::parse_identifier_expression()
{
    std::string id = current_token().value;
    next_token();

    // variable
    if (!accept("("))
        return new AST::Variable(id);

    // otherwise Function Call
    std::vector<AST::Node *> args;
    if (!accept(")"))
    { // Function with Parameters
        while (1)
        {
            auto arg = parse_expression();
            args.push_back(arg);

            // No more Parameters
            if (accept(")"))
                break;

            expect(","); // there are more parameters so i want them to be split!
        }
    }
    return new AST::FunctionCall(id, args);
}

int Parser::current_token_precedence()
{
    // TODO: Fix this dont just assume at(0) to be valid
    return Operators::s_get_precedence(current_token().value.at(0));
}

Token Parser::current_token()
{
    return this->tokens.at(this->current_index);
}

void Parser::next_token()
{
    this->current_index++;
}

bool Parser::expect(int thing)
{
    if (current_token().type == thing)
    {
        next_token();
        return true;
    }
    else
    {
        auto location = current_token().location;
        throw std::runtime_error("Unexpected Token '" + current_token().value + "' at " + location.to_string());
    }
    return false;
}

bool Parser::expect(std::string thing)
{
    if (current_token().value == thing)
    {
        next_token();
        return true;
    }
    else
    {
        auto location = current_token().location;
        throw std::runtime_error("Unexpected Token '" + current_token().value + "' at " + location.to_string() + " should have been: '"+thing+"'");
    }
    return false;
}

bool Parser::accept(std::string thing)
{
    if (current_token().value != thing)
        return false;
    next_token();
    return true;
}

} // namespace HoneyBadger
