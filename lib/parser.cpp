#include "parser.hpp"
#include "operators.hpp"
#include "llvm/ADT/STLExtras.h"
#include <exception>

namespace HoneyBadger
{

Ref<AST::FunctionTable> Parser::parse(std::vector<Token> tokens)
{
    this->tokens = tokens;
    Ref<AST::FunctionTable> functions = std::make_shared<AST::FunctionTable>();
    while (current_token().type != Token::Type::END_OF_FILE)
        functions->add_function(parse_function());
    return functions;
}

Ref<AST::Node> Parser::parse_primary()
{
    switch (current_token().type)
    {
    default:
        throw std::runtime_error("Unexpected Token '" + current_token().value + "' at " + current_token().location.to_string());
    case Token::Type::IDENTIFIER:
        return parse_identifier_expression();
    case Token::Type::NUMBER:
        return parse_number_expression();
    case Token::Type::IF:
        return parse_if_expression();
    case Token::Type::OPENING_PARENTHESIS:
        return parse_parenthesis();
    case Token::Type::VARIABLE_DEFINITION:
        return parse_variable_definition();
    }
}

Ref<AST::Node> Parser::parse_variable_definition()
{
    expect("let");
    auto variable = std::make_shared<AST::Variable>(current_token().value);
    next_token(); // eat the variable name
    expect("=");
    auto value = parse_expression();
    return std::make_shared<AST::VariableDefinition>(variable, value);
}

Ref<AST::Node> Parser::parse_if_expression()
{
    expect("if");
    Ref<AST::Node> condition = parse_expression();
    expect("do");
    inside_if_then_block = true; // the parse_block() should know that this block ends with "else" instead of "end"
    Ref<AST::Block> then = parse_block();
    inside_if_then_block = false;
    expect("else"); // every statement has to return a value so we cant allow if without an else
    Ref<AST::Block> _else = parse_block();
    expect("end");

    return std::make_shared<AST::If>(condition, then, _else);
}

Ref<AST::Block> Parser::parse_block()
{
    auto block = std::make_shared<AST::Block>();
    while ((!inside_if_then_block && current_token().value != "end") || (inside_if_then_block && current_token().value != "else"))
    {
        block->add_expression(parse_expression());
    }
    return block;
}

Ref<AST::Node> Parser::parse_binary_op_right_side(int expression_precedence, Ref<AST::Node> left)
{
    while (1)
    {
        int token_precedence = current_token_precedence();
        if (token_precedence < expression_precedence)
            return left;
        
        Token bin_op = current_token();
        next_token(); // 'eat' the bin_op

        if(bin_op.value == "=")
            throw std::runtime_error("You cannot re-assign a variable.");

        // parse expression after operator
        auto right = parse_primary();

        int next_precedence = current_token_precedence();
        if (token_precedence < next_precedence)
        {
            right = parse_binary_op_right_side(token_precedence + 1, right);
        }
        left = std::make_shared<AST::BinaryExpr>(bin_op.value.at(0), left, right);
    }
}

Ref<AST::FunctionSignature> Parser::parse_function_signature()
{
    // we basically just read whatever there is as function name and then we test if its actually the function name (e.g. a identifier)
    string function_name = current_token().value;
    expect(Token::Type::IDENTIFIER);

    expect("(");

    std::vector<AST::Argument> args;
    while (current_token().type == Token::Type::IDENTIFIER)
    {
        auto argument_name = current_token().value;
        next_token(); // eat the argument name
        expect(":");
        auto argument_type = current_token();
        next_token(); // eat the argument type
        args.push_back(AST::Argument::make_argument(argument_name, argument_type));
        accept(",");  // maybe comma seperated arguments?
    }
    expect(")");
    expect(":");
    Type return_type = Type::convert_token_type_to_type(current_token().type);
    next_token(); // eat the return type token
    expect("do");

    return std::make_shared<AST::FunctionSignature>(function_name, args, return_type);
}

Ref<AST::Function> Parser::parse_function()
{
    auto loc = current_token().location;
    expect("def");
    auto signature = parse_function_signature();
    auto body = parse_block();
    expect("end");
    auto fun = std::make_shared<AST::Function>(signature, body);
    fun->location = loc;
    return fun;
}

Ref<AST::Node> Parser::parse_expression()
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
//    auto signature = new AST::FunctionSignature("__TOP_LEVEL__", std::vector<string>());
//    return new AST::Function(signature, body);
//}

// ::= number
Ref<AST::Node> Parser::parse_number_expression()
{
    auto r = std::make_shared<AST::Number>(std::stod(current_token().value));
    next_token();
    return r;
}

// paren ::= '(' expression ')'
Ref<AST::Node> Parser::parse_parenthesis()
{
    expect("(");
    auto expr = parse_expression();
    expect(")");
    return expr;
}

// ::= identifier
// ::= identifier '(' expression ')'
Ref<AST::Node> Parser::parse_identifier_expression()
{
    string id = current_token().value;
    next_token();

    // variable
    if (!accept("("))
        return std::make_shared<AST::Variable>(id);

    // otherwise Function Call
    std::vector<Ref<AST::Node>> args;
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
    return std::make_shared<AST::FunctionCall>(id, args);
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

bool Parser::expect(string thing)
{
    if (current_token().value == thing)
    {
        next_token();
        return true;
    }
    else
    {
        auto location = current_token().location;
        throw std::runtime_error("Unexpected Token '" + current_token().value + "' at " + location.to_string() + " should have been: '" + thing + "'");
    }
    return false;
}

bool Parser::accept(string thing)
{
    if (current_token().value != thing)
        return false;
    next_token();
    return true;
}

} // namespace HoneyBadger
