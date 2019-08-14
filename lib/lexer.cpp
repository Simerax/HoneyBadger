#include "lexer.hpp"
#include <regex>

#include"precedence_table.hpp"
#include"operators.hpp"

namespace HoneyBadger
{
std::vector<Token> Lexer::lex(string raw_input)
{
    std::vector<Token> tokens;
    this->input = raw_input;

    // in case the input has no EOL e.g newline we just add one for safety
    // the worst that could happen is that we parse two EOL...Oh well
    input += "\n";

    string word = "";
    uint line = 1;
    uint column = 1;
    uint word_line = line;
    uint word_column = column;
    bool inside_comment = false;
    bool inside_string_literal = false;

    for (index = 0; index < input.length(); ++index)
    {
        char cc = input.at(index);
        column++;

        // comments
        if(!inside_comment && cc == '/' && peek('/'))
        {
            inside_comment = true;

            // In case we get into a comment we still need to make sure to add the previous word to our tokens
            // if the comment was separated by a space then this here wont apply since the word is already in the list of tokens
            // however if we have something like this: a + b//comment
            // then our word is "b" and we go inside_comment because the next two chars are "//".
            // this means that our current word ("b") won't be stored
            // for that reason we just add it here
            if(word != "")
            {
                tokens.push_back(Token(word, Location(word_line, word_column), get_type(word)));
                word = "";
                word_line = line;
                word_column = column;
            }
        }


        if(cc == '"')
        {
            if(!inside_string_literal)
                inside_string_literal = true;
            else
            {
                inside_string_literal = false;
                tokens.push_back(Token(word, Location(word_line, word_column), Token::Type::STRING_LITERAL));
                word = "";
            }
        }
        else if (!inside_string_literal && !inside_comment && (is_delimiter(cc) || is_newline(cc) || is_operator(cc) || index + 1 == input.length()))
        {
            if (word != "")
                tokens.push_back(Token(word, Location(word_line, word_column), get_type(word)));

            Token::Type t = get_type(string(1, cc));

            // in case the delimiter was a special char we add that one to the tokens
            if (is_operator(cc) || t == Token::Type::SEPARATOR || t == Token::Type::OPENING_PARENTHESIS || t == Token::Type::CLOSING_PARENTHESIS)
            {
                tokens.push_back(Token(string(1, cc), Location(line, column - 1), t));
            }

            word = "";
            word_line = line;
            word_column = column;
        }
        else
            word += cc;

        if (is_newline(cc))
        {
            line++;
            column = 1;
            word_line = line;
            word_column = column;
            inside_comment = false;
            if(!inside_string_literal) // A string literal could be multi line
                word = "";
        }
    }
    tokens.push_back(Token("", Location(line, column), Token::Type::END_OF_FILE));
    return tokens;
}

/// peeks the next char and checks if its the same as 'expected'
bool Lexer::peek(char expected) {
    if(input.length() > index + 1)
        if(input[index + 1] == expected)
            return true;
    return false;
}

Token::Type Lexer::get_type(string word)
{
    if (word == ",")
        return Token::Type::SEPARATOR;
    if (word == "if")
        return Token::Type::IF;
    if (word == "def")
        return Token::Type::FUNCTION_DEFINITION;
    if (word == "let")
        return Token::Type::VARIABLE_DEFINITION;
    if (word == "=")
        return Token::Type::ASSIGN;
    if (word == "(")
        return Token::Type::OPENING_PARENTHESIS;
    if (word == ")")
        return Token::Type::CLOSING_PARENTHESIS;
    if (word == "do" || word == "end")
        return Token::Type::KEYWORD;
    if(word.length() == 1 && is_operator(word.at(0)))
        return Token::Type::BIN_OP;
    if (looks_like_number(word))
        return Token::Type::NUMBER;

    // if nothing matches then we guess that it will be an identifier
    return Token::Type::IDENTIFIER;
}

bool Lexer::looks_like_number(string str)
{
    std::regex rx("(^[+-]?\\d+$)|(^([+-]?)(?=\\d|\\.\\d)\\d*(\\.\\d*)?([Ee]([+-]?\\d+))?$)$");
    if (std::regex_match(str, rx))
        return true;
    return false;
}

bool Lexer::is_delimiter(char c)
{
    return c == ' ' || c == '(' || c == ')' || c == ',';
}

bool Lexer::is_newline(char c)
{
    return c == '\n';
}

bool Lexer::is_operator(char op) {
    // if its an operator it will have a precedence higher then -1
    return -1 < Operators::get_instance()->get_precedence(op) ;
}
} // namespace HoneyBadger
