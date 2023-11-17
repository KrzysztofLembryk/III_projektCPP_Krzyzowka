// Franciszek Witt
// Wojciech Malecha

#include <iostream>
#include <cassert>
#include <type_traits>
#include <functional>
#include <limits>
#include <random>

#include "crosswords.h"

using namespace std;

void proste1() {
    using orientation_t::H;
    using orientation_t::V;

    size_t smax = numeric_limits<size_t>::max();

    Word w1 = Word(0, 0, H, "");
    assert(w1.length() == 1);
    assert(w1.at(0) == DEFAULT_CHAR);
    assert(w1.at(1) == DEFAULT_CHAR);
    assert(w1.get_end_position() == pos_t(0, 0));

    Crossword c1 = Crossword(w1, {});
    Word w2 = Word(0, 0, V, "");
    assert(c1.insert_word(w2));

    Word w3 = Word(1, 0, H, "");
    Word w4 = Word(1, 0, V, "");
    Word w5 = Word(0, 1, H, "");
    Word w6 = Word(0, 1, V, "");
    Word w7 = Word(1, 1, H, "");
    Word w8 = Word(1, 1, V, "");

    assert(!c1.insert_word(w3));
    assert(!c1.insert_word(w4));
    assert(!c1.insert_word(w5));
    assert(!c1.insert_word(w6));
    assert(!c1.insert_word(w7));
    assert(!c1.insert_word(w8));

    Word w9 = Word(smax, smax, H, "A");
    Crossword c2 = Crossword(w9, {});

    Word w10 = Word(smax, smax - 1, H, "A");
    Word w11 = Word(smax - 1, smax, H, "A");
    Word w12 = Word(smax - 1, smax - 1, H, "A");

    assert(!c2.insert_word(w10));
    assert(!c2.insert_word(w11));
    assert(!c2.insert_word(w12));

    Crossword c3 = Crossword(w12, {});
    assert(!c3.insert_word(w9));
    assert(!c3.insert_word(w10));
    assert(!c3.insert_word(w11));

    Crossword c4 = std::move(c3);
    Crossword c5 = std::move(c3);
    assert(c5.size() == dim_t(0, 0));

    Word w13 = Word(10, 10, H, "A");
    Crossword c6 = Crossword(w13, {});

    Word w14 = Word(11, 10, H, "A");
    Word w15 = Word(10, 11, H, "A");
    Word w16 = Word(11, 11, H, "A");

    Word w17 = Word(9, 10, H, "A");
    Word w18 = Word(10, 9, H, "A");
    Word w19 = Word(9, 9, H, "A");

    assert(!c6.insert_word(w14));
    assert(!c6.insert_word(w15));
    assert(!c6.insert_word(w16));
    assert(!c6.insert_word(w17));
    assert(!c6.insert_word(w18));
    assert(!c6.insert_word(w19));

    Word w20 = Word(5, 11, H, "AAAAAAAAAAA");
    Word w21 = Word(5, 11, V, "AAAAAAAAAAA");
    assert(!c6.insert_word(w20));
    assert(c6.insert_word(w21));

    Crossword c7 = Crossword(w20, {});
    Word w22 = Word(9, 11, H, "AAAAAAAAAAA");
    assert(!c7.insert_word(w22));

}

template<class T>
void podstawowe_statyczne_zapewnienia() {
    static_assert(is_copy_constructible_v<T>, "Wymagany konstruktor kopiowania");
    static_assert(is_move_constructible_v<T>, "Wymagany konstruktor przenoszacy");
    static_assert(is_copy_assignable_v<T>, "Wymagany kopiujacy konstruktor przypisania");
    static_assert(is_move_assignable_v<T>, "Wymagany przenoszacy konstruktor przypisania");
    static_assert(!is_default_constructible_v<T>, "Niedozwolony domysny konstruktor");
    static_assert(!is_constructible_v<T>, "Zakaz pustej konstrukcji"); //Chyba to samo co wyżej, ale better safe than sorry.
}

void statyczne_zapewnienia_word() {
    podstawowe_statyczne_zapewnienia<Word>();
    static_assert(is_constructible_v<Word, size_t, size_t, orientation_t, string>, "Wymagana mozliwosc konstrukcji");
}

