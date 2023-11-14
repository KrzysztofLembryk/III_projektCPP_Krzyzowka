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
    enum point_placement { ON_THE_RIGHT, ON_THE_LEFT, OVER, UNDER, INSIDE, NO_AREA};

    class WordPos
    {
    private:
        pos_t pos;
        orientation_t orient;

    public:
        // Constructors:
        WordPos() = delete;

        WordPos(size_t x, size_t y, orientation_t _orient) : pos(x, y),
                                                             orient(_orient) {}
        WordPos(pos_t const &p, orientation_t const &o) : pos(p), orient(o) {}

        // Copy constructor:
        WordPos(const WordPos &w) = default; //: pos(w.pos), orient(w.orient) {}

        // Move Constructor:
        WordPos(WordPos &&w) = default; //: pos(move(w.pos)), orient(move(w.orient)) {}

        // Destructors:
        ~WordPos() = default;

        // Operators:
        WordPos &operator=(const WordPos &rhs) = default;
        WordPos &operator=(WordPos &&rhs) = default;
        auto operator<=>(const WordPos &) const = default;

        // Getters:
        // we return const reference not to allow to change these values
        pos_t getPos() const;

        orientation_t getOrient() const;
    };
}

class Word
{
private:
    WordPos posAndOrient;
    std::string word;

public:
    // Constructors:
    Word();

    Word(size_t x, size_t y, orientation_t orient, std::string const &_word);

    // Copy constructor:
    Word(const Word &other); //: posAndOrient(other.posAndOrient),
    //    word(other.word) {}

    // Move construcotr:
    Word(Word &&other); //: posAndOrient(move(other.posAndOrient)),
    //    word(move(other.word)) {}

    // Destructors:
    ~Word();

    // Operators:
    Word &operator=(const Word &rhs);
    Word &operator=(Word &&rhs);

    bool operator==(const Word &other) const;

    auto operator<=>(const Word &other) const;

    // Getters:
    pos_t get_start_position() const;

    pos_t get_end_position() const;

    orientation_t get_orientation() const;

    char at(size_t idx) const;

    size_t length() const;

    RectArea rect_area() const;
};

class RectArea
{
private:
    pos_t topLeft, bottomRight;
    pos_t areaSize;
    bool atLeastOneElemExist;

    pos_t calcArea();
    point_placement isInside(pos_t) const;
    void extend_to_left_or_right(pos_t);
public:
    RectArea();
    RectArea(pos_t _topLeft, pos_t _bottomRight);
    RectArea(const RectArea &);
    RectArea(RectArea&&);

    ~RectArea();

    RectArea &operator=(const RectArea &);
    RectArea &operator=(RectArea &&);

    RectArea &operator*=(const RectArea &rhs);
    const RectArea operator*(const RectArea &rhs) const;

    // Getters:
    pos_t get_left_top() const;
    pos_t get_right_bottom() const;
    pos_t size() const;
    bool empty() const;

    // Setters:
    void set_left_top(pos_t);
    void set_right_bottom(pos_t);

    // embrace - powieksza obszar zeby objal tez nowy punkt 
    void embrace(pos_t);
};

class Crossword
{
private:
    std::vector<Word> m_words;
    RectArea m_rectArea;

public:
    // Constructors:
    Crossword() = delete;
    Crossword(const Word &word);
    Crossword(const Word &firstWord, const std::vector<Word> &words);
    Crossword(Word &&word) noexcept;
    Crossword(Word &&firstWord, std::vector<Word> &&words) noexcept;

    // Destructors:
    ~Crossword() = default;

    void insert_word(const Word &word);

    // Getters:
    dim_t size() const;
    dim_t word_count() const;

    // Operators:
    Crossword &operator=(const Crossword &other);
    Crossword &operator=(Crossword &&other) noexcept;
    Crossword operator+(const Crossword &other);
    Crossword operator+=(const Crossword &crossword);
    friend std::ostream &operator<<(std::ostream &out, const Crossword &crossword);

private:
    bool colides(const Word &word);
};

#endif // III_PROJEKTCPP_KRZYZOWKA_CROSSWORD_H
