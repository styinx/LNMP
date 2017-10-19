#include <utils/file.h>
#include <window/window.h>
#include <taglib/fileref.h>
#include <taglib/toolkit/tpropertymap.h>

Window::Window() {
//    setWindowFlags(Qt::CustomizeWindowHint);
    init();
    theme = getIni("Application", "theme").toString();
    setStyleSheet(File::getFileContents(PATH_THEMES + theme + ".css"));
    setup();
    library->loadLibrary();
}

QVariant Window::getIni(QString caption, QString key) {
    ini->beginGroup(caption);
    QVariant variant = ini->value(key);
    ini->endGroup();
    return variant;
}

void Window::init() {
    ini = new QSettings(FILE_INI, QSettings::IniFormat);
    QVariantList window = getIni("Window", "xywh").toList();
    dimension = QRect(window.at(0).toInt(), window.at(1).toInt(), window.at(2).toInt(), window.at(3).toInt());
    if(getIni("Window", "minimized").toBool()) {
        setGeometry(dimension);
    } else {
        setGeometry(dimension.x(), dimension.y(), 0, 0);
    }
    ini->beginGroup("Application");
    if(ini->value("library").toString() == ".") {
        QString library = QFileDialog::getExistingDirectory(this, "Choose Music Library", QDir::homePath(),
                                                            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        ini->setValue("library", library);
    }
    ini->endGroup();
}

void Window::setup() {
    main_widget = new QWidget();
    main_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setCentralWidget(main_widget);

    main_grid_layout = new QGridLayout(main_widget);

    library = new Library(this);
    library->setVisible(getIni("Window", "minimized").toBool());
    main_grid_layout->addWidget(library, 0, 0, 1, 2);

    // Bottom: MusicPlayer and Controls
    player                          = new MusicPlayer(this);
    player_panel                    = new QWidget();
    player_panel_layout             = new QVBoxLayout(player_panel);
    player_title                    = new QLabel("");
    player_progress                 = new QWidget();
    player_progress_layout          = new QHBoxLayout(player_progress);
    player_progress_slider          = new QSlider();
    player_progress_time_expired    = new QLabel("0:00");
    player_progress_time_left       = new QLabel("0:00");
    player_nav                      = new QWidget();
    player_nav_layout               = new QHBoxLayout(player_nav);
    player_nav_left_filler          = new QWidget();
    player_nav_previous             = new QPushButton(QIcon(PATH_ICONS + "previous.png"), "");
    player_nav_backwards            = new QPushButton(QIcon(PATH_ICONS + "backward.png"), "");
    player_nav_playback             = new QPushButton(QIcon(PATH_ICONS + "play.png"), "");
    player_nav_forward              = new QPushButton(QIcon(PATH_ICONS + "forward.png"), "");
    player_nav_next                 = new QPushButton(QIcon(PATH_ICONS + "next.png"), "");
    player_nav_separator1           = new QFrame();
    player_nav_shuffle              = new QPushButton(QIcon(PATH_ICONS + "shuffle.png"), "");
    player_nav_repeat               = new QPushButton(QIcon(PATH_ICONS + "repeat.png"), "");
    player_nav_separator2           = new QFrame();
    player_nav_loudness             = new QPushButton(QIcon(PATH_ICONS + "sound.png"), "");
    player_nav_volume               = new QSlider();
    player_nav_right_filler         = new QWidget();

    player->setVolume(getIni("Application", "volume").toInt());
    player_progress_slider->setOrientation(Qt::Orientation::Horizontal);
    player_progress_slider->setMinimum(0);
    player_progress_slider->setSingleStep(1);
    player_nav_backwards->setAutoRepeat(true);
    player_nav_forward->setAutoRepeat(true);
    player_nav_shuffle->setCheckable(true);
    player_nav_repeat->setCheckable(true);
    player_nav_separator1->setFrameShape(QFrame::HLine);
    player_nav_separator1->setFrameShadow(QFrame::Sunken);
    player_nav_separator1->setMinimumSize(1, 32);
    player_nav_separator1->setMaximumSize(1, 32);
    player_nav_volume->setOrientation(Qt::Orientation::Horizontal);
    player_nav_volume->setMaximumSize(100, 28);
    player_nav_volume->setMinimumSize(100, 28);
    player_nav_volume->setRange(0, 100);
    player_nav_volume->setValue(player->volume());
    player_nav_separator2->setFrameShape(QFrame::HLine);
    player_nav_separator2->setFrameShadow(QFrame::Sunken);
    player_nav_separator2->setMinimumSize(1, 32);
    player_nav_separator2->setMaximumSize(1, 32);

    player_progress_layout->addWidget(player_progress_time_expired);
    player_progress_layout->addWidget(player_progress_slider, 1);
    player_progress_layout->addWidget(player_progress_time_left);
    player_progress_layout->setContentsMargins(0, 0, 0, 0);

    player_nav_layout->addWidget(player_nav_left_filler, 1);
    player_nav_layout->addWidget(player_nav_previous);
    player_nav_layout->addWidget(player_nav_backwards);
    player_nav_layout->addWidget(player_nav_playback);
    player_nav_layout->addWidget(player_nav_forward);
    player_nav_layout->addWidget(player_nav_next);
    player_nav_layout->addWidget(player_nav_separator1);
    player_nav_layout->addWidget(player_nav_shuffle);
    player_nav_layout->addWidget(player_nav_repeat);
    player_nav_layout->addWidget(player_nav_separator2);
    player_nav_layout->addWidget(player_nav_loudness);
    player_nav_layout->addWidget(player_nav_volume, 1);
    player_nav_layout->addWidget(player_nav_right_filler, 1);
    player_nav_layout->setContentsMargins(0, 0, 0, 0);

    player_panel_layout->addWidget(player_title, 0, Qt::AlignCenter);
    player_panel_layout->addWidget(player_progress);
    player_panel_layout->addWidget(player_nav);

    connect(player_nav_previous, SIGNAL(clicked()), player, SLOT(onPreviousTrackClicked()));
    connect(player_nav_backwards, SIGNAL(clicked()), player, SLOT(onBackwardsClicked()));
    connect(player_nav_backwards, SIGNAL(pressed()), player, SLOT(onBackwardsClicked()));
    connect(player_nav_playback, SIGNAL(clicked()), player, SLOT(onPlaybackClicked()));
    connect(player_nav_forward, SIGNAL(clicked()), player, SLOT(onForwardClicked()));
    connect(player_nav_forward, SIGNAL(pressed()), player, SLOT(onForwardClicked()));
    connect(player_nav_next, SIGNAL(clicked()), player, SLOT(onNextTrackClicked()));
    connect(library->library_view, SIGNAL(doubleClicked(const QModelIndex &)),
            player, SLOT(onTrackDoubleClicked(const QModelIndex &)));
    connect(player_nav_volume, SIGNAL(valueChanged(int)), player, SLOT(onVolumeChanged(int)));
    connect(player_progress_slider, SIGNAL(sliderPressed()), this, SLOT(onSliderPressed()));
    connect(player_progress_slider, SIGNAL(sliderReleased()), player, SLOT(onPositionChanged()));
    connect(player_nav_shuffle, SIGNAL(clicked()), player, SLOT(onShuffleClicked()));
    connect(player_nav_repeat, SIGNAL(clicked()), player, SLOT(onRepeatClicked()));
    connect(player_nav_loudness, SIGNAL(clicked()), player, SLOT(onMuteClicked()));
    main_grid_layout->addWidget(player_panel, 1, 0, 1, 2);
    main_grid_layout->setContentsMargins(0, 0, 0, 0);

    // Menubar
    file_menu   = new QMenu();
    file_menu   = menuBar()->addMenu("&File");

    file_menu_action_settings   = new QAction(QIcon(PATH_ICONS + "settings.png"), "Settings", file_menu);
    file_menu_action_close      = new QAction(QIcon(PATH_ICONS + "error.png"), "Close", file_menu);

    connect(file_menu_action_settings, &QAction::triggered, this, quit);
    connect(file_menu_action_close, &QAction::triggered, this, quit);

    file_menu->addAction(file_menu_action_settings);
    file_menu->addAction(file_menu_action_close);

    option_menu = new QMenu();
    option_menu = menuBar()->addMenu("&Option");

    option_menu_action_display_mode = new QAction(QIcon(PATH_ICONS + "star.png"), "Change Display Mode", option_menu);
    option_sub_menu_themes = option_menu->addMenu("Themes");

    QDirIterator it(PATH_THEMES, {"*.css"}, QDir::Files);
    while (it.hasNext()) {
        QFileInfo file = QFileInfo(it.next());
        QString filename = file.fileName();
        auto theme_selection = new QAction(filename.remove(filename.length() - 4, 4), option_sub_menu_themes);
        connect(theme_selection, SIGNAL(triggered()), this, SLOT(changeStylesheet()));
        option_sub_menu_themes->addAction(theme_selection);
    }

    file_menu_action_close->setShortcut(Qt::Key_Q | Qt::CTRL);
    file_menu_action_settings->setShortcut(Qt::Key_S | Qt::CTRL);
    option_menu_action_display_mode->setShortcut(Qt::Key_D | Qt::CTRL);

    connect(option_menu_action_display_mode, &QAction::triggered, this, toggleDisplayMode);


    option_menu->addAction(option_menu_action_display_mode);
}

void Window::closeEvent (QCloseEvent *event)
{
    quit();
}

void Window::quit() {
    player->stop();
    save();
    QApplication::quit();
}

void Window::save() {
    if (library->isVisible()) {
        dimension = geometry();
    }
    ini->beginGroup("Window");
    QVariantList window = {};
    window.append(dimension.x());
    window.append(dimension.y());
    window.append(dimension.width());
    window.append(dimension.height());
    ini->setValue("xywh", QVariant::fromValue(window));
    ini->setValue("minimized", QVariant::fromValue(library->isVisible()));
    ini->endGroup();
    ini->beginGroup("Application");
    QStringList headers = {};
    QVariantList sizes = {};
    for(int col = 0; col < library->library_model->columnCount() - 1; ++col) {
        headers += QString(library->library_model->headerData(col, Qt::Horizontal).toString());
        sizes.append(library->library_view->columnWidth(col));
    }
    ini->setValue("theme", QVariant::fromValue(theme));
    ini->setValue("library_header", QVariant::fromValue(headers));
    ini->setValue("library_header_sizes", QVariant::fromValue(sizes));
    ini->setValue("volume", QVariant::fromValue(player->volume()));
    ini->endGroup();
    ini->sync();
}

void Window::onPositionChanged() {
    if(!progress_slider_selected) {
        int val = (int) player->position() / 1000;
        int len = (int) player->duration() / 1000;
        player_progress_slider->setSliderPosition(val);
        player_progress_time_expired->setText(Audio::format(val));
        player_progress_time_left->setText(Audio::format(len - val));
    }
}

void Window::onDurationChanged() {
    player_progress_slider->setMaximum((int)player->duration()/1000);
}

void Window::onSliderPressed() {
    progress_slider_selected = true;
}

void Window::toggleDisplayMode() {
    if(library->isVisible()) {
        dimension = geometry();
        library->setVisible(false);
        setMinimumSize(0, 0);
        setGeometry(dimension.x(), dimension.y(), 0, 0);
    } else {
        dimension.setX(geometry().x());
        dimension.setY(geometry().y());
        library->setVisible(true);
        setGeometry(dimension);
    }
}

void Window::changeStylesheet() {
    auto action = qobject_cast<QAction*>(QObject::sender());
    if(action != nullptr) {
        theme = action->text();
        setStyleSheet(File::getFileContents(PATH_THEMES + "/" + action->text() + ".css"));
    }
}

Library::Library(Window *owner) {
    parent = owner;

    QStringList headers = parent->getIni("Application", "library_header").value<QStringList>();
    QVariantList sizes = parent->getIni("Application", "library_header_sizes").value<QVariantList>();

    library_model   = new QStandardItemModel(0, headers.length() + 1);
    library_view    = new QTreeView();
    library_view->setModel(library_model);
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(contextMenu()));

    for(int col = 0; col < headers.length(); ++col) {
        library_model->setHeaderData(col, Qt::Orientation::Horizontal, headers.at(col));
    }
    library_model->setHeaderData(headers.length(), Qt::Orientation::Horizontal, "abs_path");
    library_view->setColumnHidden(headers.length(), true);
    for(int col = 0; col < sizes.length(); ++col) {
        library_view->setColumnWidth(col, sizes.at(col).toInt());
    }
    library_view->setRootIsDecorated(false);
    library_view->setSortingEnabled(true);
    library_view->resizeColumnToContents(headers.length() - 1);
    library_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    addTab(library_view, QIcon(PATH_ICONS + "star.png"), "Library");
}

