#include "MainWindow.hpp"
#include <QAction>
#include <QApplication>
#include <QColorSpace>
#include <QDir>
#include <QFileDialog>
#include <QImageReader>
#include <QImageWriter>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QStandardPaths>
#include <QStatusBar>

/**
 * Sets up the MainWindow.
 *
 * Args:
 *   parent: The parent window for this window.
 *   Defaults to a nullptr for the main window.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), imageLabel(new QLabel), scrollArea(new QScrollArea) {
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(imageLabel);
    scrollArea->setVisible(false);
    setCentralWidget(scrollArea);

    createActions();

    resize(QGuiApplication::primaryScreen()->availableSize() / 2);
}

/**
 * Create the menubar and keyboard shortcuts
 * for actions that the application can take.
 */
void MainWindow::createActions() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &MainWindow::open);
    openAct->setShortcut(QKeySequence::Open);
    saveAsAct = fileMenu->addAction(tr("&Save As..."), this, &MainWindow::saveAs);
    saveAsAct->setEnabled(false);
    fileMenu->addSeparator();
    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    zoomInAct = viewMenu->addAction(tr("Zoom &In (25%)"), this, &MainWindow::zoomIn);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setEnabled(false);
    zoomOutAct = viewMenu->addAction(tr("Zoom &Out (25%)"), this, &MainWindow::zoomOut);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setEnabled(false);
    normalSizeAct =
        viewMenu->addAction(tr("&Normal Size"), this, &MainWindow::normalSize);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);
    viewMenu->addSeparator();
    fitToWindowAct =
        viewMenu->addAction(tr("&Fit to Window"), this, &MainWindow::fitToWindow);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    helpMenu->addAction(tr("About &Qt"), &QApplication::aboutQt);
}

/**
 * Opens up a file to view.
 *
 * Args:
 *   filename: path to the image file to open.
 *
 * Returns:v
 *   true if we've successfully opened the file up.
 *   false otherwise.
 */
bool MainWindow::openFile(const QString &fileName) {
    QImageReader reader(fileName);
    reader.setAutoTransform(true);

    const QImage image = reader.read();
    if (image.isNull()) {
        QMessageBox::information(
            this, QGuiApplication::applicationDisplayName(),
            tr("Cannot load %1: %2")
                .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

    setImage(image);
    setWindowFilePath(fileName);

    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
                                .arg(QDir::toNativeSeparators(fileName))
                                .arg(image.width())
                                .arg(image.height())
                                .arg(image.depth());
    statusBar()->showMessage(message);

    return true;
}

/**
 * Set the image for this main window to show.
 *
 * Args:
 *   newImage: the new image to set this window's image to.
 */
void MainWindow::setImage(const QImage &newImage) {
    image = newImage;

    if (image.colorSpace().isValid()) {
        image.convertToColorSpace(QColorSpace::SRgb);
    }

    imageLabel->setPixmap(QPixmap::fromImage(image));
    scaleFactor = 1.0;

    scrollArea->setVisible(true);
    fitToWindowAct->setEnabled(true);
    updateActions();

    if (!fitToWindowAct->isChecked()) {
        imageLabel->adjustSize();
    }
}

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
bool MainWindow::saveFile(const QString &filepath) {
    QImageWriter writer(filepath);

    if (!writer.write(image)) {
        QMessageBox::information(
            this, QGuiApplication::applicationDisplayName(),
            tr("Cannot write %1: %2").arg(QDir::toNativeSeparators(filepath)),
            writer.errorString());
        return false;
    }

    const QString message = tr("Saved \"%1\"").arg(QDir::toNativeSeparators(filepath));
    statusBar()->showMessage(message);

    return true;
}

/**
 * Shows the dialog box for selecting an image file.
 *
 * Args:
 *   dialog: The file dialog box.
 *   acceptMode: The mode for the file. i.e. QFileDialog::AcceptOpen means
 *   we are opening a file rather than for writing out our file.
 */
static void showImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode) {
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations =
            QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath()
                                                        : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    QByteArrayList supportedMimeTypes;
    if (acceptMode == QFileDialog::AcceptOpen) {
        supportedMimeTypes = QImageReader::supportedMimeTypes();
    } else {
        // Assumption: if we're not opening a file, we're saving one.
        supportedMimeTypes = QImageWriter::supportedMimeTypes();
    }

    for (const QByteArray &mimeTypeName : supportedMimeTypes) {
        mimeTypeFilters.append(mimeTypeName);
    }
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");

    if (acceptMode == QFileDialog::AcceptSave) {
        dialog.setDefaultSuffix("jpg");
    }
}

/**
 * Handles the open() signal to create the open file dialog.
 */
void MainWindow::open() {
    QFileDialog dialog(this, tr("Open File"));
    showImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted &&
           !openFile(dialog.selectedFiles().first())) {
    }
}

/**
 * Handles the saveAs() signal to create the saving dialog.
 */
void MainWindow::saveAs() {
    QFileDialog dialog(this, tr("Save File As"));
    showImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted &&
           !saveFile(dialog.selectedFiles().first())) {
    }
}

/**
 * Zoom in the image by scaling the image down
 * and resizing it for our window.
 */
void MainWindow::zoomIn() { scaleImage(1.25); }

/**
 * Zoom out of the image by scaling the image up
 * and resizing it for our window.
 */
void MainWindow::zoomOut() { scaleImage(0.75); }

/**
 * Reset the image scale to the default.
 */
void MainWindow::normalSize() {
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

/**
 * Scale the image to fit the window if checked.
 * Otherwise, reset back to its normal size.
 */
void MainWindow::fitToWindow() {
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);

    if (!fitToWindow) {
        normalSize();
    }

    updateActions();
}

/**
 * Show a popup for the about information for this application.
 */
void MainWindow::about() {
    QMessageBox::about(this, tr("About Photos"),
                       tr("<p>The Photos application allows us to view images on our "
                          "computer and do light edits on them.</p>"));
}

/**
 * Updates the actions for the current state.
 *
 * This is useful for if we, say, check a checkbox
 * and want that to be updated on the screen.
 */
void MainWindow::updateActions() {
    saveAsAct->setEnabled(!image.isNull());
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

/**
 * Scale the image by a certain factor.
 * This is what allows us to zoom in and out
 * of an image.
 *
 * Args:
 *   factor: The amount to scale the image by.
 */
void MainWindow::scaleImage(double factor) {
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap(Qt::ReturnByValue).size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

/**
 * Adjust the scroll bar by a certain factor. This is
 * used for after we scale an image so the scroll bar
 * is approxriately sized.
 *
 * Args:
 *   scrollBar: The scroll bar to adjust.
 *   factor: The factor to adjust the scroll bar by.
 */
void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor) {
    scrollBar->setValue(
        int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep() / 2)));
}
