#ifndef LNMP_WINDOW_H
#define LNMP_WINDOW_H

#include <LNMP.h>
#include <QtMultimedia/QMediaPlaylist>

class Library;
class MusicPlayer;

class Window : public QMainWindow {
    Q_OBJECT
private:
    QSettings *ini;
    QRect dimension;
    QString theme;

public:
    bool progress_slider_selected = false;

    QWidget         *main_widget;
    QGridLayout     *main_grid_layout;

    Library         *library;
    MusicPlayer     *player;

    // Navigation
    QWidget         *player_panel;
    QVBoxLayout     *player_panel_layout;
    QLabel          *player_title;

    QWidget         *player_progress;
    QHBoxLayout     *player_progress_layout;
    QSlider         *player_progress_slider;
    QLabel          *player_progress_time_expired;
    QLabel          *player_progress_time_left;

    QWidget         *player_nav;
    QHBoxLayout     *player_nav_layout;
    QWidget         *player_nav_left_filler;
    QPushButton     *player_nav_previous;
    QPushButton     *player_nav_backwards;
    QPushButton     *player_nav_playback;
    QPushButton     *player_nav_forward;
    QPushButton     *player_nav_next;
    QFrame          *player_nav_separator1;
    QPushButton     *player_nav_shuffle;
    QPushButton     *player_nav_repeat;
    QFrame          *player_nav_separator2;
    QPushButton     *player_nav_loudness;
    QSlider         *player_nav_volume;
    QWidget         *player_nav_right_filler;

    QMenu           *file_menu;
    QAction         *file_menu_action_settings;
    QAction         *file_menu_action_close;
    QMenu           *option_menu;
    QAction         *option_menu_action_display_mode;
    QMenu           *option_sub_menu_themes;


    explicit Window();
    QVariant getIni(QString caption, QString key);
    void init();
    void setup();
    void closeEvent(QCloseEvent * event) override;
    void quit();
    void save();

public slots:
    void onPositionChanged();
    void onDurationChanged();
    void onSliderPressed();
    void toggleDisplayMode();
    void changeStylesheet();
};

class Library : public QTabWidget {
    Q_OBJECT
private:
    Window             *parent;
public:
    // Library
    QStandardItemModel  *library_model;
    QTreeView           *library_view;

    explicit Library(Window * parent);
    void loadLibrary();

    void editMetadata();
public slots:
    void contextMenu();
};

class MusicPlayer : public QMediaPlayer {
    Q_OBJECT
private:
    Window * parent;
    QString label;
    QString path;
    QStringList trackinfo;

    QTreeView * view;
    QStandardItemModel * model;

public:
    explicit MusicPlayer(Window * parent);

public slots:
    void onStateChanged(QMediaPlayer::State state);
    void onPreviousTrackClicked();
    void onBackwardsClicked();
    void onPlaybackClicked();
    void onForwardClicked();
    void onNextTrackClicked();
    void onTrackDoubleClicked(const QModelIndex &current);
    void onVolumeChanged(int val);
    void onPositionChanged();
    void onShuffleClicked();
    void onRepeatClicked();
    void onMuteClicked();
};


class Audio {
public:
    static QString format(int length);
    static int length(QString format);
};

#endif //LNMP_WINDOW_H
