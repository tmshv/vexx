#include "highlighter.h"

highlighter::highlighter( QTextEdit *parent ) : QSyntaxHighlighter( parent )
    {
     HighlightingRule rule;

     keywordFormat.setForeground(QColor( 102, 237, 43 ));
     keywordFormat.setFontWeight(QFont::Bold);
     QStringList keywordPatterns;
     keywordPatterns << "\\bbreak\\b" << "\\bfor\\b" << "\\bthrow\\b"
                    << "\\bcase\\b" << "\\bfunction\\b" << "\\btry\\b"
                    << "\\bcatch\\b" << "\\bif\\b" << "\\btypeof\\b"
                    << "\\bcontinue\\b" << "\\bin\\b" << "\\bvar\\b"
                    << "\\bdefault\\b" << "\\binstanceof\\b" << "\\bvoid\\b"
                    << "\\bdelete\\b" << "\\bnew\\b" << "\\bundefined\\b"
                    << "\\bdo\\b" << "\\breturn\\b" << "\\bwhile\\b"
                    << "\\belse\\b" << "\\bswitch\\b" << "\\btypename\\b"
                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bwith\\b"
                    << "\\bfinally\\b" << "\\bthis\\b";
     foreach( const QString &pattern, keywordPatterns )
        {
        rule.pattern = QRegExp( pattern );
        rule.format = keywordFormat;
        highlightingRules.append( rule );
        }

    quotationFormat.setForeground( QColor( 255, 198, 0 ) );
    rule.format = quotationFormat;
    rule.pattern = QRegExp("\".*\"");
    highlightingRules.append(rule);
    rule.pattern = QRegExp("'.*'");
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground( QColor( 50, 50, 50 ) );
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground( QColor( 50, 50, 50 ) );
    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");

    }

void highlighter::highlightBlock(const QString &text)
    {
    foreach( const HighlightingRule &rule, highlightingRules )
        {
        QRegExp expression( rule.pattern );
        int index = expression.indexIn( text );
        while( index >= 0 )
            {
            int length = expression.matchedLength( );
            setFormat( index, length, rule.format );
            index = expression.indexIn( text, index + length );
            }
        }
    setCurrentBlockState(0);

    int startIndex = 0;
    if( previousBlockState() != 1 )
        {
        startIndex = commentStartExpression.indexIn(text);
        }

    while (startIndex >= 0)
        {
        int endIndex = commentEndExpression.indexIn( text, startIndex );
        int commentLength;
        if (endIndex == -1)
            {
            setCurrentBlockState( 1 );
            commentLength = text.length() - startIndex;
            }
        else
            {
            commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
            }
        setFormat( startIndex, commentLength, multiLineCommentFormat );
        startIndex = commentStartExpression.indexIn( text, startIndex + commentLength );
        }

    }
