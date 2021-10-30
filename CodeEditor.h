#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QWidget>
#include <QRect>
#include <QPainter>
#include <QTextBlock>
#include <QPaintEvent>
#include <QResizeEvent>
#include "completelistwidget.h"
#include "Highlighter.h"
#include <QPlainTextEdit>
#include <QScrollBar>
#include<QCompleter>
class CodeEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);
    CodeEditor(const CodeEditor&) = delete;
    CodeEditor& operator= (const CodeEditor&) = delete;

    ~CodeEditor() = default;

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void setCompleter(QCompleter *c);
    QCompleter *completer() const;
    QStringList typelist;
    QStringList wordlist;

protected:
    void resizeEvent(QResizeEvent *event) override;

    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;

private slots:
    void updateLineNumberAreaWidth();
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);

    void insertCompletion(const QString &completion);


private:
    QWidget *lineNumberArea;
    QString textUnderCursor() const;
    QCompleter *c = nullptr;


};


#endif // CODEEDITOR_H
