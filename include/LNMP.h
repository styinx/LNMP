#ifndef LNMP_LMNP_H
#define LNMP_LMNP_H

#include <QtCore/QObject>
#include <QtCore/QtDebug>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QStandardPaths>
#include <QtCore/QSettings>
#include <QtCore/QVariant>
#include <QtCore/QDirIterator>
#include <QCloseEvent>
#include <QDateTime>
#include <QContextMenuEvent>

#include <QtMultimedia/QMediaPlayer>

#include <QtGui/QIcon>
#include <QtGui/QPixmap>
#include <QtGui/QStandardItemModel>

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QFileDialog>

#include <QtWidgets/QWidget>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QSlider>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QScrollArea>

const QString PATH_BASE = "../";
const QString PATH_RESOURCES = PATH_BASE + "resources/";
const QString PATH_STYLE = PATH_RESOURCES + "style/";
const QString PATH_ICONS = PATH_STYLE + "icons/";
const QString PATH_THEMES = PATH_STYLE + "themes/";

const QString FILE_INI = PATH_RESOURCES + "ini.ini";

#endif //LNMP_LMNP_H
