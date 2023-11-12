#include <iostream>
// #include <utility>
#include <typeinfo>
#include <stdexcept>

using std::cerr;
using std::cin;
using std::cout;
using std::pair;
using std::string;
using std::move;

using pos_t = pair<size_t, size_t>;
using dim_t = pair<size_t, size_t>;

enum orientation_t : bool
{
    H = true,
    V = false
};

namespace
{
    /**
     *
     */
    class WordPos
    {
    private:
        pos_t pos;
        orientation_t orient;

    public:
        // Constructors:
        WordPos() = delete;
        WordPos(size_t x, size_t y, orientation_t _orient) : pos(x, y), orient(_orient) {}
        WordPos(pos_t const &p, orientation_t const &o) : pos(p), orient(o)  {}
        // Copy constructor:
        WordPos(const WordPos &w) : pos(w.pos), orient(w.orient) {}
        // Move Constructor:
        WordPos(WordPos &&w) : pos(move(w.pos)), orient(move(w.orient)) {}

        // Destructors:
        ~WordPos() = default;

        // Getters:
        // we return const reference not to allow to change these values
        pos_t const &getPos() const
        { 
            return pos;
        }
        orientation_t const &getOrient() const
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
    Word(size_t x, size_t y, orientation_t orient, string const  &_word) : posAndOrient(x, y, orient)
    {
        // need to check rvalues in second assignment
        if (_word.empty())
            word = "?";
        else
            word = _word;
    }

    // Copy constructor:
    Word(const Word &other) : posAndOrient(other.posAndOrient), word(other.word) {}

    // Move construcotr:
    Word(Word &&other) : posAndOrient(move(other.posAndOrient)), word(move(other.word)) {}

    // Destructors:
    ~Word() = default;

    // Getters:
    const string &get_word() 
    {
        return word;
    }
    pos_t const &get_start_position() const
    {
        return posAndOrient.getPos();
    }

    pos_t get_end_position()
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

    orientation_t const get_orientation() const
    {
        return posAndOrient.getOrient();
    }

    char at(size_t idx)
    {
        if (idx < word.size())
            return word[idx];
        throw std::invalid_argument("Word - at(idx) - given idx is out of bounds\n");
    }

    size_t length()
    {
        return word.size();
    }

    void print()
    {
        cout << "string: " << word << ", ";
        posAndOrient.print();
    }

};

int main()
{
    Word w1(1, 1, H, "Krakow");
    Word w2(move(w1));

    cout << "w1: ";
    w1.print();

    cout << "\nw2: ";
    w2.print();

   
    return 0;
}