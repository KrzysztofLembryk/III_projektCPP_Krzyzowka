#ifndef III_PROJEKTCPP_KRZYZOWKA_CROSSWORD_H
#define III_PROJEKTCPP_KRZYZOWKA_CROSSWORD_H

#include <vector>
#include <iostream>

using pos_t = std::pair<size_t, size_t>;
using dim_t = std::pair<size_t, size_t>;

enum orientation_t : bool
{
    H = true,
    V = false
};

namespace
{
    class WordPos
    {
    private:
        pos_t pos;
        orientation_t orient;

    public:
        // Constructors:
        WordPos() = delete;
        WordPos(size_t x, size_t y, orientation_t _orient)
            : pos(x, y), orient(_orient)
        {
        }
        WordPos(pos_t p, orientation_t o)
            : pos(p), orient(o)
        {
        }

        // Destructors:
        ~WordPos() = default;

        // Getters:
        // we return const reference not to allow to change these values
        pos_t const &getPos() { return pos; }
        orientation_t const &getOrient() { return orient; }

    };
}

class Word
{
private:
    WordPos posAndOrient;
    std::string word;

public:
    // Constructors:
    Word() = delete;
    Word(size_t x, size_t y, orientation_t orient, const std::string& _word);

    // Destructors:
    ~Word() = default;

    // Getters:
    pos_t const &get_start_position();
    pos_t get_end_position();
    orientation_t get_orientation();
    char at(size_t idx);
    size_t length();

};

class RectArea
{

};

class Crossword
{
private:
    std::vector<Word> m_words;
    RectArea m_rectArea;

public:
    // Constructors:
    Crossword() = delete;
    Crossword(const Word& word);
    Crossword(const Word& firstWord, const std::vector<Word>& words);
    Crossword(Word&& word) noexcept;
    Crossword(Word&& firstWord, std::vector<Word>&& words) noexcept;

    // Destructors:
    ~Crossword() = default;

    void insert_word(const Word& word);

    // Getters:
    dim_t size() const;
    dim_t word_count() const;

    // Operators:
    Crossword& operator=(const Crossword& other);
    Crossword& operator=(Crossword&& other) noexcept ;
    Crossword operator+(const Crossword& other);
    Crossword operator+=(const Crossword& crossword);
    friend std::ostream& operator<<(std::ostream& out, const Crossword& crossword);

private:
    bool colides(const Word& word);

};

#endif //III_PROJEKTCPP_KRZYZOWKA_CROSSWORD_H