void statyczne_zapewnienia_rectarea() {
    podstawowe_statyczne_zapewnienia<RectArea>();
    static_assert(is_constructible_v<RectArea, pos_t, pos_t>, "Wymagana mozliwosc konstrukcji");
}

void statyczne_zapewnienia_crossword() {
    podstawowe_statyczne_zapewnienia<Crossword>();
    static_assert(is_constructible_v<Crossword, Word, std::initializer_list<Word>>, "Wymagana mozliwosc konstrukcji");
}

void basic_word() {
    size_t smax = numeric_limits<size_t>::max();
    
    {
        Word word(smax, smax, orientation_t::H, "Abcde");
        assert(word.get_start_position() == pos_t(smax, smax));
        assert(word.get_end_position() == pos_t(smax, smax));
        assert(word.rect_area().get_left_top() == pos_t(smax, smax));
        assert(word.rect_area().get_right_bottom() == pos_t(smax, smax));
        assert(word.at(0) == 'A');
        assert(word.length() == size_t(1));
        for(size_t i = 1; i < 12345678; i++)
            assert(word.at(i) == DEFAULT_CHAR);
        assert(word.at(smax) == DEFAULT_CHAR);
    }
    
    {
        Word word(smax, smax, orientation_t::V, "XDdDdDDDdDdDDd");
        assert(word.get_start_position() == pos_t(smax, smax));
        assert(word.get_end_position() == pos_t(smax, smax));
        assert(word.rect_area().get_left_top() == pos_t(smax, smax));
        assert(word.rect_area().get_right_bottom() == pos_t(smax, smax));
        assert(word.at(0) == 'X');
        assert(word.length() == size_t(1));
        for(size_t i = 1; i < 12345678; i++)
            assert(word.at(i) == DEFAULT_CHAR);
        assert(word.at(smax) == DEFAULT_CHAR);
    }

    {
        Word word(smax - 1, smax, orientation_t::H, "UY3723173217");
        assert(word.get_start_position() == pos_t(smax - 1, smax));
        assert(word.get_end_position() == pos_t(smax, smax));
        assert(word.rect_area().get_left_top() == pos_t(smax - 1, smax));
        assert(word.rect_area().get_right_bottom() == pos_t(smax, smax));
        assert(word.at(0) == 'U');
        assert(word.at(1) == 'Y');
        assert(word.length() == size_t(2));
        for(size_t i = 2; i < 12345678; i++)
            assert(word.at(i) == DEFAULT_CHAR);
        assert(word.at(smax) == DEFAULT_CHAR);
    }

    {
        Word word(smax, smax - 1, orientation_t::V, "PO          ");
        assert(word.get_start_position() == pos_t(smax, smax - 1));
        assert(word.get_end_position() == pos_t(smax, smax));
        assert(word.rect_area().get_left_top() == pos_t(smax, smax - 1));
        assert(word.rect_area().get_right_bottom() == pos_t(smax, smax));
        assert(word.at(0) == 'P');
        assert(word.at(1) == 'O');
        assert(word.length() == size_t(2));
        for(size_t i = 2; i < 12345678; i++)
            assert(word.at(i) == DEFAULT_CHAR);
        assert(word.at(smax) == DEFAULT_CHAR);
    }

    {
        Word word(smax - 1, smax, orientation_t::V, "A(*&^%$#@#$%^&*()(*&");
        assert(word.get_start_position() == pos_t(smax - 1, smax));
        assert(word.get_end_position() == pos_t(smax - 1, smax));
        assert(word.rect_area().get_left_top() == pos_t(smax - 1, smax));
        assert(word.rect_area().get_right_bottom() == pos_t(smax - 1, smax));
        assert(word.at(0) == 'A');
        assert(word.length() == size_t(1));
        for(size_t i = 1; i < 12345678; i++)
            assert(word.at(i) == DEFAULT_CHAR);
        assert(word.at(smax) == DEFAULT_CHAR);
    }

    {
        Word word(0, 0, orientation_t::H, "\t\t\n\n\n\t\t");
        assert(word.get_start_position() == pos_t(0, 0));
        assert(word.get_end_position() == pos_t(6, 0));
        assert(word.rect_area().get_left_top() == pos_t(0, 0));
        assert(word.rect_area().get_right_bottom() == pos_t(6, 0));
        assert(word.length() == size_t(7));
        for(size_t i = 7; i < 12345678; i++)
            assert(word.at(i) == DEFAULT_CHAR);
        assert(word.at(smax) == DEFAULT_CHAR);
    }

}

