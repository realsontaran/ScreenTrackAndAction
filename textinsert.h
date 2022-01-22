#ifndef TEXTINSERT_H
#define TEXTINSERT_H

#include <QLabel>
#include <QTextEdit>
#include <QWidget>

class TextInsert : public QWidget {
  Q_OBJECT
public:
  TextInsert(QWidget *parent = nullptr);
  ~TextInsert();
  void set_label(QString str);
  void set_size(int w, int h) { text_edit->setFixedSize(w, h); }
  QString get_text();

private:
  QLabel *label;
  QTextEdit *text_edit;
};

#endif // TEXTINSERT_H
