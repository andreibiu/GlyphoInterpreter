/// ============================
/// @author Andrei Biu - Pislaru
/// @brief Implementation for the Integer class
/// ============================

#ifndef INTEGER_HPP
#define INTEGER_HPP

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <string>
#include <sstream>

#include "Utils.hpp"
#include "ArithmeticOps.hpp"

/**
 * @class Integer
 * @brief Implementation of a big integer with its dimension limited
 *        (theoretically) only by available memory.
 * @details The implementation supports input and output string representations
 *          with bases in the range 2 - 36 using the character set [0-9A-Z].
 *          Internally the number is stored as an array of digits in the base
 *          2^mbw, with 'mbw' being the bit width (size on bits) of the maximum
 *          biggest hardware - supported unsigned integral type to maximize the
 *          use of hardware instructions    
 */
class Integer
{    
    /// @defgroup Static Methods
    private:
        static bool GreaterAbs(const Integer& left, const Integer& right);
        static void Add(Integer& left, const Integer& right);
        static void Add(Integer& result, const Integer& left, const Integer& right);
        static void Subtract(Integer& result, const Integer& left, const Integer& right);
        static void Multiply(Integer& result, const Integer& left, const Integer& right);
        static void Multiply(Integer& integer, byte value);
        static byte Divide(Integer& integer, byte value);
    
    /// @defgroup Fields
    private:
        bool sign;
        xword width;
        xword* magnitude;

    /// @defgroup Private Constructors
    private:
        Integer();

    /// @defgroup Public Constructors
    public:
        Integer(const Integer& integer);
        Integer(Integer&& integer);
        Integer(xword value, bool sign = 0);
        Integer(xword* magnitude, xword width, bool sign = 0);
        Integer(std::string numberString, byte base = 10);

    /// @defgroup Destructor
    public:
        ~Integer();

    /// @defgroup Operators
    public:
        Integer& operator = (const Integer& integer);
        Integer& operator = (Integer&& integer);
        bool operator == (const Integer& integer) const;
        bool operator != (const Integer& integer) const;
        bool operator < (const Integer& integer) const;
        bool operator > (const Integer& integer) const;
        bool operator <= (const Integer& integer) const;
        bool operator >= (const Integer& integer) const;
        Integer operator + (const Integer& integer) const;
        Integer operator - (const Integer& integer) const;
        Integer operator * (const Integer& integer) const;
        Integer operator + () const;
        Integer operator - () const;

    /// @defgroup Methods
    public:
        void Negate();
        bool IsZero() const;
        bool IsOne() const;
        bool IsNegative() const;
        bool IsPositive() const;
        std::string ToString(byte base = 10) const;
};

/// @ingroup Static Methods
/// @{

INLINE bool Integer::GreaterAbs(const Integer& left, const Integer& right)
{
    if (left.width == right.width)
    {
        xword i;
        for (i = left.width - 1; i > 0 && left.magnitude[i] == right.magnitude[i]; --i);
        return left.magnitude[i] > right.magnitude[i];
    }
    
    return left.width > right.width;
}

INLINE void Integer::Add(Integer& left, const Integer& right)
{
    xword carry = 0, index = 0;

    while (index < left.width)
    {
        left.magnitude[index] = carry + left.magnitude[index] + right.magnitude[index];
        carry = left.magnitude[index] < right.magnitude[index++]; 
    }
    while (index < right.width)
    {
        left.magnitude[index] = carry + right.magnitude[index];
        carry = left.magnitude[index] < right.magnitude[index++]; 
    }
    left.magnitude[index] = carry;
    left.width += (bool) carry;
}

INLINE void Integer::Add(Integer& result, const Integer& left, const Integer& right)
{
    if (right.width == 1 && right.magnitude[0] == 0)
    {
        result.width = left.width;
        result.magnitude = (xword*) malloc(left.width * sizeof(xword));
        memcpy(result.magnitude, left.magnitude, left.width * sizeof(xword));
    }
    else
    {
        xword carry = 0, index = 0;

        result.width = left.width;
        result.magnitude = (xword*) malloc((left.width + 1) * sizeof(xword));
        while (index < right.width)
        {
            result.magnitude[index] = carry + right.magnitude[index] + left.magnitude[index];
            carry = result.magnitude[index] < left.magnitude[index++];
        }
        while (index < left.width)
        {
            result.magnitude[index] = carry + left.magnitude[index];
            carry = result.magnitude[index] < left.magnitude[index++]; 
        }
        result.magnitude[index] = carry;
        result.width += (bool) carry;
    }
}

