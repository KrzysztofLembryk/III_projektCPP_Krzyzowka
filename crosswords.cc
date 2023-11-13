#include "crosswords.h"

#define LETTER_EXISTS(y, x) m_letters.count(pos_t(y, x)) != 0
#define LETTER(y, x) m_letters.at(pos_t(y, x))

Crossword::Crossword(Crossword&& other) noexcept
    : m_words(std::move(other.m_words)),
      m_rectArea(std::exchange(other.m_rectArea, DEFAULT_EMPTY_RECT_AREA)),
      m_letters(std::move(other.m_letters))
{
}

Crossword &Crossword::operator=(const Crossword &other)
{
    m_words = other.m_words;
    m_rectArea = other.m_rectArea;
    m_letters = other.m_letters;
    return *this;
}

Crossword& Crossword::operator=(Crossword&& other) noexcept
{
    m_words = std::move(other.m_words);
    m_rectArea = std::exchange(other.m_rectArea, DEFAULT_EMPTY_RECT_AREA);
    m_letters = std::move(other.m_letters);
    return *this;
}

Crossword::Crossword(const Word &word)
    : m_rectArea(RectArea(word.get_start_position(), word.get_end_position()))
{
    insert_word(word);
}

Crossword::Crossword(const Word& firstWord, const std::vector<Word>& words)
    : m_rectArea(RectArea(firstWord.get_start_position(), firstWord.get_end_position()))
{
    insert_word(firstWord);
    for(const auto& word : words)
    {
        insert_word(word);
    }
}

void Crossword::insert_word(const Word& word)
{
    if(!collides(word))
    {
        m_words.push_back(word);
        m_rectArea.embrace(word.get_start_position());
        m_rectArea.embrace(word.get_end_position());

        size_t x = word.getX();
        size_t y = word.getY();
        orientation_t o = word.get_orientation();

        for(int i = 0; i < word.length(); i++)
        {
            if(o == H)
            {
                m_letters[pos_t(y, x + i)] = {
                        word.at(i),
                        o,
                        m_letters.count(pos_t(y, x + i)) != 0};
            }
            else // (o == V)
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
    for(const auto& word : m_words)
    {
        if(word.get_orientation() == H) { result.first++; }
        else { result.second++; }
    }
    return result;
}

// syf
bool Crossword::collides(const Word& word) const
{
    size_t  x = word.getX();
    size_t  y = word.getY();
    orientation_t o = word.get_orientation();
    Letter letter{};

    if(o == H)
    {
        for(int i = 0; i < word.length(); i++)
        {
            if(i != word.length() - 1 && x + i == SIZE_MAX) { return true; }
            if(LETTER_EXISTS(y, x + i))
            {
                letter = LETTER(y, x + i);
                if(letter.character == word.at(i) && letter.orientation != o && !letter.intersection)
                {
                    if(y > 0 && LETTER_EXISTS(y - 1, x + i) && LETTER(y - 1, x + i).intersection)
                    {
                        return true;
                    }
                    if(y < SIZE_MAX && LETTER_EXISTS(y + 1, x + i) && LETTER(y + 1, x + i).intersection)
                    {
                        return true;
                    }
                }
                else { return true; }
            }
            else // (!(LETTER_EXISTS(y, x + i)))
            {
                if(y > 0 && LETTER_EXISTS(y - 1, x + i)) { return true; }
                if(y < SIZE_MAX && LETTER_EXISTS(y + 1, x + i)) { return true; }
            }
        }
        if(x > 0 && LETTER_EXISTS(y, x - 1)) { return true; }
        if(x < SIZE_MAX - word.length() + 1 && LETTER_EXISTS(y, x + word.length())) { return true; }
    }
    else // (o == V)
    {
        for (int i = 0; i < word.length(); i++)
        {
            if (i != word.length() - 1 && y + i == SIZE_MAX) { return true; }
            if (LETTER_EXISTS(y + i, x))
            {
                letter = LETTER(y + i, x);
                if (letter.character == word.at(i) && letter.orientation != o && !letter.intersection)
                {
                    if (x > 0 && LETTER_EXISTS(y + i, x - 1) && LETTER(y + i, x - 1).intersection)
                    {
                        return true;
                    }
                    if (x < SIZE_MAX && LETTER_EXISTS(y + i, x + 1) && LETTER(y + i, x + 1).intersection)
                    {
                        return true;
                    }
                }
                else { return true; }
            }
            else // (!(LETTER_EXISTS(y + i, x)))
            {
                if (x > 0 && LETTER_EXISTS(y + i, x - 1)) { return true; }
                if (x < SIZE_MAX && LETTER_EXISTS(y + i, x + 1)) { return true; }
            }
        }
        if (y > 0 && LETTER_EXISTS(y - 1, x)) { return true; }
        if (y < SIZE_MAX - word.length() + 1 && LETTER_EXISTS(y + word.length(), x)) { return true; }
    }
    return false;
}

Crossword Crossword::operator+(const Crossword& other) const
{
    Crossword crossword(m_words[0], m_words); // The first m_word will collide, so it won't be added.
    for(const auto& word : other.m_words)
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
    if((c >= 'a' && c <= 'z') || (c >= 'A' && c <='Z'))
    {
        out << toupper(c);
    }
    else { out << DEFAULT_CHAR; }
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

    for(size_t j = start.second; j <= end.second; j++)
    {
        for(size_t i = start.first; i < end.first; i++)
        {
            if(pointsItr != crossword.m_letters.cend() && (*pointsItr).first == pos_t(j, i))
            {
                writeCrosswordLetter(out, (*pointsItr).second.character);
                pointsItr++;
            }
            else { out << CROSSWORD_BACKGROUND; }
            out << ' ';
        }
        out << CROSSWORD_BACKGROUND;
        out << std::endl;
    }
}