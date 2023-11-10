#include <iostream>
// #include <utility>

using std::cerr;
using std::cin;
using std::cout;
using std::pair;
using std::string;

using pos_t = pair<size_t, size_t>;
using dim_t = pair<size_t, size_t>;

enum orientation_t : bool
{
    H = true,
    V = false
};

namespace
{

    class WordPos
    {
    private:
        pos_t pos;
        orientation_t orient;

    public:
        // Constructors:
        WordPos() = delete;
        WordPos(size_t x, size_t y, orientation_t _orient) : pos(x, y), orient(_orient) {}
        WordPos(pos_t p, orientation_t o) : pos(p), orient(o) {}

        // Destructors:
        ~WordPos() = default;
    };

}

class Word
{
private:
    WordPos pos;
    string word;

public:
    Word() = delete;
    Word(size_t x, size_t y, orientation_t orient, string _word) : pos(x, y, orient), word(_word) {}
};

int main()
{
    cout << "ciul\n";
    return 0;
}