#include "videoNode.h"
#include "imageNode.h"
#include "QBitArray"
#include "QRectF"
#include "ASettings"
#include "XFileSequence"
#include "AFileDialog"

videoNode::videoNode() : _image( 0 ), _sampler( "null" )
    {
    INIT_NODE

    addProperty( "fileSequence", "", propertyInfo::FileSequence, "File Sequence" );

    QObject::connect( this, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SLOT(setupFile(APropertyDevice*,AProperty*)) );

    // standard image attriutes
    addAttribute( "aspect", 1.0, propertyInfo::Double, "Aspect Ratio" );
    addAttribute( "width", 1.0, propertyInfo::Double, "Width" );

    addProperty( "image/isValid", FALSE, propertyInfo::Bool );
    property( "image/isValid" ).hide();

    // setup source
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

    // grab a texture unit
    _unit = getTextureUnit();
    if( _unit == -1 )
        {
        AWarning << "There are no free image units available." << endl;
        }

    // setup help
    property( "longHelp" ) = "A Video node allows you to load in a sequence of images and treat them as a video sequence.";
    property( "shortHelp" ) = "A Video node allows you to load in a sequence of images and treat them as a video sequence.";
    }

videoNode::~videoNode()
    {
    // delete image and return texture unit
    delete _image;
    returnTextureUnit( _unit );
    }

void videoNode::relocated()
    {
    // on relocated we reload the node and setup for rendering
    nodeEntity::relocated();
    setupFile( 0, &property("fileSequence") );
    _sampler.setName( tidyName().toStdString() );
    }

void videoNode::setupFile( APropertyDevice*, AProperty *prop )
    {
    // rebuild on name changes to keep source up to date with entity
    if( prop->name() == "entity/name" )
        {
        _sampler.setName( tidyName().toStdString() );
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
    else if( prop->name() == "fileSequence" )
        {
        // refresh the time of this entity to load the sequence
        ASettings settings( array(), "timeline" );
        setTime( settings.value( "frame", 0 ).toDouble() );


        // set up aspect and width
        jLib::file::ioFile file( projectDirectory().toStdString() + "/" + property( "fileSequence" ).attribute("chosenFile")->toString().toStdString() );

        if( !file.exists() )
            {
            file.setFilename( property( "fileSequence" ).attribute("chosenFile")->toString().toStdString() );
            }

        jLib::graphics::pixel::image im( file );
        // load default valies
        property( "aspect" ) = (float)im.width() / (float)im.height();
        property( "width" ) = (float)im.width();
        }
    }

void videoNode::addVariables( jLib::graphics::shader::variableSet &set )
    {
    set.add( _sampler );
    nodeEntity::addVariables( set );
    }

QRectF videoNode::rect()
    {
    // calculate the rect from width and aspect
    double width( property( "width" )->toDouble() );
    QSize size( width, width / property( "aspect")->toDouble() );
    QSize minusHalf( size * -0.5 );
    return QRectF( QPointF( minusHalf.width(), minusHalf.height() ), size );
    }

void videoNode::rebuild()
    {
    // rebuld the source from properties
    if( _image && _image->isValid() )
        {
        // setup sampler to render
        _sampler.set( _image );
        property( "image/isValid" ) = TRUE;
        }
    else
        {
        // set video to return null
        property( "nodeEntity/source") = "return vec4( 0.0 );";
        property( "image/isValid" ) = FALSE;
        }
    }

void videoNode::initiate()
    {
    // on initiation we pop up a file dialog to choose files
    QString fN( AFileDialog::getOpenFileName( "Choose a file from the sequence" ) );
    XFileSequence seq( fN, XFileSequence::NumericExtract );
    property("fileSequence").attribute("chosenFile") = fN;
    property("fileSequence") = seq;
    XString name( QFileInfo( seq.niceFilename() ).baseName() );
    if( name.length() > 5 )
        {
        // set up the node name if the file name is sensible
        property("entity/name") = array()->getUniquePath( AEntityLocation("/"), name ).name();
        }
    }

void videoNode::setTime( int in )
    {
    // on set time we load a new file
    XFileSequence sq( property( "fileSequence" ) );

    // rebuild image if the sequence specified has a frame number, or no image is loaded
    if( !_image || sq.hasFrameNumber() )
        {
        // grab the filename
        QString filename( sq.getFilename( in ) );

        // clear the current image
        if( _image )
            {
            delete _image;
            _sampler.set( 0 );
            _image = 0;
            }

        jLib::file::ioFile file( projectDirectory().toStdString() + "/" + filename.toStdString() );

        if( !file.exists() )
            {
            file.setFilename( filename.toStdString() );
            }

        if( file.exists() )
            {
            // make image from file.
            jLib::graphics::pixel::image im( file );
            setThumbnail( im.toQImage() );

            if( im.isValid() )
                {
                _image = new jLib::graphics::pixel::texture( im, jLib::graphics::pixel::texture::RGBA16F );
                _image->setTextureUnit( _unit );
                rebuild();
                }
            }
        else
            {
            // cant find file, return black
            property( "image/isValid" ) = FALSE;
            setThumbnail( QImage() );
            }
        }
    }

void videoNode::prePropertyChanged( AProperty *prop )
    {
    // set up the project directory if the file sequence has changed
    QString projectsFolder( projectDirectory() );
    if( prop->name() == "fileSequence" )
        {
        XFileSequence sq( *prop );
        if( sq.parsedFilename().startsWith( projectsFolder ) )
            {
            property("fileSequence") = XFileSequence( sq.parsedFilename().mid( projectsFolder.length() ), XFileSequence::Auto );
            }
        }

    nodeEntity::prePropertyChanged( prop );
    }
