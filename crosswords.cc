#include "crosswords.h"

Crossword::Crossword(const Word &word)
{
    m_words.push_back(word);
    //rectAreaInit
}

Crossword::Crossword(const Word& firstWord, const std::vector<Word>& words)
{

}

Crossword::Crossword(Word&& word) noexcept
{

}

Crossword::Crossword(Word&& firstWord, std::vector<Word>&& words) noexcept
{

}

void Crossword::insert_word(const Word& word)
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

Crossword& Crossword::operator=(const Crossword &other)
{

}

Crossword& Crossword::operator=(Crossword &&other) noexcept
{

}

