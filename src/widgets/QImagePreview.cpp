#include <QInputEvent>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "QImagePreview.hpp"

QImagePreview::QImagePreview(QWidget *parent) : parent(parent) {
    setMouseTracking(true);
}

void QImagePreview::loadImage(const QString &path) {
    imageSource = cv::imread(path.toStdString());
    cv::cvtColor(imageSource, imageSource, cv::COLOR_BGR2RGB);
    imageBuffer = imageSource.clone();
    activeFormat = QImage::Format_RGB888;
}

void QImagePreview::updatePreview() {
    setPixmap(QPixmap::fromImage(QImage(
        imageBuffer.data, imageBuffer.cols, imageBuffer.rows, (int)imageBuffer.step, activeFormat)));
}
