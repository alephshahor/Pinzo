#include "../include/rubberband.h"
#include <QMouseEvent>
#include <QLabel>
#include <cmath>

RubberBand::RubberBand(QWidget* parent):
    QRubberBand(QRubberBand::Rectangle, parent)
{
    //setAttribute(Qt::WA_TranslucentBackground, true);
}

RubberBand::~RubberBand()
{

}

void RubberBand::mousePressEvent(QPoint pos, QPoint fixedPos)
{

    mOrigin = pos;
    mFixedOrigin = fixedPos;
    this -> setGeometry(QRect(mOrigin, QSize()));
    this -> show();

}

void RubberBand::mouseMoveEvent(QPoint pos)
{
    this -> setGeometry(QRect(mOrigin, pos).normalized());
}

QRect RubberBand::mouseReleaseEvent(QPoint pos, QPoint fixedPos)
{
    this -> hide();
    mEnding = pos;
    mFixedEnding = fixedPos;
    return generateRectangle();
}

QRect RubberBand::generateRectangle()
{

    if((mFixedOrigin.y() > mFixedEnding.y()) && (mFixedOrigin.x() > mFixedEnding.x())){
        return QRect(mFixedEnding.x(), mFixedEnding.y(), calculateWidth() , calculateHeight());
    }else if((mFixedOrigin.y() < mFixedEnding.y()) && (mFixedOrigin.x() > mFixedEnding.x())){
        return QRect(mFixedEnding.x(), mFixedOrigin.y(), calculateWidth() , calculateHeight());
    }else if((mFixedOrigin.y() > mFixedEnding.y()) && (mFixedOrigin.x() < mFixedEnding.x())){
        return QRect(mFixedOrigin.x(), mFixedEnding.y(), calculateWidth() , calculateHeight());
    }else if((mFixedOrigin.y() < mFixedEnding.y()) && (mFixedOrigin.x() < mFixedEnding.x())){
        return QRect(mFixedOrigin.x(), mFixedOrigin.y(), calculateWidth() , calculateHeight());
    }else {
        throw "Can't generate rectangle\n";
    }
}

int RubberBand::calculateWidth(){
    int width = abs(mFixedOrigin.x() - mFixedEnding.x());
    return width;
}

int RubberBand::calculateHeight(){
    int height = abs(mFixedOrigin.y() - mFixedEnding.y());
    return height;
}


