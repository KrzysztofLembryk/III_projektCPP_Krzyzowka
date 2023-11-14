
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
using std::min;
using std::max;

using pos_t = pair<size_t, size_t>;
using dim_t = pair<size_t, size_t>;

const RectArea EMPTY_AREA(pos_t(2, 0), pos_t(1, 0));

namespace
{
    constexpr char NOT_A_LETTER = '?';
    //const RectArea EMPTY_AREA(pos_t(2, 0), pos_t(1, 0));
    // constexpr string DEFAULT_STRING("?");

    // -----WORD_POS CLASS-----

    // pos_t WordPos::getPos() const
    // {
    //     return pos;
    // }

    // orientation_t WordPos::getOrient() const
    // {
    //     return orient;
    // }

}

// -----WORD CLASS-----

// Construcotrs:
Word::Word() = delete;

Word::Word(size_t x, size_t y, orientation_t orient, std::string const &_word) : posAndOrient(x, y, orient), word(!_word.empty() ? _word : "?") {}

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

std::weak_ordering Word::operator<=>(const Word &other) const
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

RectArea Word::rect_area() const
{
    return RectArea(pos_t(posAndOrient.getPos()), 
        pos_t(this->get_end_position()));

}

// -----RECT_AREA CLASS-----

point_placement RectArea::isInside(pos_t p) const
{
    if(atLeastOneElemExist)
    {
        if (p.second < topLeft.second)
            return OVER;
        if (p.second > bottomRight.second)
            return UNDER;
        if (p.first > bottomRight.first)
            return ON_THE_RIGHT;
        if (p.first < topLeft.first)
            return ON_THE_LEFT;
        return INSIDE;
    }
    return NO_AREA;
}

dim_t RectArea::calcArea()
{
    atLeastOneElemExist = true;
    
    if (bottomRight.first < topLeft.first ||
        bottomRight.second < topLeft.second)
        {
            atLeastOneElemExist = false;
            return dim_t(0, 0);
        }
        
    return dim_t(bottomRight.first - topLeft.first,
                 bottomRight.second - topLeft.second);
}

// Constructors:
RectArea::RectArea() = delete;

RectArea::RectArea(pos_t _topLeft, pos_t _bottomRight) : topLeft(_topLeft),
                                                    bottomRight(_bottomRight), areaSize(calcArea()) {}

RectArea::RectArea(const RectArea &other) = default;
//: topLeft(other.topLeft),
                                            // bottomRight(other.bottomRight), areaSize(other.areaSize),
                                            // atLeastOneElemExist(other.atLeastOneElemExist) {}

// nie wiem czy trzeba, bo w sumie jak mamy inty to one sa kopiowane po prostu
RectArea::RectArea(RectArea &&other) = default;
// : topLeft(move(other.topLeft)),
// bottomRight(move(other.bottomRight)), areaSize(move(other.areaSize)),
// atLeastOneElemExist(move(other.atLeastOneElemExist)) {}

RectArea::~RectArea() = default;

RectArea &RectArea::operator=(const RectArea &rhs)
{
    if (this != &rhs)
    {
        topLeft = rhs.topLeft;
        bottomRight = rhs.bottomRight;
        areaSize = rhs.areaSize;
        atLeastOneElemExist = rhs.atLeastOneElemExist;
    }
    return *this;
}

RectArea &RectArea::operator=(RectArea &&rhs)
{
    if (this != &rhs)
    {
        topLeft = move(rhs.topLeft);
        bottomRight = move(rhs.bottomRight);
        areaSize = move(rhs.areaSize);
        atLeastOneElemExist = move(rhs.atLeastOneElemExist);
    }
    return *this;
}

