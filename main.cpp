#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>


class BigInt
{
private:
    std::vector<int> digits;
    bool is_negative = false;

    void RemoveLeadingZeros()
    {
        while (digits.size() > 1 && digits.back() == 0)
        {
            digits.pop_back();
        }
    }

public:

    BigInt(std::string str)
    {
        int end = 0;
        if (str[0] == '-') 
        {
            is_negative = true;
            end = 1;
        }
        for(int i = str.size() - 1; i >= end; --i)
        {
            digits.push_back(str[i] - '0');
        }
    }

    BigInt()
    {
        // digits.push_back(0);
        ;
    }

    void print()
    {
        if(is_negative) std::cout << '-';
        for (int i = digits.size() - 1; i >= 0; --i) 
        {
            std::cout << digits[i];
        }
        std::cout << std::endl;
    }

    short compare_abs(BigInt a, BigInt b) // Comparing absolute values of two BigInts
    {
        if(a.digits.size() > b.digits.size())
        {
            return 1; // a is bigger
        }
        else if (a.digits.size() < b.digits.size())
        {
            return 2; // b is bigger
        }
        else
        {
            for(int i = digits.size(); i >= 0; i--)
            {
                if(a.digits[i] > b.digits[i])
                {
                    return 1; // a is bigger
                }
                else if(a.digits[i] < b.digits[i])
                {
                    return 2; // b is bigger
                }
            }
            return 3; // a equals b
        }
    }

    BigInt operator+(BigInt other) 
    {
        BigInt result;

        if ((is_negative && other.is_negative) || (!is_negative && !other.is_negative)) // Если числа имет одинаковые знаки
        {
            int carry = 0;
            int MaxSize = std::max(digits.size(), other.digits.size());

            for(int i = 0; i < MaxSize || carry; i++)
            {
                int sum = carry;
                if(i < digits.size()) sum += digits[i];
                if(i < other.digits.size()) sum += other.digits[i];
                carry = sum / 10;
                result.digits.push_back(sum % 10);
            }
            if (is_negative && other.is_negative) result.is_negative = true;
        }
        else 
        {
            int MaxSize = std::max(digits.size(), other.digits.size());
            int draw = 0;


            // Ddeciding which BigInt is a subtrahend and which is subtractor based on their absolute values
            BigInt *subtrahend;
            BigInt *subtractor;

            short resp = compare_abs(*this, other);

            switch (resp)
            {
            case 1:
                subtrahend = this;
                subtractor = &other;
                break;
            
            case 2:
                subtrahend = &other;
                subtractor = this;
                break;
            case 3:
                result.digits.push_back(0);
                return result;
                break;
            }


            // Perfoming subtraction
            for (int i = 0; i < (*subtrahend).digits.size(); i++)
            {
                int dif = -draw;

                dif += (*subtrahend).digits[i];
                if (i < (*subtractor).digits.size()) dif -= (*subtractor).digits[i];
                if (dif < 0)
                {
                    dif += 10;
                    draw = 1;
                }
                else draw = 0;
                result.digits.push_back(dif);
            }
            
            // Determining sign of a result
            result.is_negative = subtrahend->is_negative;
        }
        result.RemoveLeadingZeros();
        return result;
    }

    BigInt operator+=(const BigInt other) 
    {
        digits = (*this + other).digits;
        return *this + other;
    }
 
    
    BigInt operator*(const BigInt other) const
    {
        BigInt result("0");
        

        for (int i = 0; i < other.digits.size(); i++)
        {
            BigInt subresult("");
            int carry = 0;
            int sum;

            for(int _ = 0; _ < i; _++) {subresult.digits.push_back(0);}

            for(int j = 0; j < digits.size() || carry; j++)
            {
                sum = carry;
                if(j < digits.size()) sum += digits[j] * other.digits[i];
                carry = sum / 10;
                subresult.digits.push_back(sum % 10);
            }
            subresult.RemoveLeadingZeros();
            result += subresult;
        }

        result.is_negative = is_negative != other.is_negative;
        return result;
    }

    BigInt operator*=(const BigInt other)
    {
        digits = (*this * other).digits;
        return *this * other;
    }

    bool operator>(const BigInt other) 
    {
        if(is_negative && !other.is_negative) return false;
        else if (!is_negative && other.is_negative) return true;
        else if (is_negative && other.is_negative) return compare_abs(*this, other) == 2;
        else return compare_abs(*this, other) == 1;
    }

    bool operator<(const BigInt other) 
    {
        if(is_negative && !other.is_negative) return true;
        else if (!is_negative && other.is_negative) return false;
        else if (is_negative && other.is_negative) return compare_abs(*this, other) == 1;
        else return compare_abs(*this, other) == 2;
    }

    bool operator==(const BigInt other)
    {
        if(is_negative != other.is_negative) return false;
        else return compare_abs(*this, other) == 3;
    }

    bool operator!=(const BigInt other)
    {
        if(is_negative != other.is_negative) return true;
        else return compare_abs(*this, other) != 3;
    }

    friend std::ostream& operator<<(std::ostream& os, BigInt& bigint);
    friend std::istream& operator>>(std::istream& is, BigInt& BigInt);
};

std::ostream& operator<<(std::ostream& os, BigInt& bigint)
{
    if(bigint.is_negative) os << '-';
    for (int i = bigint.digits.size() - 1; i >= 0; --i)
    {
        os << bigint.digits[i];
    }
    return os;
}

std::istream& operator>>(std::istream& is, BigInt& BigInt)
{
    
}



int main()
{
    std::cout << "Enter the number a: ";std::string inp1; std::getline(std::cin, inp1);
    // std::cout << "Enter the number b: ";std::string inp2; std::getline(std::cin, inp2);
    BigInt a(inp1);
    // BigInt b(inp2);

    // std::cout << "Summ: ";
    // (a + b).print();

    // std::cout << "Product: ";
    // (a * b).print();

    // std::cout << "a < b: " << (a < b) << std::endl;
    // std::cout << "a > b: " << (a > b) << std::endl;
    // std::cout << "a == b: " << (a == b) << std::endl;
    // std::cout << "a != b: " << (a != b) << std::endl;

    std::cout << a;

    return 0;
}