#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);

  private:
    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    QLabel *infoLabel;

    QAction *openAction;
    QAction *saveAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
    QAction *exitAction;

  private slots:
    void openFile();
    void save();
    void undo();
    void redo();
    void about();
    void aboutQt();
    void exit();
};
#endif // MAINWINDOW_HPP
