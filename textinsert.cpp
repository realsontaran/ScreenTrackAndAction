#include "textinsert.h"
#include <QHBoxLayout>

TextInsert::TextInsert(QWidget *parent) : QWidget(parent) {
  label = new QLabel();
  text_edit = new QTextEdit();
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->addWidget(label);
  layout->addWidget(text_edit);
}

TextInsert::~TextInsert(){
  delete label;
  delete text_edit;
}

void TextInsert::set_label(QString str) { label->setText(str); }
QString TextInsert::get_text() { return text_edit->toPlainText(); }
