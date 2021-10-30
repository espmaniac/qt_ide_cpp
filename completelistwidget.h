#ifndef COMPLETELISTWIDGET_H
#define COMPLETELISTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QPlainTextEdit>
#include <QApplication>
#include <vector>
#include <string>
#include <cmath>
using std::string;
using std::vector;
using std::min;
class CompleteListWidget : public QListWidget
{
public:
  explicit CompleteListWidget(QWidget *parent=nullptr);
  CompleteListWidget (const CompleteListWidget&) = delete;
  CompleteListWidget& operator = (const CompleteListWidget&) = delete;
  ~CompleteListWidget() = default;
  static int ldistance(const string source, const string target);
protected:
  void keyPressEvent(QKeyEvent *event) override;
private:
  QPlainTextEdit* p;
  QColor backgroundColor;
  QColor highlightColor;
};


#endif // COMPLETELISTWIDGET_H