void Library::loadLibrary() {
    QString path = parent->getIni("Application", "library").toString();
    QDirIterator it(path, QStringList() << "*.mp3", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFileInfo file = QFileInfo(it.next());
        TagLib::FileRef f(file.absoluteFilePath().toStdString().c_str());
        TagLib::AudioProperties * ap = f.audioProperties();
        QList<QStandardItem *> row = QList<QStandardItem *>();
        row.append(new QStandardItem(f.tag()->artist().toCString()));
        row.append(new QStandardItem(f.tag()->title().toCString()));
        row.append(new QStandardItem(f.tag()->album().toCString()));
        row.append(new QStandardItem(f.tag()->genre().toCString()));
        row.append(new QStandardItem(Audio::format(ap->lengthInSeconds())));
        row.append(new QStandardItem(file.absoluteFilePath()));

        library_model->insertRow(library_model->rowCount(), row);
    }
}

void Library::contextMenu() {
    auto * contextMenu = new QMenu("", this);

    auto * search_delete = new QAction("Search/Delete duplicates", contextMenu);
    auto * edit_metadata = new QAction("Edit Metadata", contextMenu);

    contextMenu->addAction(search_delete);
    contextMenu->addAction(edit_metadata);

    connect(edit_metadata, &QAction::triggered, this, editMetadata);

    contextMenu->exec(QCursor::pos());
}

