#pragma once


namespace HoneyBadger
{
    struct Type 
    {
        enum TypeName
        {
            F64,
            String,
        };
        bool list = false;
        TypeName type;

        Type(TypeName name, bool is_list = false) : type(name), list(is_list) {}


        static Type::TypeName convert_token_type_to_type(Token::Type token_type)
        {
            switch (token_type)
            {
                case Token::Type::TYPE_FLOAT_64:
                    return Type::F64;
                case Token::Type::TYPE_STRING:
                    return Type::String;
                default:
                    throw std::runtime_error("Token Type " + std::to_string(token_type) + "cant be converted to actual type");
            }
        }
    };
}