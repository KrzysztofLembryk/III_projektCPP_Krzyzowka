#include <cassert>
#include <vector>

#include "crosswords.h"

using std::cout;
using std::move;
using std::string;
using std::vector;

void printStartTest(string method, string cls)
{
    cout << "->starting " << method << " tests for " + cls + " class\n";
}
void printEndTest(string method = "")
{
    cout << method + " TESTS PASSED\n";
    cout << "--------------------\n";
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

    printEndTest();
}

void methods_Word_TEST()
{   
    printStartTest("METHODS", "WORD");

    Word w1(1, 1, H, "ALA");
    Word w2(1, 1, V, "SMOK");

    assert(w1.get_end_position() == pos_t(3, 1));
    assert(w2.get_end_position() == pos_t(1, 4));

    Word wExtreme((size_t)(-1) - 2, 2, H, "EXTREME_WORD");
    string res;

    for(size_t i = 0; i < wExtreme.length(); i++)
        res.push_back(wExtreme.at(i));

    cout << "res: " << res << "\n";
    assert(res == "EXT");

    printEndTest();
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

void printRA(RectArea const &ra)
{
    cout << "topLeft: (" << ra.get_left_top().first << ", " << 
        ra.get_left_top().second << "), ";
    cout << "btmRight: (" << ra.get_right_bottom().first << ", " <<
        ra.get_right_bottom().second << ")\n";
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

void RA_embrace_TEST()
{
    printStartTest("EMBRACE", "RECT_AREA");

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

    RectArea ra5(pos_t(2, 3), pos_t(10, 7));
    ra5.embrace(pos_t(4, 5));

    assert(ra5.size() == dim_t(9, 5));
    
    ra5.embrace(pos_t(1, 2));
    assert(ra5.size() == dim_t(10, 6));

    RectArea ra4(pos_t(1, 1), pos_t(4, 4));
    
    ra4.embrace(pos_t(2, 0));
    assert(ra4.size() == dim_t(4, 5));

    RectArea ra3(pos_t(1, 1), pos_t(4, 4));

    ra3.embrace(pos_t(5, 0));
    assert(ra3.size() == dim_t(5, 5));

    RectArea ra2(pos_t(2, 2), pos_t(5, 5));

    ra2.embrace(pos_t(0, 7));
    assert(ra2.size() == dim_t(6, 6));

    printEndTest();
    
}

void RA_operator_TEST()
{
    printStartTest("OPERATOR *", "RECT_AREA");

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
        DEFAULT_EMPTY_RECT_AREA));

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
    
    printEndTest();
}

void RA_other_TESTS()
{   
    printStartTest("OTHER", "RECT_AREA");

    RectArea ra1(pos_t(2, 2), pos_t(5, 5));
    RectArea ra1_copy(ra1);
    RectArea ra1_move(RectArea(pos_t(2, 2), pos_t(5, 5)));
    RectArea wrong_ra(pos_t(4, 4), pos_t(1,1));

    assert(ra_equals(ra1, ra1_copy) && ra1.size() == ra1_copy.size());
    assert(ra_equals(ra1, ra1_move));
    assert(ra1.size() == dim_t(4, 4));
    assert(wrong_ra.size() == dim_t(0, 0));

    RectArea ra2(pos_t(1, 1), pos_t(2, 2));

    ra2.set_left_top(pos_t(0, 0));
    assert(ra2.size() == dim_t(3, 3));

    ra2.set_right_bottom(pos_t(2, 1));
    assert(ra2.size() == dim_t(3, 2));

    ra2.set_left_top(pos_t(1, 1));
    assert(ra2.size() == dim_t(2, 1));

    printEndTest();
}

int main()
{

    // move_copy_constructor_Word_TEST();
    operators_Word_TEST();
    methods_Word_TEST();
    RA_operator_TEST();
    RA_embrace_TEST();
    RA_other_TESTS();

    return 0;
}