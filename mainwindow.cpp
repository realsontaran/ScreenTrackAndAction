#include "mainwindow.h"
#include <QScreen>
#include <QDebug>
#include <QRegularExpression>
#include "capturemouse.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  reg = new RegionSelect();
  bwl = new BottomWidget(true);
  bwr = new BottomWidget(false);
  ui->setupUi(this);
  ui->doneBtn->hide();
  ui->doneBtn->setDisabled(true);
  QList<QScreen*> list = QGuiApplication::screens();
  QRect rect;
  for(auto scr : list) {
   rect = rect.united(scr->geometry());
  }
  reg->setGeometry(rect);
  timerID = startTimer(16);
  start = false;
  ocr = new tesseract::TessBaseAPI();
  ocr->Init(NULL, "eng", tesseract::OEM_LSTM_ONLY);
  ocr->SetPageSegMode(tesseract::PSM_AUTO);
  cap = new CaptureMouse();
  recognizer = new RecognizeGesture();
}

MainWindow::~MainWindow() {
  killTimer(timerID);
  delete ui;
  delete reg;
  delete bwl;
  delete bwr;
  delete ocr;
  delete cap;
}

void MainWindow::on_regionButton_clicked() {
  reg->show();
  ui->doneBtn->setEnabled(true);
}

void MainWindow::action() {
  switch (ui->actionCombo->currentIndex()) {
  case 1: {
    qInfo() << "shutdown now";
    QMessageBox::warning(this, tr("Action Dialog"), tr("Shutdown Triggered"),
                         QMessageBox::Ok);
    break;
  }
  case 2: {
    qInfo() << "reboot";
    QMessageBox::warning(this, tr("Action Dialog"), tr("Restart Triggered"),
                         QMessageBox::Ok);
    break;
  }
  case 3: {
    qInfo() << "systemctl suspend";
    QMessageBox::warning(this, tr("Action Dialog"), tr("Suspend Triggered"),
                         QMessageBox::Ok);
    break;
  }
  case 4: {
    QStringList list = QProcess::splitCommand(bwr->get_str());
    QString cm = list[0];
    list.remove(0);
    QProcess::startDetached(cm,list);
    break;
  }
  case 5: {
    xdo_t *xdo = xdo_new(NULL);
    xdo_enter_text_window(xdo, CURRENTWINDOW,
                          bwr->get_str().toStdString().c_str(), 1200);
    xdo_free(xdo);
    break;
  }
  case 6: {
    xdo_t *xdo = xdo_new(NULL);
    std::vector<std::string> coord;
    std::string text = bwr->get_str().toStdString();
    std::string delimiter = ",";
    size_t last = 0;
    size_t next = 0;
    while ((next = text.find(delimiter, last)) != std::string::npos) {
      coord.push_back(text.substr(last, next - last));
      last = next + 1;
    }
    coord.push_back(text.substr(last));
    xdo_move_mouse(xdo, std::stoi(coord[0]), std::stoi(coord[1]), 0);
    xdo_free(xdo);
    break;
  }
  case 7: {
    xdo_t *xdo = xdo_new(NULL);
    QRegularExpression rx("(\\ |\\,|\\.|\\:|\\t)"); //RegEx for ' ' or ',' or '.' or ':' or '\t'
    QStringList coord = bwr->get_str().split(rx);

    xdo_move_mouse(xdo, coord[0].toInt(), coord[1].toInt(), 0);
    xdo_mouse_down(xdo, CURRENTWINDOW, coord[2].toInt()+1);
    xdo_mouse_up(xdo, CURRENTWINDOW, coord[2].toInt()+1);
    xdo_free(xdo);
    break;
  }
  case 8: {
    QImage current = regionPix.get_region(reg->getRect())
                         .toImage()
                         .convertToFormat(QImage::Format_Grayscale8);
    PIX *currPix = imageToPix(current);

    ocr->SetImage(currPix);
    QString text = QString(ocr->GetUTF8Text());
    QClipboard *clip = QApplication::clipboard();
    clip->setText(text);
    QMessageBox::warning(this, tr("Action Dialog"), tr("Text Copied to Clipboard"),
                         QMessageBox::Ok);
    delete currPix;
    break;
  }
  case 9: {
    QFile ss("screenshot.png");
    QImage current = regionPix.get_region(reg->getRect()).toImage();
    current.save(&ss, "PNG");
    QMessageBox::warning(this, tr("Action Dialog"), tr("Screenshot Saved"),
                         QMessageBox::Ok);
    break;
  }
  }
  killTimer(timerID);
  timerID = 0;
}

