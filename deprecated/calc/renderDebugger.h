#ifndef RENDERDEBUGGER_H
#define RENDERDEBUGGER_H

#include "AAbstractDock"

class QComboBox;
class scriptEdit;

#ifdef SYNAPSE_RENDER_DEBUG

class renderDebugger : public AAbstractDock
    {
    Q_OBJECT
public:
    renderDebugger();

private slots:
    void refreshRenderers();
    void setRenderer(int);

private:
    QComboBox *_rendererSelector;
    scriptEdit *_editor;
    };

#endif
#endif // RENDERDEBUGGER_H