RectArea &RectArea::operator*=(const RectArea &rhs)
{
    if (this->empty())
    {
        if(atLeastOneElemExist && rhs.isInside(topLeft) == INSIDE)
        {
            *this = RectArea(topLeft, topLeft);
        }
        else
            *this = EMPTY_AREA;
    }
    else if(rhs.empty())
    {
        if(rhs.atLeastOneElemExist && this->isInside(rhs.topLeft) == INSIDE)
        {
            *this = RectArea(rhs.topLeft, rhs.topLeft);
        }
        else
            *this = EMPTY_AREA;
    }
    else
    {
        if (topLeft.first > rhs.bottomRight.first ||
            topLeft.second > rhs.bottomRight.second ||
            bottomRight.first < rhs.topLeft.first ||
            bottomRight.second < rhs.topLeft.second)
        {
            *this = EMPTY_AREA;
        }
        else
        {
            pos_t newTopLeft(max(topLeft.first, rhs.topLeft.first), 
                max(topLeft.second, rhs.topLeft.second));
            pos_t newBottomRight(min(bottomRight.first, rhs.bottomRight.first),
                min(bottomRight.second, rhs.bottomRight.second));

            *this = RectArea(newTopLeft, newBottomRight);
        }
    }

    return *this;
}

const RectArea RectArea::operator*(const RectArea &rhs) const
{
    RectArea newRectArea = *this;
    newRectArea *= rhs;

    return newRectArea;
}


// Getters:
pos_t RectArea::get_left_top() const
{
    return topLeft;
}

pos_t RectArea::get_right_bottom() const
{
    return bottomRight;
}

dim_t RectArea::size() const
{
    return areaSize;
}

bool RectArea::empty() const
{
    return areaSize == dim_t(0, 0);
}

// Setters:
void RectArea::set_left_top(pos_t p)
{
    topLeft = p;
    areaSize = calcArea();
}

void RectArea::set_right_bottom(pos_t p)
{
    bottomRight = p;
    areaSize = calcArea();
}

void RectArea::extend_to_left_or_right(pos_t p)
{
    if (p.first < topLeft.first)
        topLeft.first = p.first;
    else if (p.first > bottomRight.first)
        bottomRight.first = p.first;
}

void RectArea::embrace(pos_t p)
{
    // RectArea is empty when has only one point, meaning topLeft = bottomRight
    // or when is null set
    if (!this->empty() || (this->empty() && atLeastOneElemExist))
    {
        point_placement p_pos = isInside(p);

        switch (p_pos)
        {
        case OVER:
            topLeft.second = p.second;

            extend_to_left_or_right(p);

            break;
        case UNDER:
            bottomRight.second = p.second;

            extend_to_left_or_right(p);

            break;
        case ON_THE_RIGHT:
        case ON_THE_LEFT:
            extend_to_left_or_right(p);

            break;
        default:
            break;
        }
    }
    else if(!atLeastOneElemExist)
    {
        topLeft = p;
        bottomRight = p;   
    }

    this->calcArea();
}

// -----CROSSWORD CLASS-----

// Crossword::Crossword(const Word &word)
// {
//     m_words.push_back(word);
//     // rectAreaInit
// }

// Crossword::Crossword(const Word &firstWord, const std::vector<Word> &words)
// {
// }

// Crossword::Crossword(Word &&word) noexcept
// {
// }

// Crossword::Crossword(Word &&firstWord, std::vector<Word> &&words) noexcept
// {
// }

// void Crossword::insert_word(const Word &word)
// {
// }

// dim_t Crossword::size() const
// {
// }

// dim_t Crossword::word_count() const
// {
// }

// bool Crossword::colides(const Word &word)
// {
// }

// Crossword Crossword::operator+(const Crossword &other)
// {
// }

// Crossword Crossword::operator+=(const Crossword &crossword)
// {
// }

// std::ostream &operator<<(std::ostream &out, const Crossword &crossword)
// {
// }

// Crossword &Crossword::operator=(const Crossword &other)
// {
// }

// Crossword &Crossword::operator=(Crossword &&other) noexcept
// {
// }
