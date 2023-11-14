#include <typeinfo>
#include <stdexcept>
#include "crosswords.h"

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

using pos_t = pair<size_t, size_t>;
using dim_t = pair<size_t, size_t>;

// -----WORD CLASS-----

// Constructors:
Word::Word(size_t x, size_t y, orientation_t orient, std::string const& word)
        : m_posAndOrient(x, y, orient),
          m_word(!word.empty() ? word : DEFAULT_WORD)
{
}

Word::Word(const Word& other) = default;

Word::Word(Word&& other) noexcept = default;

// Destructors:
Word::~Word() = default;

// Operators:
Word& Word::operator=(const Word& rhs) = default;

Word& Word::operator=(Word&& rhs) noexcept = default;

bool Word::operator==(const Word& other) const
{
    return m_posAndOrient == other.m_posAndOrient;
}

auto Word::operator<=>(const Word& other) const
{
    return m_posAndOrient <=> other.m_posAndOrient;
}

// Getters:
pos_t Word::get_start_position() const
{
    return m_posAndOrient.getPos();
}

pos_t Word::get_end_position() const
{
    pos_t p = m_posAndOrient.getPos();

    if (m_posAndOrient.getOrient() == H)
    {
        return {p.first + m_word.size(), p.second};
    } else
    {
        return {p.first, p.second + m_word.size()};
    }
}

orientation_t Word::get_orientation() const
{
    return m_posAndOrient.getOrient();
}

char Word::at(size_t idx) const
{
    if (idx < m_word.size())
    {
        return m_word[idx];
    }
    throw std::invalid_argument(
            "Word - at(idx) - given idx is out of bounds\n");
}

size_t Word::length() const
{
    return m_word.size();
}

RectArea Word::rect_area() const
{
    return {pos_t(m_posAndOrient.getPos()),
            pos_t(this->get_end_position())};

}

// -----RECT AREA CLASS-----

point_placement RectArea::isInside(pos_t p) const
{
    if (m_atLeastOneElemExist)
    {
        if (p.second < m_topLeft.second)
        {
            return OVER;
        }
        if (p.second > m_bottomRight.second)
        {
            return UNDER;
        }
        if (p.first > m_bottomRight.first)
        {
            return ON_THE_RIGHT;
        }
        if (p.first < m_topLeft.first)
        {
            return ON_THE_LEFT;
        }
        return INSIDE;
    }
    return NO_AREA;
}

pos_t RectArea::calcArea()
{
    m_atLeastOneElemExist = true;

    if (m_bottomRight.first < m_topLeft.first ||
        m_bottomRight.second < m_topLeft.second)
    {
        m_atLeastOneElemExist = false;
        return {0, 0};
    }

    return {m_bottomRight.first - m_topLeft.first,
            m_bottomRight.second - m_topLeft.second};
}

// Constructors:
RectArea::RectArea(pos_t topLeft, pos_t bottomRight) : m_topLeft(topLeft),
                                                       m_bottomRight(
                                                               bottomRight),
                                                       m_areaSize(calcArea())
{
}

RectArea::RectArea(const RectArea& other) = default;
//: m_topLeft(other.m_topLeft),
// m_bottomRight(other.m_bottomRight),
// m_areaSize(other.m_areaSize),
// m_atLeastOneElemExist(other.m_atLeastOneElemExist) {}

RectArea::RectArea(RectArea&& other) noexcept = default;
// : m_topLeft(move(other.m_topLeft)),
// m_bottomRight(move(other.m_bottomRight)),
// m_areaSize(move(other.m_areaSize)),
// m_atLeastOneElemExist(move(other.m_atLeastOneElemExist)) {}

RectArea::~RectArea() = default;

RectArea& RectArea::operator=(const RectArea& rhs)
{
    if (this != &rhs)
    {
        m_topLeft = rhs.m_topLeft;
        m_bottomRight = rhs.m_bottomRight;
        m_areaSize = rhs.m_areaSize;
        m_atLeastOneElemExist = rhs.m_atLeastOneElemExist;
    }
    return *this;
}