void Library::editMetadata() {
    auto dialog         = new QDialog(this);
    auto dialog_layout  = new QGridLayout(dialog);

    auto artist_label   = new QLabel("Arist");
    auto artist         = new QLineEdit();
    auto title_label    = new QLabel("Title");
    auto title          = new QLineEdit();
    auto save           = new QPushButton(QIcon(PATH_ICONS + "success.png"), "Save");
    auto close          = new QPushButton(QIcon(PATH_ICONS + "error.png"), "Close");

    dialog_layout->addWidget(artist_label, 0, 0);
    dialog_layout->addWidget(artist, 0, 1);
    dialog_layout->addWidget(title_label, 1, 0);
    dialog_layout->addWidget(title, 1, 1);
    dialog_layout->addWidget(save, 6, 0, Qt::AlignLeft);
    dialog_layout->addWidget(close, 6, 1, Qt::AlignRight);

    dialog->show();
}

MusicPlayer::MusicPlayer(Window *owner) {
    parent = owner;
    view = parent->library->library_view;
    model = parent->library->library_model;

    connect(this, SIGNAL(positionChanged(qint64)), parent, SLOT(onPositionChanged()));
    connect(this, SIGNAL(durationChanged(qint64)), parent, SLOT(onDurationChanged()));
    connect(this, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(onStateChanged(QMediaPlayer::State)));
}

