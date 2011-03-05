#include "renderDebugger.h"
#include "QPushButton"
#include "QComboBox"
#include "QHBoxLayout"
#include "nodeRenderer.h"
#include "scriptEdit.h"

#ifdef SYNAPSE_RENDER_DEBUG

renderDebugger::renderDebugger()
    {
    // layout for UI
    QVBoxLayout *layout( new QVBoxLayout( base() ) );
    QHBoxLayout *topLayout( new QHBoxLayout( ) );
    layout->addLayout( topLayout );

    // combo for renderer selection
    _rendererSelector = new QComboBox( base() );
    topLayout->addWidget( _rendererSelector );
    connect( _rendererSelector, SIGNAL(activated(int)), this, SLOT(setRenderer(int)) );

    // button to refresh
    QPushButton *refreshRenderer( new QPushButton( "Refresh Renderer List", base() ) );
    topLayout->addWidget( refreshRenderer );
    connect( refreshRenderer, SIGNAL(clicked()), this, SLOT(refreshRenderers()) );

    // script edit to display source
    _editor = new scriptEdit( base() );
    _editor->setReadOnly( true );
    layout->addWidget( _editor );

    refreshRenderers();
    }

void renderDebugger::setRenderer( int i )
    {
    // default
    _editor->setText( "No Source Available" );

    // display correct source
    if( i >= 0 )
        {
        const nodeRenderer *renderer( reinterpret_cast <const nodeRenderer *> (_rendererSelector->itemData( i ).toULongLong() ) );

        if( renderer )
            {
            _editor->setText( renderer->currentSource() );
            }
        }
    }

void renderDebugger::refreshRenderers()
    {
    // search for renderers, and put them in the combo
    _rendererSelector->clear();
    foreach( const nodeRenderer *renderer, renderDebug::renderers() )
        {
        _rendererSelector->addItem( "0x" + QString::number( (qulonglong)renderer, 16 ), reinterpret_cast <qulonglong> (renderer) );
        }
    setRenderer( _rendererSelector->currentIndex() );
    }

#endif
