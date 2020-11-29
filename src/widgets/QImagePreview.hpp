#ifndef QIMAGEPREVIEW_H
#define QIMAGEPREVIEW_H

#include <QLabel>
#include <opencv2/core.hpp>

class QImagePreview : public QLabel {
  public:
    QImagePreview(QWidget *parent);
    void loadImage(const QString &path);
    void updatePreview();

  private:
    Q_OBJECT

    QWidget *parent;
    cv::Mat imageSource;
    cv::Mat imageBuffer;
    QImage::Format activeFormat;
};

#endif
