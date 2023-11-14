#include <cassert>
#include "crosswords.h"
#include <vector>

using std::cout;
using std::move;
using std::string;
using std::vector;

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
    assert(w1 >= Word(0, 1 , V,"xd"));
    assert(w1 <= Word(1, 2, H, "dx"));
    assert(w1 == Word(1, 1, H, "dsd"));

    printEndTest("OPERATORS");
}

bool ra_equals(const RectArea &ra, const RectArea &ra2)
{
    return (ra.get_left_top() == ra2.get_left_top()) && 
            (ra.get_right_bottom() == ra2.get_right_bottom());
}

RectArea getRA(pos_t topLeft, pos_t btmRight)
{
    return RectArea(topLeft, btmRight);
}

void RectArea_operator_TEST()
{
    RectArea baseArea(pos_t(2, 2), pos_t(5, 5));
    RectArea lineArea(pos_t(2,2), pos_t(2,5));

    assert(ra_equals(baseArea * getRA(pos_t(3, 1), pos_t(6,3)), 
        getRA(pos_t(3, 2), pos_t(5,3))));

    assert(ra_equals(baseArea * getRA(pos_t(5, 1), pos_t(6,3)), 
        getRA(pos_t(5, 2), pos_t(5,3))));

    assert(ra_equals(baseArea * getRA(pos_t(5, 2), pos_t(6,3)), 
        getRA(pos_t(5, 2), pos_t(5,3))));

    assert(ra_equals(baseArea * getRA(pos_t(3, 1), pos_t(4,3)), 
        getRA(pos_t(3, 2), pos_t(4,3))));

    assert(ra_equals(baseArea * getRA(pos_t(3, 3), pos_t(4,4)), 
        getRA(pos_t(3, 3), pos_t(4,4))));

    assert(ra_equals(baseArea * getRA(pos_t(2, 2), pos_t(3,3)), 
        getRA(pos_t(2, 2), pos_t(3,3))));
    
    assert(ra_equals(baseArea * getRA(pos_t(6, 2), pos_t(7,5)), 
        EMPTY_AREA));

    assert(ra_equals(baseArea * getRA(pos_t(3, 4), pos_t(4,6)), 
        getRA(pos_t(3, 4), pos_t(4,5))));

    assert(ra_equals(baseArea * getRA(pos_t(5, 1), pos_t(6,2)), 
        getRA(pos_t(5, 2), pos_t(5,2))));

    assert(ra_equals(baseArea * getRA(pos_t(1, 3), pos_t(3,3)), 
        getRA(pos_t(2, 3), pos_t(3,3))));
    
    assert(ra_equals(baseArea * getRA(pos_t(1, 3), pos_t(6,3)), 
        getRA(pos_t(2, 3), pos_t(5,3))));

    assert(ra_equals(baseArea * getRA(pos_t(3, 3), pos_t(3,3)), 
        getRA(pos_t(3, 3), pos_t(3,3))));
    
    assert(ra_equals(baseArea * getRA(pos_t(3, 1), pos_t(3,6)), 
        getRA(pos_t(3, 2), pos_t(3,5))));
    
    assert(ra_equals(lineArea * getRA(pos_t(1, 3), pos_t(3,3)), 
        getRA(pos_t(2, 3), pos_t(2, 3))));

}

void setBase(RectArea &baseArea)
{
    baseArea.set_left_top(pos_t(2, 2));
    baseArea.set_right_bottom(pos_t(5, 5));
}

void set_up_vectors(vector<pos_t> &topLeftRes, vector<pos_t> &btmRightRes)
{
    topLeftRes.insert(topLeftRes.end(), 2, pos_t (2, 1));
    topLeftRes.insert(topLeftRes.end(), 3, pos_t (2, 2));
    topLeftRes.insert(topLeftRes.end(), 2, pos_t (1, 2));

    btmRightRes.insert(btmRightRes.end(), 2, pos_t(5, 5));
    btmRightRes.insert(btmRightRes.end(), 2, pos_t(6, 5));
    btmRightRes.push_back(pos_t(7, 7));
    btmRightRes.insert(btmRightRes.end(), 2, pos_t(5, 6));
    btmRightRes.push_back(pos_t(5, 5));
}

void RectArea_embrace_TEST()
{
    RectArea baseArea(pos_t(2, 2), pos_t(5, 5));
    RectArea _baseArea(pos_t(2, 2), pos_t(5, 5));
    vector<pos_t> pos_inside{pos_t(2, 2), pos_t(2, 3), pos_t(3, 2), pos_t(4, 4),
        pos_t(5,2)};

    for(pos_t &pos : pos_inside)
    {
        setBase(baseArea);
        baseArea.embrace(pos);
        assert(ra_equals(baseArea, _baseArea));
    }
    
    vector<pos_t> posToEmbraceVec {pos_t(1, 1), pos_t(3, 1), pos_t(6, 1), 
        pos_t(6, 3), pos_t(7, 7), pos_t(3, 6), pos_t(1, 6), pos_t(1, 3)};
    vector<pos_t> topLeftRes {pos_t(1, 1)};
    vector<pos_t> btmRightRes;

    set_up_vectors(topLeftRes, btmRightRes);
    size_t n = topLeftRes.size();

    for(size_t i = 0; i < n; i++)
    {
        setBase(baseArea);
        baseArea.embrace(posToEmbraceVec[i]);
        assert(ra_equals(baseArea, getRA(topLeftRes[i], btmRightRes[i])));
    }

}


int main()
{

    // move_copy_constructor_Word_TEST();
    operators_WordPos_TEST();
    operators_Word_TEST();
    RectArea_operator_TEST();
    RectArea_embrace_TEST();

    return 0;
}