PIX *MainWindow::imageToPix(QImage &qImage) {
  PIX *pixs;
  l_uint32 *lines;

  qImage = qImage.rgbSwapped();
  int width = qImage.width();
  int height = qImage.height();
  int depth = qImage.depth();
  int wpl = qImage.bytesPerLine() / 4;

  pixs = pixCreate(width, height, depth);
  pixSetWpl(pixs, wpl);
  pixSetColormap(pixs, NULL);
  l_uint32 *datas = pixs->data;

  for (int y = 0; y < height; y++) {
    lines = datas + y * wpl;
    QByteArray a((const char *)qImage.scanLine(y), qImage.bytesPerLine());
    for (int j = 0; j < a.size(); j++) {
      *((l_uint8 *)lines + j) = a[j];
    }
  }
  return pixEndianByteSwapNew(pixs);
}

void MainWindow::timerEvent(QTimerEvent *event) {
  if (start) {
    if (ui->trackCombo->currentIndex() == 6) {
      if (reg->getRect().contains(QCursor::pos())) {
        action();
      }
    }
    if (ui->trackCombo->currentIndex() == 5) {
      if (reg->getRect().contains(QCursor::pos())) {
        action();
      }
    }
    if (ui->trackCombo->currentIndex() == 4) {
      bool eq = true;
      if (!prevPix.isNull()) {
        QImage current = regionPix.get_region(reg->getRect()).toImage();
        QImage prev = prevPix.toImage();
        eq = (current == prev);
      }
      if (!eq) {
        action();
      }
      prevPix = regionPix.get_region(reg->getRect());
    }
    if (ui->trackCombo->currentIndex() == 3) {
      QImage current = regionPix.get_region(reg->getRect())
                           .toImage()
                           .convertToFormat(QImage::Format_Grayscale8);
      PIX *currPix = imageToPix(current);

      ocr->SetImage(currPix);
      std::string ocrResult(ocr->GetUTF8Text());
      if (ocrResult.find(bwl->get_str().toStdString()) != std::string::npos) {
        action();
      }
      delete currPix;
    }
    if (ui->trackCombo->currentIndex() == 2) {
      QColor color(bwl->get_str().right(7));
      QImage img = regionPix.get_region(reg->getRect()).toImage();
      for (int row = 0; row < img.height(); ++row) {
        for (int col = 0; col < img.width(); ++col) {
          if (color == img.pixelColor(col, row)) {
            action();
            return;
          }
        }
      }
    }
    if (ui->trackCombo->currentIndex() == 1) {
        QImage newDraw;
        QImage prev = bwl->get_gesture();
        recognizer->setGesture(prev);
        bool catched = false;
        if(reg->getRect().contains(QCursor::pos()))
            catched = cap->captureMouse(newDraw,reg->getRect());
        if(catched){
            if(recognizer->doesMatch(newDraw)){
                action();
            }
        }
    }
  }
}

void MainWindow::on_doneBtn_clicked() {
  bwl->update_data(ui->trackCombo->currentIndex());
  bwr->update_data(ui->actionCombo->currentIndex());
  regionPix.get_region(reg->getRect());
  start = true;
  if (timerID == 0) {
    timerID = startTimer(16);
  }
}

void MainWindow::on_trackCombo_currentIndexChanged(int index) {
  bwl->init(index);
  ui->hl1->addWidget(bwl);
  if (index != 0) {
    leftStr = bwl->get_str();
  }
  if (ui->actionCombo->currentIndex() != 0 && index != 0) {
    ui->doneBtn->show();
  } else {
    ui->doneBtn->hide();
  }
}

void MainWindow::on_actionCombo_currentIndexChanged(int index) {
  bwr->init(index);
  ui->hl2->addWidget(bwr);
  if (index != 0) {
    rightStr = bwr->get_str();
  }
  if (ui->trackCombo->currentIndex() != 0 && index != 0) {
    ui->doneBtn->show();
  } else {
    ui->doneBtn->hide();
  }
}
