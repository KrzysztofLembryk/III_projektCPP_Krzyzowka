#include <stdexcept>
#include <cassert>
#include "crosswords.h"

namespace {

#define LETTER_EXISTS(y, x) m_letters.count(pos_t(y, x)) != 0
#define LETTER(y, x) m_letters.at(pos_t(y, x))

using std::cerr;
using std::cin;
using std::cout;
using std::move;
using std::pair;
using std::string;
using std::min;
using std::max;

const size_t SHIFT_VAL = 1;

}

// -----WORD CLASS-----

void Word::checkOutOfBoundWord()
{
    size_t startPos;
    size_t newEndIdx = 0;
    size_t wordLen = word.size();

    if(posAndOrient.getOrient() == H)
        startPos = posAndOrient.getPos().first;
    else
        startPos = posAndOrient.getPos().second;

    for(size_t i = 1; i < wordLen; i++)
    {
        if(startPos + i >= startPos)
            newEndIdx = i;
        else
            break;
    }

    // when newEndIdx is not equal wordLen - 1, this means that we went over
    // size_t max. So we need to cut some part of our word.
    if(newEndIdx != wordLen - 1)
        word.erase(newEndIdx + 1);
}

// Constructors:
//Word::Word() = delete;

Word::Word(size_t x, size_t y, orientation_t orient, std::string const &_word) : posAndOrient(x, y, orient), word(!_word.empty() ? _word : DEFAULT_WORD) 
    {
        checkOutOfBoundWord();
    }

// Copy constructor:
Word::Word(const Word &other) = default;

// Move constructor:
Word::Word(Word &&other) noexcept = default;

// Destructors:
Word::~Word() = default;

// Operators:
Word &Word::operator=(const Word &rhs) = default;

Word &Word::operator=(Word &&rhs) noexcept = default;

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
        return pos_t(p.first + word.size() - SHIFT_VAL, p.second);
    else
        return pos_t(p.first, p.second + word.size() - SHIFT_VAL);
}

orientation_t Word::get_orientation() const
{
    return posAndOrient.getOrient();
}

char Word::at(size_t idx) const
{
    //assert(idx < word.size());
    if(idx < word.size())
        return word[idx];
    return DEFAULT_CHAR;
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

// -----RECT AREA CLASS-----

point_placement RectArea::isInside(pos_t p) const
{
    if(!this->empty())
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
    if (bottomRight.first < topLeft.first ||
        bottomRight.second < topLeft.second)
        {
            return dim_t(0, 0);
        }
    
    return dim_t(bottomRight.first - topLeft.first + SHIFT_VAL,
                 bottomRight.second - topLeft.second + SHIFT_VAL);
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
    }
    return *this;
}

