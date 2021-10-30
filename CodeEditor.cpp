#include "CodeEditor.h"
#include "completelistwidget.h"
#include "LineNumberArea.h"


CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent),
  lineNumberArea(new LineNumberArea(this))
{
  Highlighter *highlighter = new Highlighter(this->document());
  connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth()));
  connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
  connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
  this->setStyleSheet("color: #fff; font-size: 18px !important;");
  this->setTabStopWidth(18);
  updateLineNumberAreaWidth();
  highlightCurrentLine();

  this->typelist << "bool" << "void" << "char" << "int" << "short" << "long" << "signed" << "unsigned" << "float" << "double"
                 << "class" << "struct" << "union" << "const" << "constexpr" << "static" << "mutable" << "register" <<
                    "volatile" << "typename" << "typeid" << "typedef" << "using" << "friend" << "inline" << "auto" <<
                    "final" << "override" << "override" << "decltype" << "restrict" << "explicit" << "virtual" << "alignas"
                 << "consteval" << "extern" << "noexcept";

  this->typelist << "int8_t" << "uint8_t" << "int16_t" << "uint16_t" << "int32_t" << "uint32_t"
                 << "int64_t" << "uint64_t" << "wchar_t" << "size_t";

  this->wordlist << this->typelist;

  this->wordlist << "if" << "else" << "switch" << "case" << "default" << "try" << "catch";

  this->wordlist << "elif" << "ifdef" << "ifndef" << "define" << "line" << "error" << "pragma" <<
                    "include" << "undef" << "endif";

  this->wordlist << "for" << "while" << "do";

  this->wordlist << "const_cast" << "static_cast" << "dynamic_cast" << "reinterpret_cast";

  this->wordlist << "nullptr";

  this->wordlist << "public" << "private" << "protected";

  this->wordlist << "this" << "namespace";

  this->wordlist << "continue" << "break" << "return";

  this->wordlist << "malloc" << "new" << "delete" << "sizeof";

  for (qregisteruint i(0); i < wordlist.count(); ++i) {
      forever {
        qregisteruint j = wordlist.lastIndexOf(wordlist.at(i));
        if (j == i) break;
        wordlist.removeAt(j);
      }
  }


}

int32_t CodeEditor::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());

    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    uint32_t space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;
    return space;
}

void CodeEditor::updateLineNumberAreaWidth() {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int32_t dy) {
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth();
}

void CodeEditor::resizeEvent(QResizeEvent *e) {
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        selection.format.setBackground(QColor("#2e2e2e"));
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }
    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(this->lineNumberArea);
    painter.fillRect(event->rect(), QColor("#1E1E1E"));

    QTextBlock block = firstVisibleBlock();

    int blockNumber = block.blockNumber();
    int top = blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor("#2B91AF"));
            QFont bold("Sans-Serif", 12);
            bold.setBold(true);
            painter.setFont(bold);
            painter.drawText(1, top,
                             lineNumberArea->width(),
                             fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + blockBoundingRect(block).height();
        ++blockNumber;
    }
}


void CodeEditor::focusInEvent(QFocusEvent *e)
{
    if (c)
        c->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}

void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    if (c && c->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (e->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    }

    const bool isShortcut = (e->modifiers().testFlag(Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!c || !isShortcut) // do not process the shortcut when we have a completer
        QPlainTextEdit::keyPressEvent(e);
    const bool ctrlOrShift = e->modifiers().testFlag(Qt::ControlModifier) ||
                               e->modifiers().testFlag(Qt::ShiftModifier);
      if (!c || (ctrlOrShift && e->text().isEmpty()))
          return;

      static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
      const bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
      QString completionPrefix = textUnderCursor();

      if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 2
                        || eow.contains(e->text().right(1)))) {
          c->popup()->hide();
          return;
      }

      if (completionPrefix != c->completionPrefix()) {
          c->setCompletionPrefix(completionPrefix);
          c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
      }
      QRect cr = cursorRect();
      cr.setWidth(c->popup()->sizeHintForColumn(0)
                  + c->popup()->verticalScrollBar()->sizeHint().width());
      c->complete(cr); // popup it up!
  }

QString CodeEditor::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}


void CodeEditor::insertCompletion(const QString &completion)
{
    if (c->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}



QCompleter *CodeEditor::completer() const
{
    return c;
}


void CodeEditor::setCompleter(QCompleter *completer)
{
    if (c)
        c->disconnect(this);

    c = completer;

    if (!c)
        return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(c, QOverload<const QString &>::of(&QCompleter::activated),
                     this, &CodeEditor::insertCompletion);
}
