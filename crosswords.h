#ifndef CROSSWORDS_H
#define CROSSWORDS_H

#include <map>
#include <vector>
#include <iostream>
#include <compare>

using pos_t = std::pair<size_t, size_t>;
using dim_t = std::pair<size_t, size_t>;

/**
 * Enum class that stores allowed orientations: H - horizontal, V - vertical.
 * H = false, because we want H < V inequality to be true.
*/
enum orientation_t : bool
{
    H = false,
    V = true
};

namespace
{   
    /**
     * This enum helps to determine where given point is regarding rectangle 
     * in RectArea::embrace() method.
    */
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

/**
 * RectArea class stores coordinates of rectangle; its topLeft and bottomRight 
 * vertices. Apart from obvious methods (like getters, setters), this class
 * allows to enlarge rectangle to accommodate new point by embrace() method 
 * and to find intersection of rectangles using * or *= operators.
*/
class RectArea
{
private:
    pos_t topLeft, bottomRight;
    dim_t areaSize;

    /**
     * Helping private functions:
     * - calcArea() returns pair (width, height) pf rectangle
     * - isInside() checks where given point is regarding our rectangle 
     * - extend_to_left_or_right() extends width of rectangle to accommodate 
     *   given point
    */
    dim_t calcArea() const;
    point_placement isInside(pos_t) const;
    void extend_to_left_or_right(pos_t);
public:
    // Constructors:
    RectArea() = delete;

    RectArea(pos_t _topLeft, pos_t _bottomRight);

    // Copy Constructor:
    RectArea(const RectArea&) = default;

    // Move Constructor:
    RectArea(RectArea&&) noexcept = default;

    // Destructor:
    ~RectArea() = default;

    // Operators:
    RectArea& operator=(const RectArea&) = default;

    RectArea& operator=(RectArea&&) noexcept = default;

    RectArea& operator*=(const RectArea& rhs);

    RectArea operator*(const RectArea& rhs) const;

    // Getters:
    pos_t get_left_top() const;

    pos_t get_right_bottom() const;

    dim_t size() const;

    bool empty() const;

    // Setters:
    void set_left_top(pos_t);

    void set_right_bottom(pos_t);

    void embrace(pos_t);
};

/**
 * Class Word stores given string and its starting and ending position in
 * crossword and also its orientation in crossword.
*/ 
class Word
{
private:
    /**
     * Helping class WordPos stores starting position of word and its
     * orientation. Thanks to this class comparing Words via <=> can be done
     * easily using default operators. Since pos_t = pair<size_t, size_t> and 
     * orientation_t = enum bool know how to compare itselves to others,
     * WordPos by default knows that too. Thus comparing two Word objects is
     * equivalent to comparing their WordPos variables.
    */
    class WordPos
    {
    private:
        pos_t pos;
        orientation_t orient;

    public:
        // Constructors:
        WordPos() = delete;

        WordPos(size_t x, size_t y, orientation_t o) : pos(x, y), orient(o) {}

        WordPos(pos_t const& p, orientation_t const& o) : pos(p), orient(o) {}

        // Copy constructor:
        WordPos(const WordPos& w) = default;

        // Move Constructor:
        WordPos(WordPos&& w) = default;

        // Destructor:
        ~WordPos() = default;

        // Operators:
        WordPos& operator=(const WordPos& rhs) = default;

        WordPos& operator=(WordPos&& rhs) = default;

        auto operator<=>(WordPos const&) const = default;

        // Getters:
        pos_t getPos() const {return pos;}

        orientation_t getOrient() const {return orient;}
    };

    WordPos posAndOrient;
    std::string word;
    pos_t endPos;

    /**
     * Function checks if word fits in maximal crossword size:
     * = (size_t)(-1) x (size_t)(-1). If after adding word length to
     * starting position of word, we go over maximum size of crossword, 
     * the word is trimmed to fit in crossword. 
     * Function returns end position of word.
    */
    pos_t checkOutOfBoundWord();

public:
    // Constructors:
    Word() = delete;

    Word(size_t x, size_t y, orientation_t orient, std::string const& _word);

    // Copy constructor:
    Word(const Word& other) = default;

    // Move constructor:
    Word(Word&& other) noexcept = default;

    // Destructor:
    ~Word() = default;

    // Operators:
    Word& operator=(const Word& rhs) = default;

    Word& operator=(Word&& rhs) noexcept = default;

    bool operator==(const Word& other) const;

    std::weak_ordering operator<=>(const Word& other) const;

    // Getters:
    pos_t get_start_position() const;

    pos_t get_end_position() const;

    orientation_t get_orientation() const;

    // Method returns character which is at given position in word.
    char at(size_t idx) const;

    size_t length() const;
    
    // Method returns minimal rectArea that contains word, method takes into
    // account word orientation.
    RectArea rect_area() const;

    size_t getX() const
    {
        return posAndOrient.getPos().first;
    }

    size_t getY() const
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

    Crossword(const Crossword& other) = default;

    Crossword(Crossword&& other) noexcept;

    explicit Crossword(const Word& word);

    Crossword(const Word& firstWord, const std::vector<Word>& words);

    // Destructors:
    inline ~Crossword() = default;

    bool insert_word(const Word& word);

    // Getters:
    dim_t size() const;

    dim_t word_count() const;

    // Operators:
    Crossword& operator=(const Crossword& other) = default;

    Crossword& operator=(Crossword&& other) noexcept;

    Crossword& operator+(const Crossword& other);

    Crossword& operator+=(const Crossword& other);

    friend std::ostream&
    operator<<(std::ostream& out, const Crossword& crossword);

private:
    bool collides(const Word& word) const;
};

inline const RectArea DEFAULT_EMPTY_RECT_AREA(pos_t(1, 1), pos_t(0, 0));
inline char DEFAULT_CHAR = '?';
inline char CROSSWORD_BACKGROUND = '.';
inline std::string DEFAULT_WORD = "?";

#endif // CROSSWORDS_H