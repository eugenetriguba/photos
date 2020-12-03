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

    /**
     * Default constructor for the MainWindow.
     *
     * Args:
     *   parent: The parent window for this window. Defaults to a nullptr.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * Opens up an image to view.
     *
     * Args:
     *   filepath: path to the image file to open.
     *
     * Returns:
     *   true if we've successfully opened the image up.
     *   false otherwise.
     */
    bool openImage(const QString &filepath);

    // Slots for handling our application's actions. These
    // actions are all tied to items in the menu bar or keyboard
    // shortcuts.
  private slots:

    // Shows the dialog window for selecting an image to open.
    void open();

    // Shows the dialog window for saving an image.
    void saveAs();

    // Enters cropping mode so we can crop the image
    // using our mouse by creating a rectangle on the area.
    void crop();

    // Zoom in the image by scaling the image and resizing it
    // for our window. We can do this through the menu bar or
    // Ctrl++
    void zoomIn();

    // Zoom out of the image by scaling the image and resizing it
    // for our window. We can do this through the menu bar or
    // Ctrl+-
    void zoomOut();

    // Resets the image to the default scale factor.
    void normalSize();

    // Scales the image to fit the window size if checked.
    // Otherwise, reset back to the "normal size"
    void fitToWindow();

    // Show about information about the application.
    void about();

  private:
    Q_OBJECT

    /**
     * Create the menubar, keyboard shortcuts for actions,
     * and set the state of whether certain actions are enabled
     * or not yet.
     */
    void createActions();

    // Updates the enabled state of actions to the current state.
    void updateActions();

    /**
     * Set the image for this main window to show.
     *
     * Args:
     *   newImage: the new image to set this window's image to.
     */
    void setImage(const QImage &newImage);

    /**
     * Saves the current image and updates the status bar.
     * If we cannot save the image, an error dialog box
     * is shown.
     *
     * Args:
     *   filepath: The path where to save the image to.
     *
     * Returns:
     *   True if the image was able to be saved at the
     *   specified path; false otherwise.
     */
    bool saveImage(const QString &filepath);

    /**
     * Scale the image by a certain factor.
     * This is what allows us to zoom in and out
     * of an image.
     *
     * Args:
     *   factor: The amount to scale the image by.
     */
    void scaleImage(double factor);

    /**
     * Adjust the scroll bar by a certain factor. This is
     * used for after we scale an image so the scroll bar
     * is approxriately sized.
     *
     * Args:
     *   scrollBar: The scroll bar to adjust.
     *   factor: The factor to adjust the scroll bar by.
     */
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    QImage image;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    double scaleFactor = 1;

    QAction *saveAsAct;
    QAction *cropAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
};

#endif
