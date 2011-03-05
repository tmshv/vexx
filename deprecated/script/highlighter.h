#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>

class highlighter : public QSyntaxHighlighter
    {
     Q_OBJECT

 public:
     highlighter( QTextEdit *parent = 0 );

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

     QTextCharFormat keywordFormat;
     QTextCharFormat singleLineCommentFormat;
     QTextCharFormat multiLineCommentFormat;
     QTextCharFormat quotationFormat;
     QTextCharFormat functionFormat;
    };

#endif // HIGHLIGHTER_H