INLINE void Integer::Subtract(Integer& result, const Integer& left, const Integer& right)
{
    if (right.width == 1 && right.magnitude[0] == 0)
    {
        result.width = left.width;
        result.magnitude = (xword*) malloc(left.width * sizeof(xword));
        memcpy(result.magnitude, left.magnitude, left.width * sizeof(xword));
    }
    else
    {
        xword borrow = 0, index = 0;

        result.magnitude = (xword*) malloc(left.width * sizeof(xword));
        result.width = left.width;
        while (index < right.width)
        {
            result.magnitude[index] = left.magnitude[index] - right.magnitude[index] - borrow;
            borrow = result.magnitude[index] > left.magnitude[index];
            result.width = result.magnitude[index++] ? index : result.width;
        }
        while (index < left.width)
        {
            result.magnitude[index] = left.magnitude[index] - borrow;
            borrow = result.magnitude[index] > left.magnitude[index];
            result.width = result.magnitude[index++] ? index : result.width;
        }
    }
}

INLINE void Integer::Multiply(Integer& result, const Integer& left, const Integer& right)
{
	if (right.width == 1)
    {
        if (right.magnitude[0] == 0)
        {
            result.width = 1;
            result.magnitude = (xword*) malloc(sizeof(xword));
            result.magnitude[0] = 0;
            return;
        }
        else if (right.magnitude[0] == 1)
        {
            result.width = left.width;
            result.magnitude = (xword*) malloc(left.width + sizeof(xword));
            memcpy(result.magnitude, left.magnitude, left.width * sizeof(xword));
        }
        else
        {
            xword index = 0;
            std::pair<xword, xword> temp = std::make_pair(0, 0);

            result.width = left.width + 1;
            result.magnitude = (xword*) malloc(result.width * sizeof(xword));
            while (index < left.width)
            {
                result.magnitude[index] = temp.first;
                temp = ArithmeticOps::Multiply(left.magnitude[index], right.magnitude[0]);
                result.magnitude[index] += temp.second;
                temp.first += result.magnitude[index++] < temp.second; 
            }
            result.magnitude[index] = temp.first;
            result.width -= temp.first == 0;
        }
    }
    else
    {
        xword index0 = 0, index1, index2, carry;
        std::pair<xword, xword> temp;

        result.width = left.width + right.width;
        result.magnitude = (xword*) calloc(result.width, sizeof(xword));
        while (index0 < right.width)
        {
            index1 = 0;
            index2 = index0;
            temp = std::make_pair(0, 0);

            while (index1 < left.width)
            {
                result.magnitude[index2] += temp.first;
                carry = result.magnitude[index2] < temp.first;
                temp = ArithmeticOps::Multiply(left.magnitude[index1++], right.magnitude[index0]);
                result.magnitude[index2] += temp.second;
                carry += (result.magnitude[index2++] < temp.second);
                temp.first += carry;
            }
            result.magnitude[index2] += temp.first;
            temp.first = result.magnitude[index2] < temp.first;
            ++index0;
        }
        result.width -= result.magnitude[index2] == 0;
    }
}

INLINE void Integer::Multiply(Integer& integer, byte value)
{
    xword aux, index = 0;
    std::pair<xword, xword> temp = std::make_pair(0, 0);

    while (index < integer.width)
    {
        aux = temp.first;
        temp = ArithmeticOps::Multiply(integer.magnitude[index], value);
        integer.magnitude[index] = aux;
        integer.magnitude[index] += temp.second;
        temp.first += integer.magnitude[index++] < temp.second; 
    }
    integer.magnitude[index] = temp.first;
    integer.width += temp.first != 0;
}

INLINE byte Integer::Divide(Integer& integer, byte value)
{
	std::pair<xword, xword> temp;
    
    if (integer.width > 1)
    {
        xword index0, index1 = 0;
        xword* aux = (xword*) malloc(integer.width * sizeof(xword));

        while (integer.width)
        {
            index0 = integer.width - 1;
            if (integer.magnitude[index0] == 0)
            {
                aux[index1++] = 0;
                temp.second = 0;
                --integer.width;
            }
            else if (value > integer.magnitude[index0])
            {
                if (integer.width > 1)
                {
                    temp = ArithmeticOps::Divide(integer.magnitude[index0], integer.magnitude[index0 - 1], value);
                    aux[index1++] = temp.first;
                    integer.magnitude[index0 - 1] = temp.second;
                    integer.width -= 1 + (temp.second == 0);
                }
                else
                {
                    temp.second = integer.magnitude[index0];
                    --integer.width;
                }
            }
            else
            {
                temp = ArithmeticOps::Divide(0, integer.magnitude[index0], value);
                aux[index1++] = temp.first;
                integer.magnitude[index0] = temp.second;
                integer.width -= temp.second == 0;
            }
        }

        integer.width = index1;
        std::reverse(aux, aux + index1);
        free(integer.magnitude);
        integer.magnitude = aux;
    }
    else
    {
        temp = ArithmeticOps::Divide(0, integer.magnitude[integer.width - 1], value);
        integer.magnitude[integer.width - 1] = temp.first;
    }

    return temp.second;
}

