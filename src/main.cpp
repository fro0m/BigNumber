#include <QCoreApplication>
#include "BigNumber/bignumber.h"
#include "ConstexprString/constexprstring.h"
//#include <QString>
//#include <QByteArray>
#include <iostream>
#include <string>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    qDebug() << "This is the application to show how to use BigNumber.\n"
                "BigNumber class uses almost minimum possible amount of memory to keep data\n"
                "Enter 1st Big Integer and press enter";
    std::string str;
    std::cin >> str;
    BigNumber bn1(str);
    qDebug() << "Enter 2nd Big Integer and press enter";
    BigNumber bn2(str);
    qDebug() << "Sum of two numbers is " << (bn1 + bn2).toString();
    return app.exec();
}