void MusicPlayer::onStateChanged(QMediaPlayer::State state) {
    if(state == State::StoppedState) {
        int row = model->itemFromIndex(view->currentIndex())->row();
        if(row != model->rowCount() - 1) {
            if(position() == duration()) {
                onNextTrackClicked();
            }
        }
    }
}

void MusicPlayer::onPreviousTrackClicked() {
    stop();

    QModelIndex current = view->selectionModel()->currentIndex();

    if(current.sibling(current.row() - 1, 0).isValid()) {
        view->setCurrentIndex(current.sibling(current.row() - 1, 0));

        QStringList row;
        for(int col = 0; col < model->columnCount(); ++col) {
            row.append(current.sibling(current.row() - 1, col).data().toString());
        }

        trackinfo = row;
        path = row.at(row.size() - 1);
        label = "<b>" + row.at(0) + " - " + row.at(1) + "</b> [" + row.at(row.size() - 2) + "]";

        setMedia(QMediaContent(QUrl::fromLocalFile(path)));
    }
    onPlaybackClicked();
}

void MusicPlayer::onBackwardsClicked() {
    setPosition(position() - duration()/100);
}

void MusicPlayer::onPlaybackClicked() {
    if(state() == State::PlayingState) {
        parent->player_nav_playback->setIcon(QIcon(PATH_ICONS + "play.png"));
        pause();
    } else {
        parent->player_nav_playback->setIcon(QIcon(PATH_ICONS + "pause.png"));
        parent->player_title->setText(label);
        play();
    }
}

