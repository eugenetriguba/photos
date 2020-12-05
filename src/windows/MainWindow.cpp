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

QStringList getSupportedMimeTypes(QFileDialog::AcceptMode acceptMode);
static void showImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode);

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
 * Reads an image, handles if the image is non-existant with
 * an error dialog box, updates the application state to that image,
 * and updates the status bar.
 */
bool MainWindow::openImage(const QString &filepath) {
    QImageReader reader(filepath);
    reader.setAutoTransform(true);

    const QImage image = reader.read();
    if (image.isNull()) {
        QMessageBox::information(
            this, QGuiApplication::applicationDisplayName(),
            tr("Cannot load %1: %2")
                .arg(QDir::toNativeSeparators(filepath), reader.errorString()));
        return false;
    }

    setImage(image);
    setWindowFilePath(filepath);

    const QString message = tr("Opened \"%1\", %2x%3")
                                .arg(QDir::toNativeSeparators(filepath))
                                .arg(image.width())
                                .arg(image.height());
    statusBar()->showMessage(message);

    return true;
}

void MainWindow::createActions() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &MainWindow::open);
    openAct->setShortcut(QKeySequence::Open);
    saveAsAct = fileMenu->addAction(tr("&Save As..."), this, &MainWindow::saveAs);
    saveAsAct->setEnabled(false);
    fileMenu->addSeparator();
    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
    cropAct = editMenu->addAction(tr("&Crop"), this, &MainWindow::crop);
    cropAct->setShortcut(tr("Shift+X"));
    cropAct->setEnabled(false);

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
 * Updates the enabled states of actions.
 *
 * This is useful for if we, say, click a checkbox
 * and want that to be updated on the screen so it
 * shows that it is in the enabled state.
 */
void MainWindow::updateActions() {
    saveAsAct->setEnabled(!image.isNull());
    cropAct->setEnabled(!image.isNull());
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

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

bool MainWindow::saveImage(const QString &filepath) {
    QImageWriter writer(filepath);

    if (!writer.write(image)) {
        QMessageBox::information(
            this, QGuiApplication::applicationDisplayName(),
            tr("Cannot write %1: %2").arg(QDir::toNativeSeparators(filepath)),
            writer.errorString());
        return false;
    }

    const QString message = tr("Saved \"%1\"").arg(
                QDir::toNativeSeparators(filepath));
    statusBar()->showMessage(message);

    return true;
}

void MainWindow::scaleImage(double factor) {
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap(Qt::ReturnByValue).size());

    adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor) {
    scrollBar->setValue(
        int(factor * scrollBar->value() + ((factor - 1) * scrollBar->pageStep() / 2)));
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
    // We make this bit of memory static here so
    // we know whether or not it is the first dialog
    // box in subsequent invocations.
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;

        const QStringList picturesDir = QStandardPaths::standardLocations(
                    QStandardPaths::PicturesLocation);

        if (picturesDir.isEmpty()) {
            dialog.setDirectory(QDir::currentPath());
        } else {
            dialog.setDirectory(picturesDir.last());
        }
    }

    QStringList mimeTypeFilters = getSupportedMimeTypes(acceptMode);
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");

    if (acceptMode == QFileDialog::AcceptSave) {
        dialog.setDefaultSuffix("jpg");
    }
}

/**
 * Private helper function for retrieving the supported
 * mime types that we can accept for a given accept mode
 * in a file dialog.
 *
 * Args:
 *   acceptMode: The accept mode for the file dialog such as
 *   if we are saving a file or opening a file.
 *
 * Returns:
 *   A QStringList of supported mime types for the given mode.
 */
QStringList getSupportedMimeTypes(QFileDialog::AcceptMode acceptMode) {
    QStringList mimeTypeFilters;
    QByteArrayList supportedMimeTypes;

    if (acceptMode == QFileDialog::AcceptOpen) {
        supportedMimeTypes = QImageReader::supportedMimeTypes();
    } else {
        // Assumption: if we're not opening a file, we're saving one.
        supportedMimeTypes = QImageWriter::supportedMimeTypes();
    }

    std::for_each(supportedMimeTypes.begin(),
                  supportedMimeTypes.end(),
    [&mimeTypeFilters](const QByteArray &mimeTypeName) {
        mimeTypeFilters.append(mimeTypeName);
    });
    mimeTypeFilters.sort();

    return mimeTypeFilters;
}

/**
 *
 * "Slot" Action Handlers
 *
 */

void MainWindow::open() {
    QFileDialog dialog(this, tr("Open File"));
    showImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted &&
           !openImage(dialog.selectedFiles().first())) {
    }
}

void MainWindow::saveAs() {
    QFileDialog dialog(this, tr("Save File As"));
    showImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted &&
           !saveImage(dialog.selectedFiles().first())) {
    }
}

void MainWindow::crop() {
    statusBar()->showMessage(tr("Invoked cropping!"));
}

void MainWindow::zoomIn() {
    scaleImage(1.25);
}

void MainWindow::zoomOut() {
    scaleImage(0.75);
}

void MainWindow::normalSize() {
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void MainWindow::fitToWindow() {
    bool fitToWindow = fitToWindowAct->isChecked();
    scrollArea->setWidgetResizable(fitToWindow);

    if (!fitToWindow) {
        normalSize();
    }

    updateActions();
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About Photos"),
                       tr("<p>The Photos application allows us to view images on our "
                          "computer and do light edits on them.</p>"));
}
