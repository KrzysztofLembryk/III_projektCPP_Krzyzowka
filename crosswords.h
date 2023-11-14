#ifndef III_PROJEKTCPP_KRZYZOWKA_CROSSWORD_H
#define III_PROJEKTCPP_KRZYZOWKA_CROSSWORD_H

#include <map>
#include <vector>
#include <iostream>
#include <compare>

using pos_t = std::pair<size_t, size_t>;
using dim_t = std::pair<size_t, size_t>;

enum orientation_t : bool
{
    H = true,
    V = false
};

namespace
{
    enum point_placement
    {
        ON_THE_RIGHT, ON_THE_LEFT, OVER, UNDER, INSIDE, NO_AREA
    };

    class WordPos
    {
    private:
        pos_t m_pos;
        orientation_t m_orient;

    public:
        // Constructors:
        WordPos() = delete;

        inline WordPos(size_t x, size_t y, orientation_t _orient)
                : m_pos(x, y), m_orient(_orient)
        {
        }

        inline WordPos(pos_t const& p, orientation_t const& o)
                : m_pos(p), m_orient(o)
        {
        }

        // Copy constructor:
        WordPos(const WordPos& w) = default;
        //: m_pos(w.m_pos), m_orient(w.m_orient) {}

        // Move Constructor:
        WordPos(WordPos&& w) = default;
        //: m_pos(move(w.m_pos)), m_orient(move(w.m_orient)) {}

        // Destructors:
        ~WordPos() = default;

        // Operators:
        WordPos& operator=(const WordPos& rhs) = default;

        WordPos& operator=(WordPos&& rhs) = default;

        std::strong_ordering operator<=>(const WordPos&) const = default;

        // Getters:
        inline pos_t getPos() const
        { return m_pos; }

        inline orientation_t getOrient() const
        { return m_orient; }
    };

    struct Letter
    {
        char character;
        orientation_t orientation;
        bool intersection;
    };
}

class RectArea
{
private:
    pos_t m_topLeft, m_bottomRight;
    pos_t m_areaSize;
    bool m_atLeastOneElemExist;

    pos_t calcArea();

    point_placement isInside(pos_t) const;

    void extend_to_left_or_right(pos_t);

public:
    RectArea() = delete;

    RectArea(pos_t _topLeft, pos_t _bottomRight);

    RectArea(const RectArea&);

    RectArea(RectArea&&) noexcept;

    ~RectArea();

    RectArea& operator=(const RectArea&);

    RectArea& operator=(RectArea&&) noexcept;

    RectArea& operator*=(const RectArea& rhs);

    RectArea operator*(const RectArea& rhs) const;

    // Getters:
    pos_t get_left_top() const;

    pos_t get_right_bottom() const;

    pos_t size() const;

    bool empty() const;

    // Setters:
    void set_left_top(pos_t);

    void set_right_bottom(pos_t);

    void embrace(pos_t);
};

class Word
{
private:
    WordPos m_posAndOrient;
    std::string m_word;

public:
    // Constructors:
    Word() = delete;

    Word(size_t x, size_t y, orientation_t orient, std::string const& _word);

    // Copy constructor:
    Word(const Word& other);
    //: m_posAndOrient(other.m_posAndOrient),
    //    m_word(other.m_word) {}

    // Move constructor:
    Word(Word&& other) noexcept;
    //: m_posAndOrient(move(other.m_posAndOrient)),
    //    m_word(move(other.m_word)) {}

    // Destructors:
    ~Word();

    // Operators:
    Word& operator=(const Word& rhs);

    Word& operator=(Word&& rhs) noexcept;

    bool operator==(const Word& other) const;

    auto operator<=>(const Word& other) const;

    // Getters:
    pos_t get_start_position() const;

    pos_t get_end_position() const;

    orientation_t get_orientation() const;

    char at(size_t idx) const;

    size_t length() const;

    RectArea rect_area() const;

    inline size_t getX() const
    { return m_posAndOrient.getPos().first; }

    inline size_t getY() const
    { return m_posAndOrient.getPos().second; }
};

class Crossword
{
private:
    std::vector<Word> m_words;
    RectArea m_rectArea;
    std::map<pos_t, Letter> m_letters;

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
    Crossword& operator=(const Crossword& other) = default;

    Crossword& operator=(Crossword&& other) noexcept;

    Crossword operator+(const Crossword& other) const;

    Crossword operator+=(const Crossword& other) const;

    friend std::ostream&
    operator<<(std::ostream& out, const Crossword& crossword);

private:
    bool collides(const Word& word) const;

};

inline const RectArea DEFAULT_EMPTY_RECT_AREA(pos_t(1, 1), pos_t(0, 0));
inline char DEFAULT_CHAR = '?';
inline char CROSSWORD_BACKGROUND = '.';
inline std::string DEFAULT_WORD = "?";

#endif //III_PROJEKTCPP_KRZYZOWKA_CROSSWORD_H
