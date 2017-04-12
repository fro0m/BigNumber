#ifndef BIGNUMBER_H
#define BIGNUMBER_H
#include "ConstexprString/constexprstring.h"
#include <QBitArray>
#include <QString>
#include <QtGlobal>
#include <string>
#include <QDebug>

class BigNumber
{
    // use of template metaprogramming to calculate size of bits needed to hold big integer
    //    template<size_t NUMBER, size_t CONVERTED_SIZE = 0>
    //    struct calc {
    //        static const size_t size = calc<NUMBER / 2, CONVERTED_SIZE + 1>::size;
    //    };

    //    template<size_t CONVERTED_SIZE>
    //    struct calc<1, CONVERTED_SIZE> {
    //        static const size_t size = CONVERTED_SIZE + 1;
    //    };
    //    template<size_t CONVERTED_SIZE>
    //    struct calc<0, CONVERTED_SIZE> {
    //        static const size_t size = 0;
    //    };
    // ~
    enum Sign : bool { Positive, Negative };

public:    
    BigNumber(ConstexprString t_literal);
    explicit BigNumber(const std::string & t_literal);
    explicit BigNumber(const QString & t_literal);

    QString toString() const;

    BigNumber operator+(const BigNumber& t_rhs) const;



private:
    BigNumber(const QBitArray & t_data, Sign t_sign = Positive);

private:
    QBitArray m_data;
    Sign m_sign = Positive;
private: // helper methods
    /// stairway dividing
    QString divideByTwo(const QString & t_decimalString, bool * t_remainderFlag);
    /// raising BigNumber represented as string to \arg t_degree
    QString raiseTwo(int t_degree) const;
    /// adding two Big Number Strings
    QString addBigNumberString(const QString& t_string1,const QString& t_string2) const;

    QBitArray addBits(const QBitArray &lhs, const QBitArray &rhs) const;

    QBitArray subBits(const QBitArray &lhs, const QBitArray &rhs) const;
};


#endif // BIGNUMBER_H
