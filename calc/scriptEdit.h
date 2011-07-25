#ifndef SCRIPTEDIT_H
#define SCRIPTEDIT_H

#include "QTextEdit"
#include "QSyntaxHighlighter"

class scriptEdit : public QTextEdit
    {
public:
    scriptEdit( QWidget *parent=0 );
    };

class scriptHighlighter : public QSyntaxHighlighter
    {
     Q_OBJECT

 public:
     scriptHighlighter( QStringList, QStringList, QTextEdit *parent = 0 );

 protected:
     void highlightBlock(const QString &text);

 private:
    struct HighlightingRule
        {
        QRegExp pattern;
        QTextCharFormat format;
        };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat numberFormat;
    QTextCharFormat keywordFormat;
    QTextCharFormat typeFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    };

#endif // SCRIPTEDIT_H
