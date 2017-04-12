#include "bignumber.h"

namespace {
bool operator<(const QBitArray &lhs, const QBitArray &rhs)
{
    int lengthDiff = lhs.size() - rhs.size();
    if (lengthDiff < 0) {
        return true;
    } else if (lengthDiff > 0) {
        return false;
    }
    // after check we suppose that lengths are equal
    Q_ASSERT(lhs.size() == rhs.size());
    for(int i = lhs.size() - 1; i >= 0; --i) {
        int result = static_cast<int>(lhs[i] - rhs[i]);
        if (result == -1)
            return true;
        if (result == 1)
            return false;
    }
    return false;
}
}

BigNumber::BigNumber(ConstexprString t_literal) : BigNumber(QString::fromLatin1(t_literal.rawData(), t_literal.size()))
{

}

BigNumber::BigNumber(const std::string &t_literal) : BigNumber(QString::fromStdString(t_literal))
{

}

BigNumber::BigNumber(const QString &t_literal)
{
    auto processInputF = [&](size_t firstIndex = 0) {
        bool remainderOfDivision = static_cast<bool>(0);
        QString t_literalNumber = t_literal.mid(firstIndex);
        while(t_literalNumber.length() != 0) {
            t_literalNumber = divideByTwo(t_literalNumber, &remainderOfDivision);
            m_data.resize(m_data.size() + 1);
            m_data[m_data.size() - 1] = remainderOfDivision;
        }
        //qDebug() << m_data;
    };

    switch (t_literal[0].toLatin1()) {
    case '-':
        Q_ASSERT(!(t_literal[1] == '0' && t_literal.size() > 1));
        m_sign = Negative;
        processInputF(1);
        break;
    case '+':
        Q_ASSERT(!(t_literal[1] == '0' && t_literal.size() > 1));
        processInputF(1);
        break;
    case '0':
        Q_ASSERT(t_literal.size() == 1);
        break;
    default:
        processInputF();
    }
}

QString BigNumber::toString() const {
    QString string;
    for(int i = m_data.size() - 1; i != -1; --i) {
        if (m_data[i]) {
            string = addBigNumberString(string, raiseTwo(i));
        }
    }
    if (m_sign == Negative) {
        string.insert(0, QChar::fromLatin1('-'));
    }
    if (string.isNull()) {
        string = "0";
    }
    return string;
}

BigNumber BigNumber::operator+(const BigNumber &t_rhs) const {
    const BigNumber* larger;
    const BigNumber* smaller;
    if (t_rhs.m_data < m_data) {
        larger = this;
        smaller = &t_rhs;
    } else {
        larger = &t_rhs;
        smaller = this;
    }
    auto largerSign = larger->m_sign;
    auto smallerSign = smaller->m_sign;

    Sign sign = largerSign == Negative &&
            smallerSign == Positive ?
                Negative : largerSign;

    if (largerSign != smallerSign) {
        return BigNumber(subBits(larger->m_data, smaller->m_data), sign);
    }
    return BigNumber(addBits(larger->m_data, smaller->m_data), sign);
}

BigNumber::BigNumber(const QBitArray &t_data, BigNumber::Sign t_sign) : m_data(t_data), m_sign(t_sign) {
    Q_ASSERT(t_sign == Positive || t_sign == Negative);
}

QString BigNumber::divideByTwo(const QString &t_decimalString, bool *t_remainderFlag) {
    QString dividedString;
    bool remainder = false;
    for(auto ch = t_decimalString.cbegin(); ch != t_decimalString.cend(); ++ch) {
        quint8 digit = static_cast<quint8>((*ch).toLatin1() - '0');

        // case when we add 10 to divided digit
        if (remainder)
            digit += 10;
        // ~

        remainder = digit % 2;
        digit /= 2;
        if (!(digit == 0 && Q_UNLIKELY(ch == t_decimalString.cbegin()))) {
            dividedString += QChar(digit + '0');
        }
    }

    *t_remainderFlag = remainder;

    return dividedString;
}

QString BigNumber::raiseTwo(int t_degree) const {
    QString raisedString = "1";
    //raisedString.reserve(std::pow(2, t_degree));
    for(int i = 1; i <= t_degree; ++i) {
        raisedString = addBigNumberString(raisedString, raisedString);
    }
    return raisedString;
}

