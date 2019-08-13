
#include<string>
#include<map>
#include<vector>
#include<memory>

namespace HoneyBadger
{
class Operators
{
private:
    std::vector<char> operators;
    std::map<char, int> precedence;

    Operators() {}

public:
    Operators(Operators const &) = delete;
    Operators &operator=(Operators const &) = delete;

    static std::shared_ptr<Operators> get_instance();
    int get_precedence(char thing);
    static int s_get_precedence(char op);
};

} // namespace HoneyBadger