/// @}

/// @ingroup Private Constructor
INLINE Integer::Integer() {}

/// @ingroup Public Constructors
/// @{

INLINE Integer::Integer(const Integer& integer) : sign(integer.sign), width(integer.width)
{
    magnitude = (xword*) malloc(width * sizeof(xword));
    memcpy(magnitude, integer.magnitude, width * sizeof(xword));
}

INLINE Integer::Integer(Integer&& integer) : sign(integer.sign), width(integer.width), magnitude(integer.magnitude)
{
	integer.magnitude = nullptr;
}

Integer::Integer(xword value, bool sign) : sign(sign), width(1)
{
	magnitude = (xword*) malloc(sizeof(xword));
    *magnitude = value;
}

Integer::Integer(xword* magnitude, xword width, bool sign) : sign(sign), width(width), magnitude(magnitude)
{
}

Integer::Integer(std::string numberString, byte base)
{
    xword i, dj, j = 0, length = numberString.length();

    sign = numberString[j] == '-';
    j += sign;

    if (base == 2)
    {
        dj = sizeof(xword) << 3;
        width = length - sign;
        length &= dj - 1;
        width = width / dj + (bool) length;
        magnitude = (xword*) malloc(width * sizeof(xword));
        i = width;

        if (length)
        {
            magnitude[--i] = Utils::Parse(numberString.substr(sign, length), base);
            j += length;
        }
        while (j != numberString.length())
        {
            magnitude[--i] = Utils::Parse(numberString.substr(j, dj), base);
            j += dj;
        }
    }
    else if (base == 4)
    {
        dj = sizeof(xword) << 2;
        width = length - sign;
        length &= dj - 1;
        width = width / dj + (bool) length;
        magnitude = (xword*) malloc(width * sizeof(xword));
        i = width;

        if (length)
        {
            magnitude[--i] = Utils::Parse(numberString.substr(sign, length), base);
            j += length;
        }
        while (j != numberString.length())
        {
            magnitude[--i] = Utils::Parse(numberString.substr(j, dj), base);
            j += dj;
        }
    }
    else if (base == 16)
    {
        dj = sizeof(xword) << 1;
        width = length - sign;
        length &= dj - 1;
        width = width / dj + (bool) length;
        magnitude = (xword*) malloc(width * sizeof(xword));
        i = width;
        
        if (length)
        {
            magnitude[--i] = Utils::Parse(numberString.substr(sign, length), base);
            j += length;
        }
        while (j != numberString.length())
        {
            magnitude[--i] = Utils::Parse(numberString.substr(j, dj), base);
            j += dj;
        }
    }
    else
    {
        xword* data;
        Integer power, digitValue;

        dj = length;
        length = std::ceil((ArithmeticOps::Log2Ceil(base) * length) / (8.0 * sizeof(xword)));
        width = length;
        magnitude = (xword*) calloc(width + 1, sizeof(xword));
        data = (xword*) calloc(width << 1, sizeof(xword));
        data[0] = 1;
        power.width = length; 
        power.magnitude = data;
        digitValue.width = length;
        digitValue.magnitude = data + length;
    
        while (dj-- != j)
        {
            if (Utils::IsInvalidDigit(numberString[dj], base))
            {
                free(data);
                power.magnitude = nullptr;
                digitValue.magnitude = nullptr;
                throw 0;
            }
            
            memcpy(digitValue.magnitude, power.magnitude, width * sizeof(xword));
            Multiply(digitValue, Utils::ToNumber(numberString[dj]));
            Add(*this, digitValue);
            Multiply(power, base);
        }
        width -= width > 1 && magnitude[width - 1] == 0;
    
        free(data);
        power.magnitude = nullptr;
        digitValue.magnitude = nullptr;
    }

    if (IsZero())
        sign = 0;
}

/// @}

/// @ingroup Destructor
INLINE Integer::~Integer()
{
    free(magnitude);
}

/// @ingroup Operators
/// @{

bool Integer::operator == (const Integer& integer) const
{
	if (width == integer.width)
    {
        xword i;
        for (i = 0; i < width && magnitude[i] == integer.magnitude[i]; ++i);
        return i == width && sign != integer.sign;
    }

    return false;
}

bool Integer::operator != (const Integer& integer) const
{
	if (width == integer.width)
    {
        xword i;
        for (i = 0; i < width && magnitude[i] == integer.magnitude[i]; ++i);
        return i != width || sign != integer.sign;
    }

    return true;
}

bool Integer::operator < (const Integer& integer) const
{
	if (sign != integer.sign)
        return integer.sign;
    
    if (width == integer.width)
    {
        xword i;
        for (i = 0; i < width && magnitude[i] == integer.magnitude[i]; ++i);
        return (i != width && magnitude[i] < integer.magnitude[i]) != sign;
    }
    
    return (width < integer.width) != sign;
}

