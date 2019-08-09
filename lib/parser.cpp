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

std::unique_ptr<AST::Function> Parser::parse()
{
    return parse_top_level_expression();
}

std::unique_ptr<AST::Node> Parser::parse_primary()
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

std::unique_ptr<AST::Node> Parser::parse_binary_op_right_side(int expression_precedence, std::unique_ptr<AST::Node> left)
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
            right = parse_binary_op_right_side(token_precedence + 1, std::move(right));
        }
        left = llvm::make_unique<AST::BinaryExpr>(bin_op.value.at(0), std::move(left), std::move(right));
    }
}

std::unique_ptr<AST::FunctionSignature> Parser::parse_function_signature()
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

    return llvm::make_unique<AST::FunctionSignature>(function_name, std::move(args));
}

std::unique_ptr<AST::Function> Parser::parse_function()
{
    expect("def");
    auto signature = parse_function_signature();
    auto body = parse_expression();
    expect("end");
    return llvm::make_unique<AST::Function>(std::move(signature), std::move(body));
}

std::unique_ptr<AST::Node> Parser::parse_expression()
{
    auto left = parse_primary();
    if(current_token().type == Token::Type::END_OF_FILE)
        return left;
    return parse_binary_op_right_side(0, std::move(left));
}

// we allow expressions outside of functions. to make this possible we just wrap the expression in a function with no arguments
std::unique_ptr<AST::Function> Parser::parse_top_level_expression()
{
    auto body = parse_expression();
    auto signature = llvm::make_unique<AST::FunctionSignature>("__ANON__", std::vector<std::string>());
    return llvm::make_unique<AST::Function>(std::move(signature), std::move(body));
}

// ::= number
std::unique_ptr<AST::Node> Parser::parse_number_expression()
{
    auto r = llvm::make_unique<AST::Number>(std::stod(current_token().value));
    next_token();
    return std::move(r);
}

// paren ::= '(' expression ')'
std::unique_ptr<AST::Node> Parser::parse_parenthesis()
{
    expect("(");
    auto expr = parse_expression();
    expect(")");
    return expr;
}

// ::= identifier
// ::= identifier '(' expression ')'
std::unique_ptr<AST::Node> Parser::parse_identifier_expression()
{
    std::string id = current_token().value;
    next_token();

    // variable
    if (!accept("("))
        return llvm::make_unique<AST::Variable>(id);

    // otherwise Function Call
    std::vector<std::unique_ptr<AST::Node>> args;
    if (!accept(")"))
    { // Function with Parameters
        while (1)
        {
            auto arg = parse_expression();
            args.push_back(std::move(arg));

            // No more Parameters
            if (accept(")"))
                break;

            expect(","); // there are more parameters so i want them to be split!
        }
    }
    return llvm::make_unique<AST::FunctionCall>(id, std::move(args));
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
        throw std::runtime_error("Unexpected Token " + current_token().value + " at " + location.to_string());
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
        throw std::runtime_error("Unexpected Token " + current_token().value + " at " + location.to_string());
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
