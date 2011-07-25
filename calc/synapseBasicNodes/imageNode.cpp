#include "imageNode.h"
#include "AFileDialog"
#include "QRectF"

imageNode::imageNode() : _image( 0 ), _sampler( "null" )
    {
    INIT_NODE
    // add file property
    addProperty( "File", "", propertyInfo::File );
    QObject::connect( this, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SLOT(fileChanged(APropertyDevice*,AProperty*)) );

    // add attributes for user control
    addAttribute( "aspect", 1.0, propertyInfo::Double, "Aspect Ratio" );
    addAttribute( "width", 1.0, propertyInfo::Double, "Width" );

    addProperty( "image/isValid", FALSE, propertyInfo::Bool );
    property( "image/isValid" ).hide();

    // grab and check a texture unit
    _unit = getTextureUnit();
    if( _unit == -1 )
        {
        AWarning << "There are no free image units available." << endl;
        }

    property( "longHelp" ) = "An Image node allows input from a range of image types.\n\nSupported types include: JPG, EXR, PNG and 8-bit TIF";
    property( "shortHelp" ) = "An Image node allows input from a range of image types.\n\nSupported types include: JPG, EXR, PNG and 8-bit TIF";
    }

imageNode::~imageNode()
    {
    delete _image;
    // give texture unit back
    returnTextureUnit( _unit );
    }

void imageNode::relocated()
    {
    // trigger a reload of the image
    nodeEntity::relocated();
    fileChanged( 0, &property("File") );
    }

void imageNode::fileChanged( APropertyDevice*, AProperty *prop )
    {
    if( prop->name() == "entity/name" )
        {
        // rebuild so we get source code refreshed
        rebuild();
        }
    if( prop->name() == "File" )
        {
        // delete image if it exists
        if( _image )
            {
            setThumbnail( QImage() );
            delete _image;
            _sampler.set( 0 );
            _image = 0;
            }
        // create a file
        jLib::file::ioFile file( projectDirectory().toStdString() + "/" + prop->value().toString().toStdString() );

        if( !file.exists() )
            {
            file.setFilename( prop->value().toString().toStdString() );
            }

        if( file.exists() )
            {
            // load an image
            jLib::graphics::pixel::image im( file );

            // check we loaded ok
            if( im.isValid() )
                {
                setThumbnail( im.toQImage() );
                // create a texture from it
                _image = new jLib::graphics::pixel::texture( im, jLib::graphics::pixel::texture::RGBA16F );
                // set the texute unit
                _image->setTextureUnit( _unit );

                property( "aspect" ) = (float)_image->width() / (float)_image->height();
                property( "width" ) = (float)_image->width();

                rebuild();
                }
            }
        else
            {
            // image is invalid, reset everything to black
            property( "nodeEntity/source") = "return vec4( 0.0 );";
            property( "image/isValid" ) = FALSE;
            }
        }
    }

void imageNode::addVariables( jLib::graphics::shader::variableSet &set )
    {
    // add the sampler variable
    set.add( _sampler );
    // then normal variables
    nodeEntity::addVariables( set );
    }

void imageNode::initiate()
    {
    // get a filename
    property( "File" ) = AFileDialog::getOpenFileName( "Choose an image file for the new imageNode" );

    // if its valid, set up the filename
    if( property("File")->toString() != "" )
        {
        property("entity/name") = array()->getUniquePath( AEntityLocation("/"), QFileInfo(property("File")->toString()).baseName() ).name();
        }
    }

QRectF imageNode::rect()
    {
    // grab the width
    double width( property( "width" )->toDouble() );
    // height = width / aspect
    QSize size( width, width / property( "aspect")->toDouble() );
    // divide by two
    QSize minusHalf( size * -0.5 );
    // create a rect from the half values
    return QRectF( QPointF( minusHalf.width(), minusHalf.height() ), size );
    }

void imageNode::rebuild()
    {
    // set the name to the entity name
    _sampler.setName( tidyName().toStdString() );

    // check we have a valid image
    if( _image && _image->isValid() )
        {
        // set up the sampler
        _sampler.set( _image );
        // the image is valid now
        property( "image/isValid" ) = TRUE;

        // the source for actually sampling a texture
        property( "nodeEntity/source" ) = "vec2 pos = vec2( ( coords.s / getAttribute( \"width\" ) ) + 0.5, ( ( getAttribute( \"aspect\" ) / getAttribute( \"width\" ) ) * coords.t ) + 0.5 );"
                               "if( pos.s >= 0.0 && pos.s <= 1.0 && \n" \
                               "pos.t >= 0.0 && pos.t <= 1.0 )\n" \
                               "{\n" \
                               "return texture2D( " + tidyName() + ", pos );\n" \
                               "}\n" \
                               "else\n" \
                               "{\n" \
                               "return vec4( 0.0 );\n" \
                               "}\n";
        }
    else
        {
        // else null
        property( "nodeEntity/source") = "return vec4( 0.0 );";
        property( "image/isValid" ) = FALSE;
        }

    }

void imageNode::setTime( int )
    {
    // dont do anything as its just an image
    }

void imageNode::prePropertyChanged( AProperty *prop )
    {
    QString projectsFolder( projectDirectory() );
    if( prop->name() == "File" && prop->value().toString().startsWith( projectsFolder ) )
        {
        property("File") = prop->value().toString().mid( projectsFolder.length() );
        }

    nodeEntity::prePropertyChanged( prop );
    }
