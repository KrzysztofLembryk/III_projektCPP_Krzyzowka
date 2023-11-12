#include "crosswords.h"

Crossword::Crossword(Crossword&& other) noexcept
    : m_rectArea(other.m_rectArea),
    m_words(std::move(other.m_words)),
    m_points(std::move(other.m_points))
{
}

Crossword &Crossword::operator=(const Crossword &other)
{
    m_words = other.m_words;
    m_rectArea = other.m_rectArea;
    return *this;
}

Crossword& Crossword::operator=(Crossword&& other) noexcept
{
    m_words = std::move(other.m_words);
    m_rectArea = other.m_rectArea;
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
                m_points[pos_t(y, x + i)] = {o, word.at(i)};
            }
            else // (o == V)
            {
                m_points[pos_t(y + i, x)] = {o, word.at(i)};
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
    std::pair<orientation_t, char> temp;

    if(o == H)
    {
        for(int i = 0; i < word.length(); i++)
        {
            if(m_points.count({y + 1, x + i}) != 0)
            {
                return true;
            }
            if(m_points.count({y - 1, x + i}) != 0)
            {
                return true;
            }
            if(m_points.count({y, x + i}) != 0)
            {
                temp = m_points.at({y, x + i});
                if(temp.first == o || temp.second != word.at(i))
                {
                    return true;
                }
            }
        }
        if(m_points.count({y, x - 1}) != 0 ||
           m_points.count({y, x + word.length()}) != 0) {
            return true;
        }
    }
    else // (o == V)
    {
        for(int i = 0; i < word.length(); i++)
        {
            if(m_points.count({y + i, x + 1}) != 0)
            {
                return true;
            }
            if(m_points.count({y + i, x - 1}) != 0)
            {
                return true;
            }
            if(m_points.count({y + i, x}) != 0)
            {
                temp = m_points.at({y + i, x});
                if(temp.first == o || temp.second != word.at(i))
                {
                    return true;
                }
            }
        }
        if(m_points.count({y - 1, x}) != 0 ||
           m_points.count({y + word.length(), x}) != 0) {
            return true;
        }
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

void print(std::ostream& out, char c)
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
    auto pointsItr = crossword.m_points.cbegin();

    for(size_t j = start.second; j <= end.second; j++)
    {
        for(size_t i = start.first; i < end.first; i++)
        {
            if(pointsItr != crossword.m_points.cend() && (*pointsItr).first == pos_t(j, i))
            {
                print(out, (*pointsItr).second.second);
                pointsItr++;
            }
            else { out << CROSSWORD_BACKGROUND; }
            out << ' ';
        }
        out << CROSSWORD_BACKGROUND;
        out << std::endl;
    }
}