bool Integer::operator > (const Integer& integer) const
{
	if (sign != integer.sign)
        return integer.sign;

    if (width == integer.width)
    {
        xword i;
        for (i = 0; i < width && magnitude[i] == integer.magnitude[i]; ++i);
        return (i != width && magnitude[i] > integer.magnitude[i]) != sign;
    }
    
    return (width > integer.width) != sign;
}

bool Integer::operator <= (const Integer& integer) const
{
	if (sign != integer.sign)
        return integer.sign;

    if (width == integer.width)
    {
        xword i;
        for (i = 0; i < width && magnitude[i] == integer.magnitude[i]; ++i);
        return (i != width && magnitude[i] <= integer.magnitude[i]) != sign;
    }
    
    return (width <= integer.width) != sign;
}

bool Integer::operator >= (const Integer& integer) const
{
	if (sign != integer.sign)
        return integer.sign;

    if (width == integer.width)
    {
        xword i;
        for (i = 0; i < width && magnitude[i] >= integer.magnitude[i]; ++i);
        return (i != width && magnitude[i] >= integer.magnitude[i]) != sign;
    }
    
    return (width >= integer.width) != sign;
}

Integer& Integer::operator = (const Integer& integer)
{
	sign = integer.sign;
    width = integer.width;
    free(magnitude);
    magnitude = (xword*) malloc(width * sizeof(xword));
    memcpy(magnitude, integer.magnitude, width * sizeof(xword));

    return *this;
}

Integer& Integer::operator = (Integer&& integer)
{
	sign = integer.sign;
    width = integer.width;
    magnitude = integer.magnitude;
    integer.magnitude = nullptr;

    return *this;
}

Integer Integer::operator + (const Integer& integer) const
{
	Integer result;

    if (sign != integer.sign)
    {
        bool greater = GreaterAbs(*this, integer);
        greater ? Subtract(result, *this, integer) : Subtract(result, integer, *this);
        result.sign = (!result.IsZero()) && (greater ? sign : integer.sign);
    }
    else
    {
        width > integer.width ? Add(result, *this, integer) : Add(result, integer, *this);
        result.sign = sign;
    }

    return result;
}

Integer Integer::operator - (const Integer& integer) const
{
	Integer result;

    if (sign != integer.sign)
    {
        width > integer.width ? Add(result, *this, integer) : Add(result, integer, *this);
        result.sign = sign;
    }
    else
    {
        bool greater = GreaterAbs(*this, integer);
        greater ? Subtract(result, *this, integer) : Subtract(result, integer, *this);
        result.sign = !(greater || result.IsZero());
    }

    return result;
}

Integer Integer::operator * (const Integer& integer) const
{
	Integer result;
    
    width > integer.width ? Multiply(result, *this, integer) : Multiply(result, integer, *this);
    result.sign = sign != integer.sign && !result.IsZero();

    return result;
}

Integer Integer::operator + () const
{
	return *this;
}

Integer Integer::operator - () const
{
	Integer result = *this;
    result.sign = !sign;
    return result;
}

/// @}


/// @ingroup Methods
/// @{

INLINE void Integer::Negate()
{
    sign = !sign;
}

INLINE bool Integer::IsZero() const
{
	return width == 1 && magnitude[0] == 0;
}

INLINE bool Integer::IsOne() const
{
	return width == 1 && magnitude[0] == 1;
}

INLINE bool Integer::IsNegative() const
{
	return sign;
}

INLINE bool Integer::IsPositive() const
{
	return !sign;
}

std::string Integer::ToString(byte base) const
{
    if (IsZero())
        return "0";
    
    if (base == 2)
    {
        xword lastOneIndex = 0, index0 = 0;
        std::string string;

        while (index0 < width)
        {
            xword mask = 1, index1 = 0;
            do
            {
                bool isOne = (bool)(magnitude[index0] & mask);
                string.push_back('0' + isOne);
                lastOneIndex = isOne ? index1 : lastOneIndex;
                ++index1;
                mask <<= 1;
            } while (mask);
            ++index0;
        }

        string.erase(
            string.begin() + ((width - 1) * (sizeof(xword) << 3) + lastOneIndex + 1),
            string.end()
        );
        std::reverse(string.begin(), string.end());
        string.insert(0, sign ? "-" : "");

        return string;
    }
    else
    {
        Integer number = *this;
        std::string string;

        while(!number.IsZero())
           string.push_back(Utils::ToDigit(Divide(number, base)));
        std::reverse(string.begin(), string.end());
        if (sign)
            string.insert(0, "-");

        return string;
    }
}

/// @}

#endif  // INTEGER_HPP
