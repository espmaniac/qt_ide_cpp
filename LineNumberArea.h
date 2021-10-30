#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H
#include <QWidget>
#include "CodeEditor.h"
class LineNumberArea : public QWidget {
public:
    explicit LineNumberArea (CodeEditor *editor);
    LineNumberArea (const LineNumberArea&) = delete;
    LineNumberArea& operator= (const LineNumberArea&) = delete;

    ~LineNumberArea() = default;

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
private:
    CodeEditor *codeEditor;
};
#endif // LINENUMBERAREA_H
