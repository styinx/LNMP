#include <utils/file.h>

QString File::getFileContents(QString filename) {
    QFile f(filename);
    if (!f.open(QFile::ReadOnly | QFile::Text))
        return "";

    QTextStream in(&f);
    return in.readAll();
}

QHash<QString, QHash<QString, QString>> File::getIniContents(QString filename) {
    QHash<QString, QHash<QString, QString>> ini;

    QFile f(filename);
    if (!f.open(QFile::ReadOnly | QFile::Text))
        return ini;

    QTextStream in(&f);
    QString line = "";
    QString caption = "";
    while(!in.atEnd()) {
        line = in.readLine(255);
        QStringList kvp = line.split("=");
        if(kvp.length() == 1) {
            caption = line.replace('[', "").replace(']', "");
        } else if(kvp.length() == 2){
            ini[caption][kvp.at(0).simplified()] = kvp.at(1).simplified();
        }
    }
    return ini;
};