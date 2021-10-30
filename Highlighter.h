#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include "CodeEditor.h"

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit Highlighter (QTextDocument *parent=nullptr);
    Highlighter (const Highlighter&) = delete;
    Highlighter& operator= (const Highlighter&) = delete;

    ~Highlighter() = default;

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat preprocessorFormat;
    QTextCharFormat branching;

    QTextCharFormat branching_CONTINUE;

    QTextCharFormat numbers;

    QTextCharFormat defaultTypes;

    QTextCharFormat typeQualifiers;

    QTextCharFormat customTypes;

    QTextCharFormat accessSpecifiers;

    QTextCharFormat x86AsmCommands;
    QTextCharFormat x86AsmRegisters;

    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat include;
    QTextCharFormat functionFormat;
};

#endif // HIGHLIGHTER_H
