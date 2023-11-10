#include <iostream>
// #include <utility>
#include <typeinfo>
#include <stdexcept>

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
        WordPos(pos_t p, orientation_t o) : pos(p), orient(o) {}

        // Destructors:
        ~WordPos() = default;

        // Getters:
        // we return const reference not to allow to change these values
        pos_t const &getPos()
        {
            return pos;
        }
        orientation_t const &getOrient()
        {
            return orient;
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
    
    Word(size_t x, size_t y, orientation_t orient, string _word) : posAndOrient(x, y, orient)
    {
        // need to check rvalues in second assignment
        if (_word.empty())
            word = "?";
        else
            word = _word;
    }

    // Destructors:

    // Getters:
    pos_t const &get_start_position()
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

    orientation_t get_orientation()
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
};

int main()
{
    cout << "ciul\n";

    return 0;
}