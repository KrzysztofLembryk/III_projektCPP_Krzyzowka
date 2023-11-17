#include <cassert>
#include <string>
#include <utility>
#include "crosswords.h"

namespace
{
    #define LETTER_EXISTS(x, y) (letters.count(pos_t(x, y)) != 0)
    #define LETTER(x, y) (letters.at(pos_t(x, y)))

    using std::cerr;
    using std::cin;
    using std::cout;
    using std::move;
    using std::pair;
    using std::string;
    using std::min;
    using std::max;

    constexpr size_t SHIFT_VAL = 1;
    constexpr dim_t ZERO_DIM(0, 0);
}

// -----WORD CLASS-----

/**
 * Function checks whether any of end position coordinates of given word isn't
 * greater than max(size_t). If it is string word is accordingly trimmed. 
 * Then we return correct end position of word.
*/
pos_t Word::checkAndGetEndPos()
{
    size_t startPos;
    size_t wordLen = word.size();

    if (posAndOrient.getOrient() == H)
    {
        startPos = posAndOrient.getPos().first;
    } else
    {
        startPos = posAndOrient.getPos().second;
    }

    for (size_t i = 0; i < wordLen - 1; i++)
    {
        if (startPos + i == SIZE_MAX)
        {
            word.erase(i + 1);
            break;
        }
    }

    pos_t p = posAndOrient.getPos();

    if (posAndOrient.getOrient() == H)
    {
        return {p.first + word.size() - SHIFT_VAL, p.second};
    } else
    {
        return {p.first, p.second + word.size() - SHIFT_VAL};
    }
}

// Constructor:
Word::Word(size_t x, size_t y, orientation_t orient, std::string const& _word)
        : posAndOrient(x, y, orient),
          word(!_word.empty() ? _word : DEFAULT_WORD),
          endPos(checkAndGetEndPos()) {}

// Operators:
/**
 * == and <=> operators for Word class are done via comparing their
 * WordPos variables, since we have lexicographic order of (x, y, orient).
*/
bool Word::operator==(const Word& other) const
{
    return posAndOrient == other.posAndOrient;
}

std::weak_ordering Word::operator<=>(const Word& other) const
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
    return endPos;
}

orientation_t Word::get_orientation() const
{
    return posAndOrient.getOrient();
}

char Word::at(size_t idx) const
{
    if (idx < word.size())
    {
        return word[idx];
    }

    return DEFAULT_CHAR;
}

size_t Word::length() const
{
    return word.size();
}

RectArea Word::rect_area() const
{
    return {pos_t(posAndOrient.getPos()),
            pos_t(this->get_end_position())};

}

// -----RECT AREA CLASS-----

/**
 * Function checks where given point p is regarding our rectangle. It returns
 * the information whether point is over, under etc our rectangle.
*/
point_placement RectArea::isInside(pos_t p) const
{
    if (!this->empty())
    {
        if (p.second < topLeft.second)
        {
            return OVER;
        }
        if (p.second > bottomRight.second)
        {
            return UNDER;
        }
        if (p.first > bottomRight.first)
        {
            return ON_THE_RIGHT;
        }
        if (p.first < topLeft.first)
        {
            return ON_THE_LEFT;
        }
        return INSIDE;
    }
    return NO_AREA;
}

/**
 * Function calculates area of our rectangle. It returns pair (width, height)
 * of our rectangle. It also checks if our rectangle is empty or bigger than
 * size_t max.
*/
dim_t RectArea::calcArea() const
{
    if (bottomRight.first < topLeft.first ||
        bottomRight.second < topLeft.second)
    {
        return ZERO_DIM;
    }

    assert((!(topLeft.first == 0 && bottomRight.first == SIZE_MAX) &&
            !(topLeft.second == 0 && bottomRight.second == SIZE_MAX)));

    return {bottomRight.first - topLeft.first + SHIFT_VAL,
            bottomRight.second - topLeft.second + SHIFT_VAL};
}

// Constructor:
RectArea::RectArea(pos_t _topLeft, pos_t _bottomRight)
    : topLeft(std::move(_topLeft)),
      bottomRight(std::move(_bottomRight)),
      areaSize(calcArea()) {}

/**
 * Operator *= finds coordinates of rectangle that is an intersection of
 * this and rhs rectangles.
*/
RectArea& RectArea::operator*=(const RectArea& rhs)
{
    if (this->empty() || rhs.empty())
    {
        *this = DEFAULT_EMPTY_RECT_AREA;
    } else
    {
        if (topLeft.first > rhs.bottomRight.first ||
            topLeft.second > rhs.bottomRight.second ||
            bottomRight.first < rhs.topLeft.first ||
            bottomRight.second < rhs.topLeft.second)
        {
            *this = DEFAULT_EMPTY_RECT_AREA;
        } else
        {
            pos_t newTopLeft(max(topLeft.first, rhs.topLeft.first),
                             max(topLeft.second, rhs.topLeft.second));
            pos_t newBottomRight(min(bottomRight.first, rhs.bottomRight.first),
                                 min(bottomRight.second,
                                     rhs.bottomRight.second));

            *this = RectArea(newTopLeft, newBottomRight);
        }
    }

    return *this;
}

