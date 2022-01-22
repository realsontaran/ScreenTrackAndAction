#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "./ui_mainwindow.h"
#include "bottomwidget.h"
#include "getpixmap.h"
#include "regionselect.h"
#include "scribblearea.h"
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QMainWindow>
#include <QMessageBox>
#include <QProcess>
#include <QRect>
#include <QThread>
#include <bits/stdc++.h>
#include <leptonica/allheaders.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <qapplication.h>
#include <qcoreevent.h>
#include <qcursor.h>
#include <QEvent>
#include <QFile>
#include <QtGlobal>
#include <QMessageBox>
#include <QPixmap>
#include <string>
#include <tesseract/baseapi.h>
#include <vector>
#include <xdo.h>
#include "capturemouse.h"
#ifdef Success
  #undef Success
#endif
#include "recognizegesture.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_regionButton_clicked();
  void on_doneBtn_clicked();
  void on_trackCombo_currentIndexChanged(int index);
  void on_actionCombo_currentIndexChanged(int index);

protected:
  void timerEvent(QTimerEvent *event);
  PIX *imageToPix(QImage &img);
  void action();

private:
  Qt::MouseButton mouseClickedBtnFlag;
  int timerID;
  bool start;
  Ui::MainWindow *ui;
  RegionSelect *reg;
  GetPixmap regionPix;
  QPixmap prevPix;
  BottomWidget *bwl;
  BottomWidget *bwr;
  QString leftStr, rightStr;
  tesseract::TessBaseAPI *ocr;
  CaptureMouse *cap;
  RecognizeGesture *recognizer;
};
#endif // MAINWINDOW_H
