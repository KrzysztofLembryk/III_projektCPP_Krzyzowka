#include "crosswords.h"

Crossword::Crossword(const Crossword& other)
{
    *this = other;
}

Crossword::Crossword(Crossword&& other) noexcept
{
    *this = other;
}

Crossword& Crossword::operator=(Crossword&& other) noexcept
{
    m_words = std::move(other.m_words);
    m_rectArea = std::move(other.m_rectArea);
    return *this;
}

Crossword::Crossword(const Word &word)
{
    m_words.push_back(word);
    //rectAreaInit
}

Crossword::Crossword(const Word& firstWord, const std::vector<Word>& words)
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

        size_t  x = word.getX();
        size_t  y = word.getY();
        orientation_t o = word.get_orientation();

        for(int i = 0; i < word.length(); i++)
        {
            if(o == H)
            {
                m_points[{y, x + i}] = {o, word.at(i)};
            }
            else // (o == V)
            {
                m_points[{y + 1, x}] = {o, word.at(i)};
            }
        }
    }
    //rectAreaUpdate
}

dim_t Crossword::size() const
{

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

bool Crossword::collides(const Word& word) const
{
    size_t  x = word.getX();
    size_t  y = word.getY();
    orientation_t o = word.get_orientation();

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
                // stuff
            }
        }
    }
    else // (o == V)
    {
        // more stuff
    }
    return false
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

std::ostream& operator<<(std::ostream& out, const Crossword& crossword)
{

}


