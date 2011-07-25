#include "scriptEdit.h"
#include "ALog"

scriptHighlighter::scriptHighlighter( QStringList keywordPatterns, QStringList typePatterns, QTextEdit *parent ) : QSyntaxHighlighter( parent )
    {
    HighlightingRule rule;

    // match floating point numbers before others, so they override it
    numberFormat.setForeground( QColor( 255, 13, 255 ) );
    rule.format = numberFormat;
    rule.pattern = QRegExp("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?");
    highlightingRules.append(rule);

    // search for giuven key words
    keywordFormat.setForeground(QColor( 18, 129, 31 ));
    keywordFormat.setFontWeight(QFont::Bold);
    foreach( const QString &pattern, keywordPatterns )
        {
        rule.pattern = QRegExp( "\\b" + pattern + "\\b" );
        rule.format = keywordFormat;
        highlightingRules.append( rule );
        }

    // search for given types
    typeFormat.setForeground(QColor( 225, 89, 26 ));
    foreach( const QString &pattern, typePatterns )
        {
        rule.pattern = QRegExp( "\\b" + pattern + "\\b" );
        rule.format = typeFormat;
        highlightingRules.append( rule );
        }

    // search for quotes ( not the minimal search
    quotationFormat.setForeground( QColor( 255, 198, 0 ) );
    rule.format = quotationFormat;
    rule.pattern = QRegExp("\".*\"");
    rule.pattern.setMinimal( TRUE );
    highlightingRules.append(rule);
    rule.pattern = QRegExp("'.*'");
    rule.pattern.setMinimal( TRUE );
    highlightingRules.append(rule);

    // search for single line comments
    singleLineCommentFormat.setForeground( QColor( 150, 150, 150 ) );
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    // search for long comments
    multiLineCommentFormat.setForeground( QColor( 150, 150, 150 ) );
    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");

    }

void scriptHighlighter::highlightBlock( const QString &text )
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

scriptEdit::scriptEdit( QWidget *parent ) : QTextEdit( parent )
    {
    QStringList keywords;
    keywords << "coords" << "getInput" << "getAttribute" << "attribute" << "const" << "uniform" << "varying"
             << "centroid" << "break" << "continue" << "do" << "for" << "while" << "if" << "else" << "in" << "out"
             << "inout" << "true" << "false" << "invariant" << "discard" << "return" << "struct";
    QStringList types;
    types << "void" << "bool" << "int" << "float" << "vec2" << "vec3" << "vec4" << "bvec2" << "bvec3" << "bvec4"
            << "ivec2" << "ivec3" << "ivec4" << "mat2" << "mat3" << "mat4" << "mat2x2" << "mat2x3" << "mat2x4"
            << "mat3x2" << "mat3x3" << "mat3x4" << "mat4x2" << "mat4x3" << "mat4x4" << "sampler1D" << "sampler2D"
            << "sampler3D" << "samplerCube" << "sampler1DShadow" << "sampler2DShadow";
    new scriptHighlighter( keywords, types, this );
    }
