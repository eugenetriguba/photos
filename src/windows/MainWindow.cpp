#include "MainWindow.hpp"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    infoLabel = new QLabel(tr("<i>Choose a menu option, or right-click to "
                              "invoke a context menu</i>"));
    infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    infoLabel->setAlignment(Qt::AlignCenter);

    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addWidget(topFiller);
    layout->addWidget(infoLabel);
    layout->addWidget(bottomFiller);
    widget->setLayout(layout);

    createActions();
    createMenus();

    setWindowTitle("Photos");
    setMinimumSize(160, 160);
    resize(480, 320);
}

void MainWindow::createActions() {
    openAction = new QAction(tr("&Open"), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an image to preview and edit"));
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcuts(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save your image edits"));
    connect(saveAction, &QAction::triggered, this, &MainWindow::save);

    undoAction = new QAction(tr("&Undo"), this);
    undoAction->setShortcuts(QKeySequence::Undo);
    undoAction->setStatusTip(tr("Undo an image edit"));
    connect(undoAction, &QAction::triggered, this, &MainWindow::undo);

    redoAction = new QAction(tr("&Redo"), this);
    redoAction->setShortcuts(QKeySequence::Redo);
    redoAction->setStatusTip(tr("Redo an image edit"));
    connect(redoAction, &QAction::triggered, this, &MainWindow::redo);

    aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);

    aboutQtAction = new QAction(tr("&AboutQt"), this);
    connect(aboutQtAction, &QAction::triggered, this, &MainWindow::aboutQt);

    exitAction = new QAction(tr("&Exit"), this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::exit);
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void MainWindow::openFile() {
    QString filepath = QFileDialog::getOpenFileName(this);
    //    if (!filepath.isEmpty()) {
    //        ui->image->LoadImage(filepath);
    //        ui->menuEffects->setEnabled(true);
    //    }
    infoLabel->setText(tr("Invoked <b>File|Open</b>"));
}

void MainWindow::save() { infoLabel->setText(tr("Invoked <b>File|Save</b>")); }

void MainWindow::undo() { infoLabel->setText(tr("Invoked <b>Edit|Undo</b>")); }

void MainWindow::redo() { infoLabel->setText(tr("Invoked <b>Edit|Redo</b>")); }

void MainWindow::about() {
    infoLabel->setText(tr("Invoked <b>Help|About</b>"));
    QMessageBox::about(this, tr("About Menu"),
                       tr("The <b>Menu</b> example shows how to create "
                          "menu-bar menus and context menus."));
}

void MainWindow::aboutQt() { infoLabel->setText(tr("Invoked <b>Help|About Qt</b>")); }

void MainWindow::exit() { infoLabel->setText(tr("Invoked <b>File|Exit</b>")); }
