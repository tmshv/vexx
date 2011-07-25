#include "appScript.h"
#include "QScriptEngine"

inline QVariant toVariant( const propertyVariant &v )
    {
    if( v.type() == propertyVariant::Int )
        {
        return QVariant( v.toInt() );
        }
    else if( v.type() == propertyVariant::Double )
        {
        return QVariant( v.toDouble() );
        }
    else if( v.type() == propertyVariant::UnsignedInt )
        {
        return QVariant( v.toUnsignedInt() );
        }
    else
        {
        return QVariant( v.toQtString() );
        }
    }

QScriptValue customPrint( QScriptContext *context, QScriptEngine *engine )
    {
    QString result;
    for (int i = 0; i < context->argumentCount(); ++i) {
        if (i > 0)
            result.append(" ");
        result.append(context->argument(i).toString());
    }

    appLog<<result<<endl;

    return engine->undefinedValue();
    }

appScript::appScript( QObject *parent ) : QObject( parent )
    {
    }

void appScript::added()
    {
    _engine = new QScriptEngine( this );
    QScriptValue objectValue = _engine->newQObject( this );
    _engine->globalObject().setProperty( "alter", objectValue );

    QScriptValue logPrint = _engine->newFunction(customPrint);
    _engine->globalObject().setProperty( "print", logPrint );
    }

void appScript::execute( QString code )
    {
    //appLog<<"Run {"<<endl<<code<<endl<<"}"<<endl;
    QScriptValue ret = _engine->evaluate( code );
    if( _engine->hasUncaughtException() )
        {
        appLog<<"Error in script at line "<<_engine->uncaughtExceptionLineNumber()<<": "<<endl<<ret.toString()<<endl;
        }
    else if( !ret.isUndefined() )
        {
        appLog<<"Script Returned: "<<ret.toString()<<endl;
        }
    else
        {
        //appLog<<"Script Returned"<<endl;
        }
    }

//--------------------------------------------------- COMMANDS ---------------------------------------------------------

void appScript::log( QString text )
    {
    appLog<<text<<endl;
    }

QString appScript::addEntity( QString in )
    {
    return app().addEntity( in ).path();
    }

void appScript::removeEntity( QString in )
    {
    app().removeEntity( entityLocation( in ) );
    }

QStringList appScript::entities( )
    {
    QStringList ret;
    propertyEntitySearch s = app().search();
    FOREACH( s, ent )
        {
        ret.append( (*ent)->location().path() );
        }
    return ret;
    }

void appScript::addProperty( QString ent, QString pr, QString val )
    {
    propertyEntitySearch s = app().search().findEntity( entityLocation( ent ) );
    if( s.size() && !(s[0].propertyExists( pr ) ) )
        {
        s[0].addProperty( pr, val );
        }
    }

void appScript::removeProperty( QString ent, QString pr )
    {
    propertyEntitySearch s = app().search().findEntity( entityLocation( ent ) );
    if( s.size() && s[0].propertyExists( pr ) )
        {
        s[0].removeProperty( pr );
        }
    }

QStringList appScript::properties( QString in )
    {
    QStringList ret;
    propertyEntitySearch s = app().search().findEntity( entityLocation( in ) );
    if( s.size() )
        {
        FOREACH( s[0], prop )
            {
            ret.append( prop.key() );
            }
        }
    return ret;
    }

QVariant appScript::getProperty( QString ent, QString pr )
    {
    propertyEntitySearch s = app().search().findEntity( entityLocation( ent ) );
    if( s.size() && s[0].propertyExists(pr) )
        {
        return toVariant( s[0].property( pr ) );
        }
    return QVariant();
    }

void appScript::setProperty( QString ent, QString pr, QString val )
    {
    propertyEntitySearch s = app().search().findEntity( entityLocation( ent ) );
    if( s.size() && s[0].propertyExists(pr) )
        {
        s[0].property( pr ) = val;
        }
    }

QVariant appScript::attribute( QString ent, QString pr, QString att )
    {
    propertyEntitySearch s = app().search().findEntity( entityLocation( ent ) );
    if( s.size() && s[0].propertyExists(pr) )
        {
        return toVariant( s[0].property( pr ).attribute( att ) );
        }
    return QVariant();
    }

void appScript::setAttribute( QString ent, QString pr, QString att, QString val )
    {
    propertyEntitySearch s = app().search().findEntity( entityLocation( ent ) );
    if( s.size() && s[0].propertyExists(pr) )
        {
        s[0].property( pr ).attribute( att ) = val;
        }
    }

QStringList appScript::attributes( QString ent, QString pr )
    {
    QStringList ret;
    propertyEntitySearch s = app().search().findEntity( entityLocation( ent ) );
    if( s.size() && s[0].propertyExists(pr) )
        {
        const propertyData::attributeMap &attrs( s[0].property( pr ).attributes( ) );
        FOREACHCONST( attrs, at )
            {
            ret.append( at.key() );
            }
        }
    return ret;
    }