void basic_rectarea() {

    constexpr const size_t smax = numeric_limits<size_t>::max();

    {
        RectArea area(pos_t(0, 0), pos_t(smax, smax));
        assert(!area.empty());
        RectArea area2(area);
        area2.embrace(pos_t(smax, smax));
        area2.embrace(pos_t(0, 0));
        area *= area2;
        assert(!area.empty());
        assert(!area2.empty());
        assert(area.get_left_top() == pos_t(0, 0));
        assert(area.get_right_bottom() == pos_t(smax, smax));
        assert(area2.get_left_top() == pos_t(0, 0));
        assert(area2.get_right_bottom() == pos_t(smax, smax));
        
    }

    {
        RectArea area(pos_t(smax, 0), pos_t(0, smax));
        assert(area.empty());
        area *= area;
        assert(area.empty());
    }

    {
        RectArea area(DEFAULT_EMPTY_RECT_AREA);
        assert(area.empty());
        area.embrace(pos_t(216, 379));
        assert(!area.empty());
        assert(area.get_left_top() == pos_t(216, 379));
        assert(area.get_right_bottom() == pos_t(216, 379));
        assert(area.size() == dim_t(1, 1));
    }
}

const int only_this_testcase = -1;

void ostateczny_test(vector<pos_t> positions, string charset) {
    assert(!positions.empty());
    assert(!charset.empty());
    const int test_count = 22345;
    const int test_size = 15;
    const int max_len = 7;
    mt19937 gen;
    auto give_char = [&] {
        return charset[gen() % charset.size()];
    };
    auto give_string = [&] (int len) {
        assert(len);
        string res = "";
        for(int i = 0; i < len; i++)
            res += give_char();
        return res;
    };
    auto give_position = [&]() {
        return positions[gen() % positions.size()];
    };
    auto give_orientation = [&] () {
        const vector<orientation_t> orientations = {orientation_t::H, orientation_t::V};
        return orientations[gen() % orientations.size()]; 
    };
    auto run_testcase = [&]() {
        vector<Crossword> cws;
        for(int i = 0; i < test_size; i++) {
            auto position = give_position();
            auto orientation = give_orientation();
            auto str = give_string(gen() % max_len + 1);
            Word w(position.first, position.second, orientation, str);
            cws.emplace_back(Crossword(w, {}));
        }
        while(!cws.empty()) {
            if(gen() % 2 == 0)
                cws[0] += cws.back();
            else
                cws[0] = cws[0] + cws.back();
            cout << cws[0] << endl;
            cws.pop_back();
        }
    };

    if(only_this_testcase != -1) {
        gen = mt19937(only_this_testcase);
        run_testcase();
        return;
    }
    
    for(int i = 0; i < test_count; i++) {
        cout << "-----" << endl;
        cout << "testcase " << i << endl;
        cout << endl;
        gen = mt19937(i);
        run_testcase();
    }
}

int main() {
    statyczne_zapewnienia_word();
    statyczne_zapewnienia_rectarea();
    statyczne_zapewnienia_crossword();
    proste1();
    basic_word();
    basic_rectarea();

    // {
    //     vector<pos_t> positions;
    //     for(int i = 0; i < 10; i++)
    //         for(int j = 0; j < 10; j++)
    //             positions.emplace_back(i, j);

    //     ostateczny_test(positions, "aAbB");
    // }
    // {
    //     constexpr const size_t smax = numeric_limits<size_t>::max();
    //     vector<pos_t> positions;
    //     for(size_t i = 0; i < 10; i++)
    //         for(size_t j = 0; j < 10; j++)
    //             positions.emplace_back(smax - i, smax - j);

    //     ostateczny_test(positions, "*^");
    // }
    
}
