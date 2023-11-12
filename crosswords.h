#ifndef III_PROJEKTCPP_KRZYZOWKA_CROSSWORD_H
#define III_PROJEKTCPP_KRZYZOWKA_CROSSWORD_H

#include <map>
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
        pos_t m_pos;
        orientation_t m_orient;

    public:
        // Constructors:
        inline WordPos() = delete;
        inline WordPos(size_t x, size_t y, orientation_t _orient)
            : m_pos(x, y), m_orient(_orient)
        {
        }
        inline WordPos(pos_t p, orientation_t o)
            : m_pos(p), m_orient(o)
        {
        }

        // Destructors:
        inline ~WordPos() = default;

        // Getters:
        inline pos_t getPos() const { return m_pos; }
        inline orientation_t getOrient() const { return m_orient; }

    };
}

class Word
{
private:
    WordPos m_posAndOrient;
    std::string m_word;

public:
    // Constructors:
    Word() = delete;
    Word(size_t x, size_t y, orientation_t orient, const std::string& word);

    // Destructors:
    inline ~Word() = default;

    // Getters:
    pos_t get_start_position() const;
    pos_t get_end_position() const;
    orientation_t get_orientation() const;
    char at(size_t idx) const;
    size_t length() const;
    size_t getX() const;
    size_t getY() const;

};

class RectArea
{
private:
    pos_t m_leftTop;
    pos_t m_rightBottom;

public:
    // Constructors:
    RectArea() = delete;
    RectArea(pos_t leftTop, pos_t rightBottom);

    // Destructors:
    inline ~RectArea() = default;

    // Getters:
    pos_t get_left_top() const;
    pos_t get_right_bottom() const;
};

class Crossword
{
private:
    std::vector<Word> m_words;
    RectArea m_rectArea;
    std::map<pos_t, std::pair<orientation_t, char>> m_points;

public:
    // Constructors:
    Crossword() = delete;
    Crossword(const Crossword& other) = default;
    Crossword(Crossword&& other) noexcept;
    explicit Crossword(const Word& word);
    Crossword(const Word& firstWord, const std::vector<Word>& words);

    // Destructors:
    inline ~Crossword() = default;

    void insert_word(const Word& word);

    // Getters:
    dim_t size() const;
    dim_t word_count() const;

    // Operators:
    Crossword& operator=(const Crossword& other);
    Crossword& operator=(Crossword&& other) noexcept;
    Crossword operator+(const Crossword& other) const;
    Crossword operator+=(const Crossword& other) const;
    friend std::ostream& operator<<(std::ostream& out, const Crossword& crossword);

private:
    bool collides(const Word& word) const;

};

const RectArea DEFAULT_EMPTY_RECT_AREA(pos_t(0, 0), pos_t(0, 0));
char DEFAULT_CHAR = '?';
char CROSSWORD_BACKGROUND = '.';
std::string DEFAULT_WORD = "?";

#endif //III_PROJEKTCPP_KRZYZOWKA_CROSSWORD_H