void MusicPlayer::onForwardClicked() {
    setPosition(position() + duration()/100);
}

void MusicPlayer::onNextTrackClicked() {
    stop();

    QModelIndex current = view->selectionModel()->currentIndex();

    if(current.sibling(current.row() + 1, 0).isValid()) {
        view->setCurrentIndex(current.sibling(current.row() + 1, 0));

        QStringList row;
        for (int col = 0; col < model->columnCount(); ++col) {
            row.append(current.sibling(current.row() + 1, col).data().toString());
        }

        trackinfo = row;
        path = row.at(row.size() - 1);
        label = "<b>" + row.at(0) + " - " + row.at(1) + "</b> [" + row.at(row.size() - 2) + "]";

        setMedia(QMediaContent(QUrl::fromLocalFile(path)));
    }

    onPlaybackClicked();
}

void MusicPlayer::onTrackDoubleClicked(const QModelIndex &current) {
    stop();

    QStringList row;
    for(int col = 0; col < parent->library->library_model->columnCount(); ++col) {
        row.append(current.sibling(current.row(), col).data().toString());
    }

    trackinfo = row;
    path = row.at(row.size() - 1);
    label = "<b>" + row.at(0) + " - " + row.at(1) + "</b> [" + row.at(row.size() - 2) + "]";

    setMedia(QMediaContent(QUrl::fromLocalFile(path)));

    onPlaybackClicked();
}

void MusicPlayer::onVolumeChanged(int val) {
    setVolume(val);
}

void MusicPlayer::onPositionChanged() {
    int val = parent->player_progress_slider->value();
    onPlaybackClicked();
    setPosition(val*1000);
    onPlaybackClicked();
    parent->player_progress_time_expired->setText(Audio::format(val/1000));
    parent->player_progress_time_left->setText(Audio::format(((int)duration() - val)/1000));
    parent->progress_slider_selected = false;
}

void MusicPlayer::onShuffleClicked() {
    parent->player_nav_shuffle->setChecked(parent->player_nav_shuffle->isChecked());
}

void MusicPlayer::onRepeatClicked() {
    parent->player_nav_repeat->setChecked(parent->player_nav_repeat->isChecked());
}

void MusicPlayer::onMuteClicked() {
    if(isMuted()) {
        setMuted(false);
        parent->player_nav_loudness->setIcon(QIcon(PATH_ICONS + "sound.png"));
    } else {
        setMuted(true);
        parent->player_nav_loudness->setIcon(QIcon(PATH_ICONS + "mute.png"));
    }
}

QString Audio::format(int duration) {
    QString res;
    auto seconds = duration % 60;
    duration /= 60;
    auto minutes = duration % 60;
    duration /= 60;
    auto hours = duration % 24;
    auto days = duration / 24;

    if((hours == 0) && (days == 0)) {
        return res.sprintf("%02d:%02d", minutes, seconds);
    }
    if (days == 0) {
        return res.sprintf("%02d:%02d:%02d", hours, minutes, seconds);
    }
    return res.sprintf("%dd%02d:%02d:%02d", days, hours, minutes, seconds);
}

int Audio::length(QString format) {
    QStringList time = format.split(":");
    int length = 0;
    if(time.length() == 4) {
        length = time.at(0).toInt() * 86400;
        length += time.at(1).toInt() * 3600;
        length += time.at(2).toInt() * 60;
        length += time.at(3).toInt();
    }
    if(time.length() == 3) {
        length = time.at(0).toInt() * 3600;
        length += time.at(1).toInt() * 60;
        length += time.at(2).toInt();
    }
    if(time.length() == 2) {
        length = time.at(0).toInt() * 60;
        length += time.at(1).toInt();
    }
    if(time.length() == 1) {
        length = time.at(0).toInt();
    }
    return length;
}