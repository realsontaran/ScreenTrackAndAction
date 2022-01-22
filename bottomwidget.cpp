#include "bottomwidget.h"
#include "scribblearea.h"
#include "textinsert.h"
#include <QComboBox>
#include <QDebug>
#include <QFormLayout>
#include <QLayoutItem>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qwidget.h>

BottomWidget::BottomWidget(bool left, QWidget *parent) : QWidget(parent) {
  layout = new QHBoxLayout(this);
  layout->setAlignment(Qt::AlignCenter);
  this->isLeft = left;
  wid = new QLabel();
  scribble = new ScribbleArea();
}

BottomWidget::~BottomWidget() { delete wid; }

void clearLayout(QLayout *layout) {
  if (layout == NULL)
    return;
  QLayoutItem *item;
  while ((item = layout->takeAt(0))) {
    if (item->layout()) {
      clearLayout(item->layout());
      delete item->layout();
    }
    if (item->widget()) {
      delete item->widget();
    }
    delete item;
  }
}

void BottomWidget::init(int index) {
  clearLayout(layout);
  if (!layout) {
    layout = new QHBoxLayout(this);
  }
  if (isLeft) {
    switch (index) {
    case 0:
      wid = new QLabel();
      break;
    case 1: {
      QPushButton *button = new QPushButton();
      button->setText("Draw Gesture");
      wid = button;
      connect(button, &QPushButton::clicked, [this]() {
        scribble = new ScribbleArea();
        scribble->show();
        gestureImage = scribble->getImage();
      });
      break;
    }
    case 2: {
      QPushButton *button = new QPushButton();
      button->setText("Choose Color");
      wid = button;
      connect(button, &QPushButton::clicked, [this]() {
        QColor color = QColorDialog::getColor(Qt::white, this, "Choose Color");
        if (color.isValid()) {
          QString qss = QString("background-color: %1").arg(color.name());
          wid->setStyleSheet(qss);
        }
      });
      break;
    }
    case 3: {
      QFormLayout *com = new QFormLayout();
      QTextEdit *tEdit = new QTextEdit();
      tEdit->setFixedSize(100, 30);
      com->addRow(tr("Detect Text:"), tEdit);
      wid = new QWidget();
      wid->setLayout(com);
      break;
    }
    case 4: {
      QLabel *label = new QLabel();
      label->setText("Update of region as trigger");
      wid = label;
      break;
    }

    case 5: {
      QComboBox *combo = new QComboBox();
      combo->addItem("Left Button");
      combo->addItem("Middle Button");
      combo->addItem("Right Button");
      wid = combo;
      break;
    }
    case 6: {
      QLabel *label = new QLabel();
      label->setText("Mouse Position");
      wid = label;
      break;
    }
    }
  } else {
    switch (index) {
    case 0:
      wid = new QLabel();
      break;
    case 1: {
      QLabel *label = new QLabel();
      label->setText("Shutdown");
      wid = label;
      break;
    }
    case 2: {
      QLabel *label = new QLabel();
      label->setText("Restart");
      wid = label;
      break;
    }
    case 3: {
      QLabel *label = new QLabel();
      label->setText("Sleep");
      wid = label;
      break;
    }
    case 4: {
      QFormLayout *com = new QFormLayout();
      QTextEdit *tEdit = new QTextEdit();
      tEdit->setFixedSize(100, 30);
      com->addRow(tr("Command:"), tEdit);
      wid = new QWidget();
      wid->setLayout(com);
      break;
    }
    case 5: {
      QFormLayout *com = new QFormLayout();
      QTextEdit *tEdit = new QTextEdit();
      com->addRow(tr("Text to Insert:"), tEdit);
      wid = new QWidget();
      wid->setLayout(com);
      break;
    }
    case 6: {
      QFormLayout *com = new QFormLayout();
      QTextEdit *tEdit = new QTextEdit();
      tEdit->setFixedSize(100, 30);
      QTextEdit *tEdit2 = new QTextEdit();
      tEdit2->setFixedSize(100, 30);
      com->addRow(tr("X:"), tEdit);
      com->addRow(tr("Y: "), tEdit2);
      wid = new QWidget();
      wid->setLayout(com);

      break;
    }
    case 7: {
      QFormLayout *com = new QFormLayout();
      QTextEdit *tEdit = new QTextEdit();
      tEdit->setFixedSize(100, 30);
      QTextEdit *tEdit2 = new QTextEdit();
      tEdit2->setFixedSize(100, 30);
      com->addRow(tr("X:"), tEdit);
      com->addRow(tr("Y:"), tEdit2);
      QComboBox *combo = new QComboBox();
      combo->addItem("Left Button");
      combo->addItem("Middle Button");
      combo->addItem("Right Button");
      com->addRow(combo);
      wid = new QWidget();
      wid->setLayout(com);
      break;
    }
    case 8: {
      QLabel *label = new QLabel();
      label->setText("Text will be copied from region");
      wid = label;
      break;
    }
    case 9: {
      QLabel *label = new QLabel();
      label->setText("Screenshot of region");
      wid = label;
      break;
    }
    case 10: {
      QLabel *label = new QLabel();
      label->setText("Screencapture of region");
      wid = label;
      break;
    }
    }
  }
  if (wid) {
    layout->addWidget(wid);
  }
}

void BottomWidget::update_data(int index) {

  if (isLeft) {
    switch (index) {
    case 1:
      str = "Gesture";
      gestureImage = scribble->getImage();
      break;
    case 2:
      str = wid->styleSheet().right(7);
      break;
    case 3:
      str = ((QTextEdit *)wid->children().at(2))->toPlainText();
      break;
    case 4:
      str = "Update Region";
      break;
    case 5:
      str = QString::number(((QComboBox *)wid)->currentIndex()).right(1);
      break;
    case 6:
      str = "Mouse Position";
      break;
    }
  } else {
    switch (index) {
    case 1:
      str = "Shutdown";
      break;
    case 2:
      str = "Restart";
      break;
    case 3:
      str = "Sleep";
      break;
    case 4:
      str = ((QTextEdit *)wid->children().at(2))->toPlainText();
      break;
    case 5:
      str = ((QTextEdit *)wid->children().at(2))->toPlainText();
      break;
    case 6:
      str = ((QTextEdit *)wid->children().at(2))->toPlainText();
      str += "," + ((QTextEdit *)wid->children().at(4))->toPlainText();
      break;
    case 7:
      str = ((QTextEdit *)wid->children().at(2))->toPlainText();
      str += "," + ((QTextEdit *)wid->children().at(4))->toPlainText();
      str +=
        "," + QString::number(((QComboBox *)wid->children().at(5))->currentIndex());
      break;
    case 8:
      str = "Copy from Region";
      break;
    case 9:
      str = "Screenshot of Region";
      break;
    case 10:
      str = "Screen record of Region";
      break;
    }
  }
}
