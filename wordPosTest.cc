#include <iostream>
#include <cassert>

using std::cerr;
using std::cin;
using std::cout;
using std::max;
using std::min;
using std::move;
using std::pair;
using std::string;

using pos_t = std::pair<size_t, size_t>;
using dim_t = std::pair<size_t, size_t>;

enum orientation_t : bool
{
    H = false,
    V = true
};

class WordPos
{
private:
    pos_t pos;
    orientation_t orient;

public:
    // Constructors:
    WordPos() = delete;

    WordPos(size_t x, size_t y, orientation_t _orient) : pos(x, y),
                                                         orient(_orient) {}
    WordPos(pos_t const &p, orientation_t const &o) : pos(p), orient(o) {}

    // Copy constructor:
    WordPos(const WordPos &w) = default; //: pos(w.pos), orient(w.orient) {}

    // Move Constructor:
    WordPos(WordPos &&w) = default; //: pos(move(w.pos)), orient(move(w.orient)) {}

    // Destructors:
    ~WordPos() = default;

    // Operators:
    WordPos &operator=(const WordPos &rhs) = default;
    WordPos &operator=(WordPos &&rhs) = default;
    auto operator<=>(const WordPos &) const = default;

    // Getters:
    // we return const reference not to allow to change these values
    pos_t getPos() const;

    orientation_t getOrient() const;
};

pos_t WordPos::getPos() const
{
    return pos;
}

orientation_t WordPos::getOrient() const
{
    return orient;
}

void printStartTest(string method, string cls)
{
    cout << "STARTING " << method << " TESTS FOR " + cls + " CLASS\n";
}
void printEndTest(string method = "")
{
    cout << method + " TESTS PASSED\n";
}

void operators_WordPos_TEST()
{
    printStartTest("OPERATORS", "WORD_POS");

    cout << "OPERATOR: =\n";
    WordPos pos1(1, 1, H);
    WordPos pos2 = pos1;

    assert(pos2.getPos().first == 1 && pos2.getPos().second == 1 &&
           pos2.getOrient() == H);

    pos2 = pos2;

    assert(pos2.getPos().first == 1 && pos2.getPos().second == 1 &&
           pos2.getOrient() == H);

    pos2 = WordPos(2, 2, H);

    assert(pos2.getPos().first == 2 && pos2.getPos().second == 2 &&
           pos2.getOrient() == H);

    cout << "PASSED\n";

    cout << "OPERATOR: == && !=\n";

    pos1 = pos2;

    assert(pos2 == pos2);
    assert(pos2 == WordPos(2, 2, H));
    assert(pos1 == pos2);

    pos1 = WordPos(3, 3, H);

    assert(!(pos2 == WordPos(1, 2, H)));
    assert(!(pos2 == WordPos(1, 2, V)));
    assert(!(pos2 == WordPos(1, 1, H)));
    assert(!(pos2 == pos1));
    assert(pos2 != pos1);
    assert(pos2 != WordPos(3, 3, H));

    cout << "PASSED\n";

    cout << "OPERATOR: <=>\n";

    // H = false, V = true
    pos1 = WordPos(1, 2, H);
    pos2 = WordPos(2, 1, H);
    WordPos posH(1, 1, H);
    WordPos posV(1, 1, V);

    assert(pos1 < pos2);
    assert(pos1 > posH);
    assert(pos1 > posV);
    assert(posH < posV);
    assert(pos1 <= pos1);
    assert(pos1 <= pos2);
    assert(pos1 >= posV);
    assert((pos1 <=> pos1) == 0);
    assert((pos1 <=> pos2) < 0);
    assert((posV <=> posH) > 0);

    cout << "PASSED\n";

    printEndTest("OPERATORS");
    cout << "\n";
}

int main()
{
    operators_WordPos_TEST();
    return 0;
}