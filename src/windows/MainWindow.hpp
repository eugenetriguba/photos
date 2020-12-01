#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QImage>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  public:
    MainWindow(QWidget *parent = nullptr);
    bool openFile(const QString &);

    // Slots for handling our application's actions.
  private slots:
    void open();
    void saveAs();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void about();

  private:
    Q_OBJECT

    void createActions();
    void createMenus();
    void updateActions();
    bool saveFile(const QString &filepath);
    void setImage(const QImage &newImage);
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    QImage image;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor = 1;

    QAction *saveAsAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
};

#endif