RectArea RectArea::operator*(const RectArea& rhs) const
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
    return areaSize == ZERO_DIM;
}

// Setters:
/**
 * After we set new topLeft or bottomRight we need to calculate area again.
*/
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

/**
 * Function extends width of rectangle either to lef or right 
 * to accommodate given point p.
*/
void RectArea::extend_to_left_or_right(pos_t p)
{
    if (p.first < topLeft.first)
    {
        topLeft.first = p.first;
    } else if (p.first > bottomRight.first)
    {
        bottomRight.first = p.first;
    }
}

/**
 * Embrace function makes minimal enlargement of rectangle area so that it can
 * accommodate given point and all old points. It calculates new area size
 * after accommodation.
*/
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
    } else
    {
        topLeft = p;
        bottomRight = p;
    }

    areaSize = calcArea();
}

// -----CROSSWORD CLASS-----

// Constructors:
Crossword::Crossword(Crossword&& other) noexcept
        : words(std::move(other.words)),
          rectArea(std::move(other.rectArea)),
          letters(std::move(other.letters))
{
    if (words.empty())
    {
        letters.clear();
        rectArea = DEFAULT_EMPTY_RECT_AREA;
    }
}

Crossword::Crossword(const Word& word)
        : rectArea(RectArea(word.get_start_position(),
                            word.get_end_position()))
{
    insert_word(word);
}

Crossword::Crossword(const Word& firstWord, const std::vector<Word>& words)
        : rectArea(RectArea(firstWord.get_start_position(),
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
        words.push_back(word);
        rectArea.embrace(word.get_start_position());
        rectArea.embrace(word.get_end_position());

        size_t x = word.get_start_position().first;
        size_t y = word.get_start_position().second;
        orientation_t o = word.get_orientation();
        bool intersection;

        for (size_t i = 0; i < word.length(); i++)
        {
            if (o == H)
            {
                intersection = letters.count(pos_t(x + i, y)) != 0;
                letters[pos_t(x + i, y)] = {
                        word.at(i),
                        o,
                        intersection};
            } else // (o == V)
            {
                intersection = letters.count(pos_t(x, y + i)) != 0;
                letters[pos_t(x, y + i)] = {
                        word.at(i),
                        o,
                        intersection};
            }
        }
        return true;
    }
    return false;
}

dim_t Crossword::size() const
{
    return rectArea.size();
}

dim_t Crossword::word_count() const
{
    dim_t result = {0, 0};
    for (const auto& word: words)
    {
        if (word.get_orientation() == H)
        { result.first++; }
        else
        { result.second++; }
    }
    return result;
}

// Auxiliary Function
unsigned char toCrosswordLetter(unsigned char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
        return toupper(c);
    } else
    { return DEFAULT_CHAR; }
}

/**
 * Checks if a word collides with any other word in the crossword.
 * A word collides with another if they have the same orientation and
 * and are separated by less than two spaces or they have an opposite
 * orientation, are separated by less then two spaces and don't
 * share a letter.
 * */
