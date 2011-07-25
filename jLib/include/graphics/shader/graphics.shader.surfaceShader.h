#ifndef JLIB_GRAPHICS_SHADER_SURFACESHADER_H
#define JLIB_GRAPHICS_SHADER_SURFACESHADER_H

#include "graphics/shader/graphics.shader.types.h"

BEGIN_JLIB_GRAPHICS_SHADER_NAMESPACE

class JLIB_EXPORT surfaceShaderVariable
    {
public:
    virtual ~surfaceShaderVariable();
    void setName( string );
    string name( ) const;
    template <typename T>T *getAs() { jAssert( dynamic_cast <T *> (this) ); return static_cast <T *> (this); }
    template <typename T>const T *getAs() const { jAssert( dynamic_cast <const T *> (this) ); return static_cast <const T *> (this); }
    virtual void set( ) = 0;
    virtual void unset( ) { }
    virtual void refreshLocation( int program );
    virtual string glslType() const = 0;
    string glslUniformVariableDefinition( ) const { return "uniform " + glslType() + " " + name(); }
//protected:
    surfaceShaderVariable( string );
    string _name;
    int _varNum;
    };

class JLIB_EXPORT surfaceShader
    {
public:
    typedef surfaceShaderVariable variable;
    surfaceShader( string fragmentShader="", string vertexShader="" );
    void setFragmentSource( string );
    void setVertexSource( string );
    void setSource( string, string );
    void rebuild();
    string getFragmentSource( );
    string getVertexSource( );
    string getError( unsigned int );
    void clearErrors();
    void clearError( unsigned int );
    unsigned int sizeErrors();
    bool isValid();
    void use();
    void clear();
    /*variable *getVariable( string );
    variable *getVariable( unsigned int );
    template <typename T> T *getVariableAs( string name )
        {
        return dynamic_cast <T *> ( getVariable( name ) );
        }
    void setVariable( string, variable * );
    void setVariable( string, variable & );
    unsigned int sizeVariables();*/
protected:
    bool compileFragment();
    bool compileVertex();
    bool linkProgram();
    bool isValidForFragmentCompile();
    bool isValidForVertexCompile();
    void clearShader();
private:
    //void addVariable( string, variable * );
    //void addVariable( string, variable & );
    void clearFragment();
    void clearVertex();
    void clearProgram();
    string _fragment;
    string _vertex;
    int _fragNum;
    int _vertNum;
    int _programNum;
    vector <string> _errors;
    //unsigned int _shaderNumber;
    //vector <string> _variableNames;
    //vector <variable *> _variables;
    friend class variableSet;
    };

END_JLIB_GRAPHICS_SHADER_NAMESPACE

#endif