QString BigNumber::addBigNumberString(const QString &t_string1, const QString &t_string2) const {
    QString resultString;
    const QString * longer;
    const QString * shorter;
    bool remainder = false;
    if (t_string1.length() >= t_string2.length()) {
        longer = &t_string1;
        shorter = &t_string2;
    } else {
        longer = &t_string2;
        shorter = &t_string1;
    }
    resultString.resize(longer->length());
    quint8 digit;
    const size_t lengthDiff = longer->length() - shorter->length();
    // filling in resultString with sum
    for(int i = shorter->length() - 1; i >= 0 ; --i) {
        digit = static_cast<quint8>(shorter->at(i).toLatin1() - '0');
        if (remainder) {
            digit++;
            remainder = false;
        }
        quint8 resultNumber = static_cast<quint8>(longer->at(i + lengthDiff).toLatin1() - '0');
        resultNumber += digit;
        if (resultNumber > 9) {
            resultNumber -= 10;
            remainder = true;
        }
        resultString[static_cast<int>(i + lengthDiff)] = QChar::fromLatin1(static_cast<char>(resultNumber) + '0');
    }

    //filling up resultString with rest of longer
    for(int i = lengthDiff - 1; i >= 0; --i) {
        digit = static_cast<quint8>(longer->at(i).toLatin1() - '0');
        if (remainder) {
            digit += 1;
            remainder = false;
            if (digit == 10) {
                digit -= 10;
                remainder = true;
            }
        }
        resultString[i] = QChar::fromLatin1(static_cast<char>(digit) + '0');
    }

    // inserting 1 if there is remainder left
    if (remainder) {
        resultString.insert(0, QChar::fromLatin1('1'));
    }
    return resultString;
}

QBitArray BigNumber::addBits(const QBitArray &lhs, const QBitArray &rhs) const {
    QBitArray ba;
    ba.resize(lhs.size());
    bool remainder = false;
    //  calculating shift to delete superfluous zeros
    for(int i = 0; i < rhs.size(); ++i) {
        int res = lhs[i] + rhs[i] + remainder;
        switch (res) {
        case 0:
            ba[i] = false;
            break;
        case 1:
            ba[i] = true;
            remainder = false;
            break;
        case 2:
            ba[i] = false;
            remainder = true;
            break;
        case 3:
            ba[i] = true;
            remainder = true;
        }
    }
    for(int i = rhs.size(); i < lhs.size(); ++i) {
        int res = lhs[i] + remainder;
        switch (res) {
        case 0:
            ba[i] = false;
            break;
        case 1:
            ba[i] = true;
            remainder = false;
            break;
        case 2:
            ba[i] = false;
            remainder = true;
        }
    }
    if (remainder) {
        ba.resize(ba.size() + 1);
        ba[ba.size() - 1] = true;
    }
    return ba;
}

QBitArray BigNumber::subBits(const QBitArray &lhs, const QBitArray &rhs) const {
    QBitArray ba;
    bool remainder = false;
    //  calculating shift to delete superfluous zeros
    size_t shift = 0;
    for(int i = 0; i < rhs.size(); ++i) {
        ba.resize(ba.size() + 1);
        int res = lhs[i] - rhs[i] - remainder;
        switch (res) {
        case 0:
            ba[i] = false;
            remainder = false;
            shift++;
            break;
        case 1:
            ba[i] = true;
            shift = 0;
            break;
        case -1:
            ba[i] = true;
            remainder = true;
            shift = 0;
            break;
        case -2:
            ba[i] = false;
            remainder = true;
            shift++;
        }
    }
    for(int i = rhs.size(); i < lhs.size(); ++i) {
        ba.resize(ba.size() + 1);
        int res = lhs[i] - remainder;
        switch (res) {
        case -1:
            ba[i] = true;
            remainder = true;
            shift = 0;
            break;
        case 0:
            ba[i] = false;
            remainder = false;
            shift++;
            break;
        case 1:
            ba[i] = true;
            shift++;
        }
    }
    if (remainder) {
        ba.resize(ba.size() + 1);
        ba[ba.size() - 1] = true;
        shift = 0;
    }
    ba.resize(qMax(static_cast<size_t>(1), ba.size() - shift));
    return ba;
}
