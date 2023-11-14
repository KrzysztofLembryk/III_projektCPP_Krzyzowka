#ifndef CROSWORD_H
#define CROSSWORD_H

#include <map>
#include <vector>
#include <iostream>
#include <compare>

using pos_t = std::pair<size_t, size_t>;
using dim_t = std::pair<size_t, size_t>;

enum orientation_t : bool
{
    H = false,
    V = true
};

namespace
{
    enum point_placement
    {
        ON_THE_RIGHT,
        ON_THE_LEFT,
        OVER,
        UNDER,
        INSIDE,
        NO_AREA
    };

}

class RectArea
{
private:
    pos_t topLeft, bottomRight;
    dim_t areaSize;
    bool atLeastOneElemExist;

    dim_t calcArea();
    point_placement isInside(pos_t) const;
    void extend_to_left_or_right(pos_t);

public:
    RectArea();
    RectArea(pos_t _topLeft, pos_t _bottomRight);
    RectArea(const RectArea &);
    RectArea(RectArea &&);

    ~RectArea();

    RectArea &operator=(const RectArea &);
    RectArea &operator=(RectArea &&);

    RectArea &operator*=(const RectArea &rhs);
    const RectArea operator*(const RectArea &rhs) const;

    // Getters:
    pos_t get_left_top() const;
    pos_t get_right_bottom() const;
    dim_t size() const;
    bool empty() const;

    // Setters:
    void set_left_top(pos_t);
    void set_right_bottom(pos_t);

    // embrace - powieksza obszar zeby objal tez nowy punkt
    void embrace(pos_t);
};

class Word
{
private:
    class WordPos
    {
    private:
        pos_t pos;
        orientation_t orient;

    public:
        // Constructors:
        WordPos() = delete;

        inline WordPos(size_t x, size_t y, orientation_t o) : pos(x, y),
                                                              orient(o) {}

        inline WordPos(pos_t const &p, orientation_t const &o) : pos(p), orient(o) {}

        // Copy constructor:
        WordPos(const WordPos &w) = default;
        //: m_pos(w.m_pos), m_orient(w.m_orient) {}

        // Move Constructor:
        WordPos(WordPos &&w) = default;
        //: m_pos(move(w.m_pos)), m_orient(move(w.m_orient)) {}

        // Destructors:
        ~WordPos() = default;

        // Operators:
        WordPos &operator=(const WordPos &rhs) = default;

        WordPos &operator=(WordPos &&rhs) = default;

        auto operator<=>(WordPos const &) const = default;

        // Getters:
        pos_t getPos() const { return pos; }

        orientation_t getOrient() const { return orient; }
    };

    WordPos posAndOrient;
    std::string word;

public:
    // Constructors:
    Word() = delete;

    Word(size_t x, size_t y, orientation_t orient, std::string const &_word);

    // Copy constructor:
    Word(const Word &other);
    //: m_posAndOrient(other.m_posAndOrient),
    //    m_word(other.m_word) {}

    // Move constructor:
    Word(Word &&other) noexcept;
    //: m_posAndOrient(move(other.m_posAndOrient)),
    //    m_word(move(other.m_word)) {}

    // Destructors:
    ~Word();

    // Operators:
    Word &operator=(const Word &rhs);

    Word &operator=(Word &&rhs) noexcept;

    bool operator==(const Word &other) const;

    std::weak_ordering operator<=>(const Word &other) const;

    // Getters:
    pos_t get_start_position() const;

    pos_t get_end_position() const;

    orientation_t get_orientation() const;

    char at(size_t idx) const;

    size_t length() const;

    RectArea rect_area() const;

    inline size_t getX() const
    {
        return posAndOrient.getPos().first;
    }

    inline size_t getY() const
    {
        return posAndOrient.getPos().second;
    }
};

class Crossword
{
private:
    struct Letter
    {
        char character;
        orientation_t orientation;
        bool intersection;
    };

    std::vector<Word> m_words;
    RectArea m_rectArea;
    std::map<pos_t, Letter> m_letters;

public:
    // Constructors:
    Crossword() = delete;

    Crossword(const Crossword &other) = default;

    Crossword(Crossword &&other) noexcept;

    explicit Crossword(const Word &word);

    Crossword(const Word &firstWord, const std::vector<Word> &words);

    // Destructors:
    inline ~Crossword() = default;

    bool insert_word(const Word &word);

    // Getters:
    dim_t size() const;

    dim_t word_count() const;

    // Operators:
    Crossword &operator=(const Crossword &other) = default;

    Crossword &operator=(Crossword &&other) noexcept;

    Crossword operator+(const Crossword &other) const;

    Crossword operator+=(const Crossword &other) const;

    friend std::ostream &
    operator<<(std::ostream &out, const Crossword &crossword);

private:
    bool collides(const Word &word) const;
};

inline const RectArea DEFAULT_EMPTY_RECT_AREA(pos_t(1, 1), pos_t(0, 0));
inline char DEFAULT_CHAR = '?';
inline char CROSSWORD_BACKGROUND = '.';
inline std::string DEFAULT_WORD = "?";

#endif // CROSSWORD_H
