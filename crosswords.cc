
#include <iostream>
// #include <utility>
#include <typeinfo>
#include <stdexcept>
#include <cassert>
#include "crosswords.h"

using std::cerr;
using std::cin;
using std::cout;
using std::move;
using std::pair;
using std::string;

using pos_t = pair<size_t, size_t>;
using dim_t = pair<size_t, size_t>;

enum orientation_t : bool
{
    H = false,
    V = true
};

namespace
{
    constexpr char NOT_A_LETTER = '?';
    //constexpr string DEFAULT_STRING("?");

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
        WordPos(const WordPos &w) = default;//: pos(w.pos), orient(w.orient) {}

        // Move Constructor:
        WordPos(WordPos &&w) = default;//: pos(move(w.pos)), orient(move(w.orient)) {}

        // Destructors:
        ~WordPos() = default;

        // Operators:
        WordPos &operator=(const WordPos &rhs) = default;
        WordPos &operator=(WordPos &&rhs) = default;
        auto operator<=>(const WordPos &) const = default;
        
        // Getters:
        // we return const reference not to allow to change these values
        pos_t getPos() const
        {
            return pos;
        }

        orientation_t getOrient() const
        {
            return orient;
        }

        void print()
        {
            cout << "pos: (" << pos.first << ", " << pos.second << ") ";
            cout << "orient: " << orient << "\n";
        }
    };
}

class Word
{
private:
    WordPos posAndOrient;
    string word;

public:
    // Constructors:
    Word() = delete;

    Word(size_t x, size_t y, orientation_t orient, string const &_word) :
            posAndOrient(x, y, orient), word(!_word.empty() ? _word : "?") {}

    // Copy constructor:
    Word(const Word &other) = default;//: posAndOrient(other.posAndOrient), 
    //    word(other.word) {}

    // Move construcotr:
    Word(Word &&other) = default;//: posAndOrient(move(other.posAndOrient)), 
    //    word(move(other.word)) {}

    // Destructors:
    ~Word() = default;

    // Operators:
    Word &operator=(const Word &rhs) = default;
    Word &operator=(Word &&rhs) = default;

    bool operator==(const Word &other) const
    {
        return posAndOrient == other.posAndOrient;
    }

    auto operator<=>(const Word &other) const
    {
        return posAndOrient <=> other.posAndOrient;
    }

    // Getters:
    string const &get_word() const
    {
        return word;
    }

    pos_t get_start_position() const
    {
        return posAndOrient.getPos();
    }

    pos_t get_end_position() const
    {
        pos_t p = posAndOrient.getPos();

        if (posAndOrient.getOrient() == H)
        {
            // pos_t endPos(p.first + word.size(), p.second);
            return pos_t(p.first + word.size(), p.second);
        }
        else
            return pos_t(p.first, p.second + word.size());
    }

    orientation_t get_orientation() const
    {
        return posAndOrient.getOrient();
    }

    char at(size_t idx) const
    {
        if (idx < word.size())
            return word[idx];
        throw std::invalid_argument("Word - at(idx) - given idx is out of bounds\n");
    }

    size_t length() const
    {
        return word.size();
    }

    void print()
    {
        cout << "string: " << word << ", ";
        posAndOrient.print();
    }
};

// TESTS
void printStartTest(string method, string cls)
{
    cout << "STARTING " << method << " TESTS FOR " + cls + " CLASS\n";
}
void printEndTest(string method = "")
{
    cout << method + " TESTS PASSED\n";
}

void move_copy_constructor_Word_TEST()
{
    printStartTest("MOVE_COPY", "WORD");

    Word w1(1, 1, H, "Krakow");
    Word w2(move(w1));

    assert(w2.get_word().compare("Krakow") == 0);
    assert(w1.get_word().compare("") == 0);

    Word wx(Word(1, 1, H, "Warszawa"));

    assert(wx.get_word().compare("Warszawa") == 0);

    Word w3(w2);

    assert(w3.get_word().compare(w2.get_word()) == 0);

    printEndTest("MOVE_COPY");
    cout << "\n";
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
    assert((pos1 <=> pos2) < 0 );
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

    move_copy_constructor_Word_TEST();
    operators_WordPos_TEST();
    operators_Word_TEST();

    return 0;
}


Crossword::Crossword(const Word &word)
{
    m_words.push_back(word);
    //rectAreaInit
}

Crossword::Crossword(const Word& firstWord, const std::vector<Word>& words)
{

}

Crossword::Crossword(Word&& word) noexcept
{

}

Crossword::Crossword(Word&& firstWord, std::vector<Word>&& words) noexcept
{

}

void Crossword::insert_word(const Word& word)
{

}

dim_t Crossword::size() const
{

}

dim_t Crossword::word_count() const
{

}

bool Crossword::colides(const Word &word)
{

}

Crossword Crossword::operator+(const Crossword &other)
{

}

Crossword Crossword::operator+=(const Crossword &crossword)
{

}

std::ostream &operator<<(std::ostream &out, const Crossword &crossword)
{

}

Crossword& Crossword::operator=(const Crossword &other)
{

}

Crossword& Crossword::operator=(Crossword &&other) noexcept
{

}
