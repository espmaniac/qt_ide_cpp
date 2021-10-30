#include "Highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent),
      commentStartExpression(QRegularExpression(QStringLiteral("/\\*"))),
      commentEndExpression(QRegularExpression(QStringLiteral("\\*/")))
{
    HighlightingRule rule;

    keywordFormat.setForeground(QColor("#569CD6"));
    keywordFormat.setFontWeight(QFont::Light);

    // подсветить true false

    const QString keywordPatterns[] = {
        QStringLiteral("\\benum\\b"), QStringLiteral("\\bnamespace\\b"), QStringLiteral("\\boperator\\b"),
        QStringLiteral("\\bprotected\\b"),
        QStringLiteral("\\btemplate\\b"),
        QStringLiteral("\\btypedef\\b"), QStringLiteral("\\btypename\\b"), QStringLiteral("\\bthis\\b"),
        QStringLiteral("\\bconst_cast\\b"), QStringLiteral("\\bstatic_cast\\b"), QStringLiteral("\\bdynamic_cast\\b"),
        QStringLiteral("\\breinterpret_cast\\b"), QStringLiteral("\\bnullptr\\b"), QStringLiteral("\\busing\\b"),
        QStringLiteral("\\basm\\b"), QStringLiteral("\\bfor\\b"), QStringLiteral("\\bwhile\\b"), QStringLiteral("\\bdo\\b"),
        QStringLiteral("\\bsizeof\\b")
    };


    for (const QString &pattern : keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    customTypes.setForeground(QColor("#4EC9B0"));
    const QString customTypesList[] {
      QStringLiteral("\\bint8_t\\b"), QStringLiteral("\\buint8_t\\b"),
      QStringLiteral("\\bint16_t\\b"), QStringLiteral("\\buint16_t\\b"),
      QStringLiteral("\\bint32_t\\b"), QStringLiteral("\\buint32_t\\b"),
      QStringLiteral("\\bint64_t\\b"), QStringLiteral("\\buint64_t\\b"),
      QStringLiteral("\\bsize_t\\b"), QStringLiteral("\\bwchar_t\\b")
    };

    for (const QString &pattern : customTypesList) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = customTypes;
        highlightingRules.append(rule);
    }

    x86AsmCommands.setForeground(QColor("#9900ff"));

    const QString asmCommands[] = {
      QStringLiteral("\\bmov\\b"), QStringLiteral("\\bpush\\b"), QStringLiteral("\\bpop\\b"),
      QStringLiteral("\\blea\\b"), QStringLiteral("\\badd\\b"), QStringLiteral("\\bsub\\b"),
      QStringLiteral("\\binc\\b"), QStringLiteral("\\bdec\\b"), QStringLiteral("\\bimul\\b"),
      QStringLiteral("\\bidiv\\b"), QStringLiteral("\\band\\b"), QStringLiteral("\\bor\\b"),
      QStringLiteral("\\bxor\\b"), QStringLiteral("\\bnot\\b"), QStringLiteral("\\bjmp\\b")
    };

    for (const QString &pattern : asmCommands) {
      rule.pattern = QRegularExpression(pattern);
      rule.format = x86AsmCommands;
      highlightingRules.append(rule);
    }

    x86AsmRegisters.setForeground(QColor("#4cc1a9"));

    const QString asmRegisters[] = {
      QStringLiteral("\\beax\\b"), QStringLiteral("\\bah\\b"), QStringLiteral("\\bal\\b"),
      QStringLiteral("\\bebx\\b"), QStringLiteral("\\bbh\\b"), QStringLiteral("\\bbl\\b"),
      QStringLiteral("\\becx\\b"), QStringLiteral("\\bch\\b"), QStringLiteral("\\bcl\\b"),
      QStringLiteral("\\bedx\\b"),QStringLiteral("\\bdh\\b"), QStringLiteral("\\bdl\\b"),

      QStringLiteral("\\besp\\b"), QStringLiteral("\\bsp\\b"), QStringLiteral("\\bebp\\b"),
      QStringLiteral("\\bbp\\b"), QStringLiteral("\\besi\\b"), QStringLiteral("\\bsi\\b"),
      QStringLiteral("\\bedi\\b"), QStringLiteral("\\bdi\\b"),

    };

    for (const QString &pattern : asmRegisters) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = x86AsmRegisters;
        highlightingRules.append(rule);
      }

    rule.pattern = QRegularExpression("\\w*::");
    rule.format.setFontWeight(QFont::Light);
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression("\\*");
    rule.format.setFontWeight(QFont::Bold);
    rule.format.setForeground(QColor("#b973ff"));
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression("&");
    rule.format.setFontWeight(QFont::Bold);
    rule.format.setForeground(QColor("#b973ff"));
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression("[< > = | % ^ \\- + \\/]");
    rule.format.setFontWeight(QFont::Bold);
    rule.format.setForeground(QColor("#ffa200"));
    highlightingRules.append(rule);

    accessSpecifiers.setForeground(QColor("#4aff89"));
    accessSpecifiers.setFontWeight(QFont::Bold);

    rule.pattern = QRegularExpression(QStringLiteral("\\bpublic\\b"));
    rule.format = accessSpecifiers;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("\\bprivate\\b"));
    rule.format = accessSpecifiers;
    rule.format.setForeground(QColor("#eb005a"));
    highlightingRules.append(rule);



    rule.pattern = QRegularExpression(QStringLiteral("\\bprotected\\b"));
    rule.format = accessSpecifiers;
    rule.format.setForeground(QColor("#e60000"));
    highlightingRules.append(rule);





    typeQualifiers.setForeground(QColor("#56B6C2"));
    typeQualifiers.setFontWeight(QFont::Bold);

    defaultTypes.setForeground(QColor("#ff55ff"));

    rule.pattern = QRegularExpression(QStringLiteral("\\bclass\\b"));
    rule.format = defaultTypes;
    rule.format.setFontWeight(QFont::Bold);
    rule.format.setFontItalic(true);
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("\\bunion\\b"));
    rule.format = defaultTypes;
    rule.format.setFontWeight(QFont::Bold);
    rule.format.setFontItalic(true);
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("\\bstruct\\b"));
    rule.format = defaultTypes;
    rule.format.setFontWeight(QFont::Bold);
    rule.format.setFontItalic(true);
    highlightingRules.append(rule);

    const QString types[] = {
      QStringLiteral("\\bvoid\\b"), QStringLiteral("\\bsigned\\b"), QStringLiteral("\\bunsigned\\b"),
      QStringLiteral("\\bbool\\b"), QStringLiteral("\\bregister\\b"),
      QStringLiteral("\\bchar\\b"), QStringLiteral("\\bshort\\b"), QStringLiteral("\\blong\\b"),
      QStringLiteral("\\bint\\b"), QStringLiteral("\\bfloat\\b"), QStringLiteral("\\bdouble\\b"),
      QStringLiteral("\\bconst\\b"), QStringLiteral("\\bconstexpr\\b"),
      QStringLiteral("\\balignas\\b"), QStringLiteral("\\bauto\\b"), QStringLiteral("\\bconsteval\\b"),
      QStringLiteral("\\bvolatile\\b"), QStringLiteral("\\brestrict\\b"), QStringLiteral("\\bmutable\\b"),
      QStringLiteral("\\bdecltype\\b"), QStringLiteral("\\binline\\b"), QStringLiteral("\\bexplicit\\b"),
      QStringLiteral("\\bfriend\\b"), QStringLiteral("\\bvirtual\\b"), QStringLiteral("\\bstatic\\b"),
      QStringLiteral("\\bextern\\b"), QStringLiteral("\\bnoexcept\\b")

    };

    for (const QString &pattern : types) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = defaultTypes;
        highlightingRules.append(rule);
    }

    numbers.setForeground(QColor("#6e6e6e"));
    rule.pattern = QRegularExpression(QStringLiteral("\\d+$"));
    rule.format = numbers;
    highlightingRules.append(rule);

    preprocessorFormat.setForeground(QColor("#4a4adb"));

    rule.pattern = QRegularExpression(QStringLiteral("#.\\w+"));
    rule.format = preprocessorFormat;
    highlightingRules.append(rule);


    branching.setForeground(QColor("#fd1a1a"));
    rule.pattern = QRegularExpression(QStringLiteral("\\bif\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    branching.setForeground(QColor("#fd1a1a"));
    rule.pattern = QRegularExpression(QStringLiteral("\\btry\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    branching.setForeground(QColor("#fd1a1a"));
    rule.pattern = QRegularExpression(QStringLiteral("\\bcatch\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    branching.setForeground(QColor("#fd1a1a"));
    rule.pattern = QRegularExpression(QStringLiteral("\\bcmp\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    branching.setForeground(QColor("#fd1a1a"));
    rule.pattern = QRegularExpression(QStringLiteral("\\bje\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    branching.setForeground(QColor("#fd1a1a"));
    rule.pattern = QRegularExpression(QStringLiteral("\\bjne\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    branching.setForeground(QColor("#fd1a1a"));
    rule.pattern = QRegularExpression(QStringLiteral("\\bjz\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    branching.setForeground(QColor("#fd1a1a"));
    rule.pattern = QRegularExpression(QStringLiteral("\\bjg\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    branching.setForeground(QColor("#fd1a1a"));
    rule.pattern = QRegularExpression(QStringLiteral("\\bjge\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    branching.setForeground(QColor("#fd1a1a"));
    rule.pattern = QRegularExpression(QStringLiteral("\\bjl\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    branching.setForeground(QColor("#fd1a1a"));
    rule.pattern = QRegularExpression(QStringLiteral("\\bjle\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("\\belse\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("\\bswitch\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("\\bcase\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("\\bdefault\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("\\breturn\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("\\bret\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("\\bcall\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("\\bbreak\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression(QStringLiteral("\\bdelete\\b"));
    rule.format = branching;
    highlightingRules.append(rule);

    branching_CONTINUE.setForeground(QColor("#00ff66"));
    rule.pattern = QRegularExpression(QStringLiteral("\\bcontinue\\b"));
    rule.format = branching_CONTINUE;
    highlightingRules.append(rule);

    branching_CONTINUE.setForeground(QColor("#00ff66"));
    rule.pattern = QRegularExpression(QStringLiteral("\\bnew\\b"));
    rule.format = branching_CONTINUE;
    highlightingRules.append(rule);

    include.setForeground(QColor("#56B6C2"));
    rule.pattern = QRegularExpression("(?<=#include\\s)(<.*>)|(?<=#include)(<.*>)|(?<=#include\\s)(\".*\")|(?<=#include)(\".*\")|\".*\"");
    rule.format = include;
    highlightingRules.append(rule);

    quotationFormat.setForeground(QColor("#D69D85"));
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::lightGray);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);


    singleLineCommentFormat.setForeground(QColor("#56A349"));
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(QColor("#56A349"));

}


void Highlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
      startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