RectArea& RectArea::operator=(RectArea&& rhs) noexcept
{
    if (this != &rhs)
    {
        m_topLeft = rhs.m_topLeft;
        m_bottomRight = rhs.m_bottomRight;
        m_areaSize = rhs.m_areaSize;
        m_atLeastOneElemExist = rhs.m_atLeastOneElemExist;
        rhs = DEFAULT_EMPTY_RECT_AREA;
    }
    return *this;
}

RectArea& RectArea::operator*=(const RectArea& rhs)
{
    if (this->empty())
    {
        if (m_atLeastOneElemExist && rhs.isInside(m_topLeft) == INSIDE)
        {
            *this = RectArea(m_topLeft, m_topLeft);
        } else
        {
            *this = DEFAULT_EMPTY_RECT_AREA;
        }
    } else if (rhs.empty())
    {
        if (rhs.m_atLeastOneElemExist &&
            this->isInside(rhs.m_topLeft) == INSIDE)
        {
            *this = RectArea(rhs.m_topLeft, rhs.m_topLeft);
        } else
        {
            *this = DEFAULT_EMPTY_RECT_AREA;
        }
    } else
    {
        if (m_topLeft.first > rhs.m_bottomRight.first ||
            m_topLeft.second > rhs.m_bottomRight.second ||
            m_bottomRight.first < rhs.m_topLeft.first ||
            m_bottomRight.second < rhs.m_topLeft.second)
        {
            *this = DEFAULT_EMPTY_RECT_AREA;
        } else
        {
            pos_t newTopLeft(max(m_topLeft.first, rhs.m_topLeft.first),
                             max(m_topLeft.second, rhs.m_topLeft.second));
            pos_t newBottomRight(min(m_topLeft.first, rhs.m_topLeft.first),
                                 min(m_bottomRight.second,
                                     rhs.m_bottomRight.second));

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
    return m_topLeft;
}

pos_t RectArea::get_right_bottom() const
{
    return m_bottomRight;
}

pos_t RectArea::size() const
{
    return m_areaSize;
}

bool RectArea::empty() const
{
    return m_areaSize == pos_t(0, 0);
}

// Setters:
void RectArea::set_left_top(pos_t p)
{
    m_topLeft = p;
    m_areaSize = calcArea();
}

void RectArea::set_right_bottom(pos_t p)
{
    m_bottomRight = p;
    m_areaSize = calcArea();
}

void RectArea::extend_to_left_or_right(pos_t p)
{
    if (p.first < m_topLeft.first)
    {
        m_topLeft.first = p.first;
    } else if (p.first > m_bottomRight.first)
    {
        m_bottomRight.first = p.first;
    }
}

void RectArea::embrace(pos_t p)
{
    // RectArea is empty when has only one point, meaning m_topLeft = m_bottomRight
    // or when is null set
    if (!this->empty() || (this->empty() && m_atLeastOneElemExist))
    {
        point_placement p_pos = isInside(p);

        switch (p_pos)
        {
            case OVER:
                m_topLeft.second = p.second;

                extend_to_left_or_right(p);

                break;
            case UNDER:
                m_bottomRight.second = p.second;

                extend_to_left_or_right(p);

                break;
            case ON_THE_RIGHT:
            case ON_THE_LEFT:
                extend_to_left_or_right(p);

                break;
            default:
                break;
        }
    } else if (!m_atLeastOneElemExist)
    {
        m_topLeft = p;
        m_bottomRight = p;
    }

    this->calcArea();
}

// -----CROSSWORD CLASS-----

Crossword::Crossword(Crossword&& other) noexcept
        : m_words(std::move(other.m_words)),
          m_rectArea(std::move(other.m_rectArea)),
          m_letters(std::move(other.m_letters))
{
}

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

void Crossword::insert_word(const Word& word)
{
    if (!collides(word))
    {
        m_words.push_back(word);
        m_rectArea.embrace(word.get_start_position());
        m_rectArea.embrace(word.get_end_position());

        size_t x = word.getX();
        size_t y = word.getY();
        orientation_t o = word.get_orientation();

        for (int i = 0; i < word.length(); i++)
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
    }
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
        for (int i = 0; i < word.length(); i++)
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
        for (int i = 0; i < word.length(); i++)
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

int main()
{
    return 0;
}