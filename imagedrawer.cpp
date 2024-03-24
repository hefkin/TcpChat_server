#include "imagedrawer.h"

ImageDrawer::ImageDrawer(QWidget *parent) : QWidget(parent)
{
    imgLabel = new QLabel();
    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(imgLabel);
}

ImageDrawer::~ImageDrawer()
{

}

void ImageDrawer::resizeEvent(QResizeEvent *event)
{
    int w = imgLabel->width();
    int h = imgLabel->height();
    imgLabel->setPixmap(img.scaled(w, h, Qt::KeepAspectRatio));
    QWidget::resizeEvent(event);
}

void ImageDrawer::setImage(QPixmap image)
{
    img = image;
    imgLabel->setPixmap(img);
}
