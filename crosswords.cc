
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
    // constexpr string DEFAULT_STRING("?");

    // WordPos methods:
    pos_t WordPos::getPos() const
    {
        return pos;
    }

    orientation_t WordPos::getOrient() const
    {
        return orient;
    }
}

// -----WORD CLASS-----

// Construcotrs:
Word::Word() = delete;

Word::Word(size_t x, size_t y, orientation_t orient, std::string const &_word) :
            posAndOrient(x, y, orient), word(!_word.empty() ? _word : "?") {}

Word::Word(const Word &other) = default;

Word::Word(Word &&other) = default;

// Destructors:
Word::~Word() = default;

// Operators:
Word &Word::operator=(const Word &rhs) = default;

Word &Word::operator=(Word &&rhs) = default;

bool Word::operator==(const Word &other) const
{
    return posAndOrient == other.posAndOrient;
}

auto Word::operator<=>(const Word &other) const
{
    return posAndOrient <=> other.posAndOrient;
}

// Getters:
pos_t Word::get_start_position() const
{
    return posAndOrient.getPos();
}

pos_t Word::get_end_position() const
{
    pos_t p = posAndOrient.getPos();

    if (posAndOrient.getOrient() == H)
        return pos_t(p.first + word.size(), p.second);
    else
        return pos_t(p.first, p.second + word.size());
}

orientation_t Word::get_orientation() const
{
    return posAndOrient.getOrient();
}

char Word::at(size_t idx) const
{
    if (idx < word.size())
        return word[idx];
        throw std::invalid_argument("Word - at(idx) - given idx is out of bounds\n");
}

size_t Word::length() const
{
        return word.size();
}


Crossword::Crossword(const Word &word)
{
        m_words.push_back(word);
        // rectAreaInit
}

Crossword::Crossword(const Word &firstWord, const std::vector<Word> &words)
{
}

Crossword::Crossword(Word &&word) noexcept
{
}

Crossword::Crossword(Word &&firstWord, std::vector<Word> &&words) noexcept
{
}

void Crossword::insert_word(const Word &word)
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

Crossword &Crossword::operator=(const Crossword &other)
{
}

Crossword &Crossword::operator=(Crossword &&other) noexcept
{
}
