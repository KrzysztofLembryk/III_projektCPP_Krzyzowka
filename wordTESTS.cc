#include <cassert>
#include "crosswords.h"

using std::cout;
using std::move;
using std::string;

void printStartTest(string method, string cls)
{
    cout << "STARTING " << method << " TESTS FOR " + cls + " CLASS\n";
}
void printEndTest(string method = "")
{
    cout << method + " TESTS PASSED\n";
}

// void move_copy_constructor_Word_TEST()
// {
//         printStartTest("MOVE_COPY", "WORD");

//         Word w1(1, 1, H, "Krakow");
//         Word w2(move(w1));

//         assert(w2.get_word().compare("Krakow") == 0);
//         assert(w1.get_word().compare("") == 0);

//         Word wx(Word(1, 1, H, "Warszawa"));

//         assert(wx.get_word().compare("Warszawa") == 0);

//         Word w3(w2);

//         assert(w3.get_word().compare(w2.get_word()) == 0);

//         printEndTest("MOVE_COPY");
//         cout << "\n";
// }

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

void operators_Word_TEST()
{
    printStartTest("ALL OPERATORS", "WORD");

    Word w1(1, 1, H, "w1");
    Word w2 = w1;
    Word w3 = Word(2, 1, H, "w3");
    Word w4 = Word(1, 1, V, "w4");
    Word w5 = Word(2, 1, V, "w5");

    // H = false, V = true
    assert(w1 == w2);
    assert(w3 > w1);
    assert(w3 >= w2);
    assert(w3 < w5);
    assert(w3 <= w5);
    assert(w4 > w1);
    assert(w4 >= w1);
    assert((w1 <=> w2) == 0);
    assert((w3 <=> w1) > 0);
    assert((w3 <=> w5) < 0);

    printEndTest("OPERATORS");
}

int main()
{

    // move_copy_constructor_Word_TEST();
    operators_WordPos_TEST();
    operators_Word_TEST();

    return 0;
}