bool Crossword::collides(const Word& word) const
{
    size_t x = word.get_start_position().first;
    size_t y = word.get_start_position().second;
    orientation_t o = word.get_orientation();

    if (o == H)
    {
        for (size_t i = 0; i < word.length(); i++)
        {
            if (LETTER_EXISTS(x + i, y))
            {
                Letter letter = LETTER(x + i, y);
                if (toCrosswordLetter(letter.character) ==
                    toCrosswordLetter(word.at(i)) &&
                    letter.orientation != o &&
                    !letter.intersection)
                {
                    if (y > 0 && LETTER_EXISTS(x + i, y - 1) &&
                        LETTER(x + i, y - 1).intersection)
                    { return true; }
                    if (y < SIZE_MAX && LETTER_EXISTS(x + i, y + 1) &&
                        LETTER(x + i, y + 1).intersection)
                    { return true; }
                } else
                { return true; }
            } else // (!LETTER_EXISTS(x + i, y))
            {
                if (y > 0 && LETTER_EXISTS(x + i, y - 1))
                { return true; }
                if (y < SIZE_MAX && LETTER_EXISTS(x + i, y + 1))
                { return true; }
            }
        }
        if (x > 0)
        {
            if (LETTER_EXISTS(x - 1, y) ||
                (y > 0 && LETTER_EXISTS(x - 1, y - 1)) ||
                (y < SIZE_MAX && LETTER_EXISTS(x - 1, y + 1)))
            { return true; }
        }
        if (x < SIZE_MAX - word.length() + 1)
        {
            if (LETTER_EXISTS(x + word.length(), y) ||
                (y > 0 && LETTER_EXISTS(x + word.length(), y - 1)) ||
                (y < SIZE_MAX && LETTER_EXISTS(x + word.length(), y + 1)))
            { return true; }
        }
    } else // (o == V)
    {
        for (size_t i = 0; i < word.length(); i++)
        {
            if (LETTER_EXISTS(x, y + i))
            {
                Letter letter = LETTER(x, y + i);
                if (toCrosswordLetter(letter.character) ==
                    toCrosswordLetter(word.at(i)) &&
                    letter.orientation != o &&
                    !letter.intersection)
                {
                    if (x > 0 && LETTER_EXISTS(x - 1, y + i) &&
                        LETTER(x - 1, y + i).intersection)
                    { return true; }
                    if (x < SIZE_MAX && LETTER_EXISTS(x + 1, y + i) &&
                        LETTER(x + 1, y + i).intersection)
                    { return true; }
                } else
                { return true; }
            } else // (!LETTER_EXISTS(x, y + i))
            {
                if (x > 0 && LETTER_EXISTS(x - 1, y + i))
                { return true; }
                if (x < SIZE_MAX && LETTER_EXISTS(x + 1, y + i))
                { return true; }
            }
        }
        if (y > 0)
        {
            if (LETTER_EXISTS(x, y - 1) ||
                (x > 0 && LETTER_EXISTS(x - 1, y - 1)) ||
                (x < SIZE_MAX && LETTER_EXISTS(x + 1, y - 1)))
            { return true; }
        }
        if (y < SIZE_MAX - word.length() + 1)
        {
            if (LETTER_EXISTS(x, y + word.length()) ||
                (x > 0 && LETTER_EXISTS(x - 1, y + word.length())) ||
                (x < SIZE_MAX && LETTER_EXISTS(x + 1, y + word.length())))
            { return true; }
        }
    }
    return false;
}

// Operators:
Crossword& Crossword::operator=(Crossword&& other) noexcept
{
    words = std::move(other.words);
    rectArea = std::move(other.rectArea);
    letters = std::move(other.letters);
    if (words.empty())
    {
        letters.clear();
        rectArea = DEFAULT_EMPTY_RECT_AREA;
    }
    return *this;
}

Crossword& Crossword::operator+(const Crossword& other)
{
    return *this += other;
}

Crossword& Crossword::operator+=(const Crossword& other)
{
    for (const auto& word: other.words)
    {
        this->insert_word(word);
    }
    return *this;
}

// Auxiliary Function
void lineOfBackgroundChars(std::ostream& out, size_t width)
{
    for (size_t i = 0; i < width; i++)
    {
        out << CROSSWORD_BACKGROUND;
        out << ' ';
    }
}

std::ostream& operator<<(std::ostream& out, const Crossword& crossword)
{
    if(crossword.size() == ZERO_DIM) { // If that's even possible.
        out << CROSSWORD_BACKGROUND << ' ' << CROSSWORD_BACKGROUND << '\n';
        out << CROSSWORD_BACKGROUND << ' ' << CROSSWORD_BACKGROUND << '\n';
        return out;
    }

    pos_t start = crossword.rectArea.get_left_top();
    pos_t end = crossword.rectArea.get_right_bottom();
    size_t widthMinusOne = end.first - start.first;
    // we would like to add 1 to widthMinusOne,
    // but width could already be SIZE_MAX, so we can't.
    auto pointsItr = crossword.letters.cbegin();

    lineOfBackgroundChars(out, widthMinusOne);
    // Last three characters are added in this way,
    // because widthMinusOne could be equal to SIZE_MAX.
    lineOfBackgroundChars(out, 2);
    out << CROSSWORD_BACKGROUND << '\n';

    for (size_t j = start.second; j <= end.second; j++)
    {
        out << CROSSWORD_BACKGROUND << ' ';
        for (size_t i = start.first; i <= end.first; i++)
        {
            if (pointsItr != crossword.letters.cend() &&
                (*pointsItr).first == pos_t(i, j))
            {
                out << toCrosswordLetter((*pointsItr).second.character);
                pointsItr++;
            } else
            { out << CROSSWORD_BACKGROUND; }
            out << ' ';
            if (i == SIZE_MAX)
            { break; } // If we need to print the whole range
                       // from 0 to SIZE_MAX we will have
                       // an infinite loop without it.
        }
        out << CROSSWORD_BACKGROUND << '\n';
        if (j == SIZE_MAX)
        { break; } // Likewise.
    }

    lineOfBackgroundChars(out, widthMinusOne);
    lineOfBackgroundChars(out, 2);
    out << CROSSWORD_BACKGROUND << '\n';

    return out;
}