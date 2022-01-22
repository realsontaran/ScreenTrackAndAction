#include "recognizegesture.h"
#include <QColor>
#include <QRect>
#include <QDebug>
#include <cmath>
RecognizeGesture::RecognizeGesture() {
  model = fdeep::load_model("model/fdeep_model.json");
}
QRect RecognizeGesture::getBoundsWithoutColor(QImage qImage)
{
    QRect ofTheKing;

    int maxX = 0;
    int minX = qImage.width();
    int maxY = 0;
    int minY = qImage.height();

    for(int x=0; x < qImage.width(); x++)
        for(int y=0; y < qImage.height(); y++)
            if (QColor::fromRgb(qImage.pixel(x, y)) != Qt::white)
            {
                if(x < minX) minX = x;
                if(x > maxX) maxX = x;
                if(y < minY) minY = y;
                if(y > maxY) maxY = y;
            }

    if (minX > maxX || minY > maxY){
        // The whole picture is white. How you wanna handle this case is up to you.
    }else{
        ofTheKing.setCoords(abs(minX-10), abs(minY-10), maxX+10, maxY+10);
    }
    return ofTheKing;
 }
cv::Mat RecognizeGesture::prepareImage(QImage &image, int format) {
  QImage cropped = image.copy(getBoundsWithoutColor(image));
  cv::Mat A(cropped.height(),cropped.width(),CV_8UC4,(void *)cropped.constBits(),cropped.bytesPerLine());
  cv::Mat newA;
  cv::resize(A, newA, cv::Size(28, 28));
  cv::cvtColor(newA,newA,CV_BGRA2GRAY);
  cv::bitwise_not(newA,newA);
  return newA;
}

void RecognizeGesture::setGesture(QImage image) {
    cv::Mat cvmat = prepareImage(image, CV_8UC3);
    const auto input = getTensor(cvmat);
    result = model->predict_class({input});
    qInfo() << result;
}

bool RecognizeGesture::doesMatch(QImage gesture) {
  cv::Mat gest = prepareImage(gesture, CV_8UC3);
  cv::imwrite("newDraw.png",gest);
  return result == model->predict_class({getTensor(gest)});
}

fdeep::tensor RecognizeGesture::getTensor(cv::Mat cvmat) {
  return fdeep::tensor_from_bytes(
      cvmat.ptr(), static_cast<std::size_t>(cvmat.rows),
      static_cast<std::size_t>(cvmat.cols),
      static_cast<std::size_t>(cvmat.channels()), 0.0f, 1.0f);
}
