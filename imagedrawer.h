#ifndef IMAGEDRAWER_H
#define IMAGEDRAWER_H

#include <QWidget>
#include <QLabel>
#include <QResizeEvent>
#include <QPixmap>
#include <QtGlobal>
#include <QVBoxLayout>

class ImageDrawer : public QWidget
{
    Q_OBJECT

public:
    ImageDrawer(QWidget *parent = 0);
    ~ImageDrawer();

private:
    QLabel *imgLabel;
    QVBoxLayout *mainLayout;
    QPixmap img;

protected:
    void resizeEvent(QResizeEvent *event) override;

signals:

public slots:
    void setImage(QPixmap image);
};

#endif // IMAGEDRAWER_H
