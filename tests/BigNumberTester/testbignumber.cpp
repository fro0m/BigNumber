#include <QtTest/QtTest>
#include <QObject>
#include "BigNumber/bignumber.h"


class TestBigNumber : public QObject {
    Q_OBJECT
private slots:
    void testConstruct();
    void testToString();
    void testAddition();
};

void TestBigNumber::testConstruct() {

    constexpr ConstexprString st0("01212345678901234"); // wrong
    constexpr ConstexprString st1("1212345678901234");
    constexpr ConstexprString st2("+1212345678901234");
    constexpr ConstexprString st3("-123");
    constexpr ConstexprString st4("-121234567890123412123456789012341212345678901234121234567890123412123456789012341212345678901234");
    //BigNumber bn0(st0);  // assert fires
    BigNumber bn1(st1);
    BigNumber bn2(st2);
    BigNumber bn3(st3);
    BigNumber bn4(st4);
}

void TestBigNumber::testToString()
{
    constexpr ConstexprString st4("-121234567890123412123456789012341212345678901234121234567890123412123456789012341212345678901234");
    QString st4tst("-121234567890123412123456789012341212345678901234121234567890123412123456789012341212345678901234");
    BigNumber bn4(st4);
    QCOMPARE(st4tst, bn4.toString());
}

void TestBigNumber::testAddition()
{
    constexpr ConstexprString st1("-1212");
    constexpr ConstexprString st2("-1212");
    constexpr ConstexprString st3("600");
    constexpr ConstexprString st4("1212");
    constexpr ConstexprString st5("0");
    BigNumber bn1(st1);
    BigNumber bn2(st2);
    BigNumber bn3(st3);
    BigNumber bn4(st4);
    BigNumber bn5(st5);

    const BigNumber & sum13 = bn1 + bn3;
    QCOMPARE(sum13.toString(), QString::number(-1212 + 600));
    const BigNumber & sum14 = bn1 + bn4;
    QCOMPARE(sum14.toString(), QString::number(0));
    const BigNumber & sum12 = bn1 + bn2;
    QCOMPARE(sum12.toString(), QString::number(-1212 + -1212));
    const BigNumber & sum34 = bn3 + bn4;
    QCOMPARE(sum34.toString(), QString::number(600 + 1212));
    const BigNumber & sum23 = bn2 + bn3;
    QCOMPARE(sum23.toString(), QString::number(-1212 + 600));

    QCOMPARE((bn2 + bn3).toString(), (bn3 + bn2).toString());

    const BigNumber & sum1234 = bn1 + bn2 + bn3 + bn4;
    QCOMPARE(sum1234.toString(), QString::number(-1212 + -1212 + 600 + 1212));

    const BigNumber &sum55 = bn5 + bn5;
    QCOMPARE(sum55.toString(), QString::number(0));

    constexpr ConstexprString stBig("-1212345678901234121234234567890123412123456789012341212345678901234121234567890123412123456789012341212345678901234");
    QString strBig("-2424691357802468242468469135780246824246913578024682424691357802468242469135780246824246913578024682424691357802468");
    BigNumber bnSuperFat(stBig);
    QCOMPARE((bnSuperFat + bnSuperFat).toString(), strBig);
}

QTEST_GUILESS_MAIN(TestBigNumber)
#include "testbignumber.moc"