RectArea &RectArea::operator*=(const RectArea &rhs)
{
    if (this->empty() || rhs.empty())
    {
        *this = DEFAULT_EMPTY_RECT_AREA;
    }
    else
    {
        if (topLeft.first > rhs.bottomRight.first ||
            topLeft.second > rhs.bottomRight.second ||
            bottomRight.first < rhs.topLeft.first ||
            bottomRight.second < rhs.topLeft.second)
        {
            *this = DEFAULT_EMPTY_RECT_AREA;
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
    if (!this->empty())
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
    else
    {
        topLeft = p;
        bottomRight = p;   
    }

    areaSize = calcArea();
}

// -----CROSSWORD CLASS-----

Crossword::Crossword(Crossword&& other) noexcept
        : m_words(std::move(other.m_words)),
          m_rectArea(std::move(other.m_rectArea)),
          m_letters(std::move(other.m_letters)) {}

Crossword& Crossword::operator=(Crossword&& other) noexcept
{
    m_words = std::move(other.m_words);
    m_rectArea = std::move(other.m_rectArea);
    m_letters = std::move(other.m_letters);
    return *this;
}

Crossword::Crossword(const Word& word)
        : m_rectArea(RectArea(word.get_start_position(),
                              word.get_end_position()))
{
    insert_word(word);
}

Crossword::Crossword(const Word& firstWord, const std::vector<Word>& words)
        : m_rectArea(RectArea(firstWord.get_start_position(),
                              firstWord.get_end_position()))
{
    insert_word(firstWord);
    for (const auto& word: words)
    {
        insert_word(word);
    }
}

bool Crossword::insert_word(const Word& word)
{
    if (!collides(word))
    {
        m_words.push_back(word);
        m_rectArea.embrace(word.get_start_position());
        m_rectArea.embrace(word.get_end_position());

        size_t x = word.getX();
        size_t y = word.getY();
        orientation_t o = word.get_orientation();

        for (size_t i = 0; i < word.length(); i++)
        {
            if (o == H)
            {
                m_letters[pos_t(y, x + i)] = {
                        word.at(i),
                        o,
                        m_letters.count(pos_t(y, x + i)) != 0};
            } else // (o == V)
            {
                m_letters[pos_t(y + i, x)] = {
                        word.at(i),
                        o,
                        m_letters.count(pos_t(y, x + i)) != 0};
            }
        }
        return true;
    }
    return false;
}

dim_t Crossword::size() const
{
    return m_rectArea.size();
}

dim_t Crossword::word_count() const
{
    dim_t result = {0, 0};
    for (const auto& word: m_words)
    {
        if (word.get_orientation() == H)
        { result.first++; }
        else
        { result.second++; }
    }
    return result;
}

bool Crossword::collides(const Word& word) const
{
    size_t x = word.getX();
    size_t y = word.getY();
    orientation_t o = word.get_orientation();

    if (o == H)
    {
        for (size_t i = 0; i < word.length(); i++)
        {
            if (i != word.length() - 1 && x + i == SIZE_MAX)
            { return true; }
            if (LETTER_EXISTS(y, x + i))
            {
                Letter letter = LETTER(y, x + i);
                if (letter.character == word.at(i) &&
                    letter.orientation != o &&
                    !letter.intersection)
                {
                    if (y > 0 && LETTER_EXISTS(y - 1, x + i) &&
                        LETTER(y - 1, x + i).intersection)
                    { return true; }
                    if (y < SIZE_MAX && LETTER_EXISTS(y + 1, x + i) &&
                        LETTER(y + 1, x + i).intersection)
                    { return true; }
                } else
                { return true; }
            } else // (!(LETTER_EXISTS(y, x + i)))
            {
                if (y > 0 && LETTER_EXISTS(y - 1, x + i))
                { return true; }
                if (y < SIZE_MAX && LETTER_EXISTS(y + 1, x + i))
                { return true; }
            }
        }
        if (x > 0 && LETTER_EXISTS(y, x - 1))
        { return true; }
        if (x < SIZE_MAX - word.length() + 1 &&
            LETTER_EXISTS(y, x + word.length()))
        { return true; }
    } else // (o == V)
    {
        for (size_t i = 0; i < word.length(); i++)
        {
            if (i != word.length() - 1 && y + i == SIZE_MAX)
            { return true; }
            if (LETTER_EXISTS(y + i, x))
            {
                Letter letter = LETTER(y + i, x);
                if (letter.character == word.at(i) &&
                    letter.orientation != o &&
                    !letter.intersection)
                {
                    if (x > 0 && LETTER_EXISTS(y + i, x - 1) &&
                        LETTER(y + i, x - 1).intersection)
                    { return true; }
                    if (x < SIZE_MAX && LETTER_EXISTS(y + i, x + 1) &&
                        LETTER(y + i, x + 1).intersection)
                    { return true; }
                } else
                { return true; }
            } else // (!(LETTER_EXISTS(y + i, x)))
            {
                if (x > 0 && LETTER_EXISTS(y + i, x - 1))
                { return true; }
                if (x < SIZE_MAX && LETTER_EXISTS(y + i, x + 1))
                { return true; }
            }
        }
        if (y > 0 && LETTER_EXISTS(y - 1, x))
        { return true; }
        if (y < SIZE_MAX - word.length() + 1 &&
            LETTER_EXISTS(y + word.length(), x))
        { return true; }
    }
    return false;
}

Crossword Crossword::operator+(const Crossword& other) const
{
    Crossword crossword(m_words[0], m_words);
    // The first m_word will collide, so it won't be added.
    for (const auto& word: other.m_words)
    {
        crossword.insert_word(word);
    }
    return crossword;
}

Crossword Crossword::operator+=(const Crossword& other) const
{
    return *this + other;
}

void writeCrosswordLetter(std::ostream& out, char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
        out << toupper(c);
    } else
    { out << DEFAULT_CHAR; }
}

std::ostream& operator<<(std::ostream& out, const Crossword& crossword)
{
    pos_t start = crossword.m_rectArea.get_left_top();
    pos_t end = crossword.m_rectArea.get_right_bottom();
    start.first--;
    start.second--;
    end.first++;
    end.second++;
    auto pointsItr = crossword.m_letters.cbegin();

    for (size_t j = start.second; j <= end.second; j++)
    {
        for (size_t i = start.first; i < end.first; i++)
        {
            if (pointsItr != crossword.m_letters.cend() &&
                (*pointsItr).first == pos_t(j, i))
            {
                writeCrosswordLetter(out, (*pointsItr).second.character);
                pointsItr++;
            } else
            { out << CROSSWORD_BACKGROUND; }
            out << ' ';
        }
        out << CROSSWORD_BACKGROUND;
        out << std::endl;
    }
    return out;
}