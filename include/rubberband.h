#ifndef RUBBERBAND_H
#define RUBBERBAND_H

#include <QRubberBand>

class RubberBand : public QRubberBand{

    public:
        RubberBand(QWidget* parent = nullptr);
        ~RubberBand();

        QRect mouseReleaseEvent(QPoint pos, QPoint fixedPos);
        void mousePressEvent(QPoint pos, QPoint fixedPos);
        void mouseMoveEvent(QPoint pos);
        QRect generateRectangle();
        int calculateWidth();
        int calculateHeight();
private:

        QPoint mOrigin;
        QPoint mEnding;
        QPoint mFixedOrigin;
        QPoint mFixedEnding;


};

#endif
