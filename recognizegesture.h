#ifndef RECOGNIZEGESTURE_H
#define RECOGNIZEGESTURE_H
#include <QImage>
#include <cstddef>
#include <fdeep/fdeep.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class RecognizeGesture {
public:
  RecognizeGesture();
  ~RecognizeGesture();
  bool doesMatch(QImage gesture);
  void setGesture(QImage image);

private:
  QImage image;
  std::optional<fdeep::model> model;
  std::size_t result;
  cv::Mat prepareImage(QImage &img, int format);
  QRect getBoundsWithoutColor(QImage qImage);
  fdeep::tensor getTensor(cv::Mat cvmat);
};

#endif // RECOGNIZEGESTURE_H
