#include "LineNumberArea.h"

LineNumberArea::LineNumberArea(CodeEditor *editor) : QWidget(editor)
{
  this->codeEditor = editor;
}

QSize LineNumberArea::sizeHint() const
{
  return QSize(this->codeEditor->lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
  this->codeEditor->lineNumberAreaPaintEvent(event);
}
