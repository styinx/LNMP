#ifndef LNMP_FILE_H
#define LNMP_FILE_H

#include "LNMP.h"
#include <iostream>
#include <iterator>
#include <map>

class File {
public:
    static QString getFileContents(QString filename);
    static QHash<QString, QHash<QString, QString>> getIniContents(QString filename);
};

#endif //LNMP_FILE_H
