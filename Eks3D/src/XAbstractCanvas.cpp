#include "XAbstractCanvas.h"
#include "XAbstractRenderModel.h"
#include "XAbstractDelegate.h"
#include "XAbstractCanvasController.h"

XAbstractCanvas::XAbstractCanvas(XAbstractRenderModel *m, XAbstractCanvasController *c) : _model(0), _controller(c), _iterator(0)
  {
  setModel(m);
  }

XAbstractCanvas::~XAbstractCanvas()
  {
  setModel(0);
  }

void XAbstractCanvas::update(XAbstractRenderModel::UpdateMode)
  {
  xAssert(_model && _iterator);
  if(_model && _iterator)
    {
    _model->resetIterator(_iterator);
    while(_iterator->next())
      {
      const XAbstractDelegate *delegate = _model->delegateFor(_iterator, this);
      if(delegate)
        {
        delegate->update(this, _iterator, _model);
        }
      }
    }
  }

void XAbstractCanvas::paint()
  {
  xAssert(_model && _iterator);
  if(_model && _iterator)
    {
    xuint32 numPasses = 1;

    _model->resetIterator(_iterator);
    while(_iterator->next())
      {
      const XAbstractDelegate *delegate = _model->delegateFor(_iterator, this);
      if(delegate)
        {
        numPasses = qMax(numPasses, delegate->maxNumberOfPasses(this, _iterator, _model));
        }
      }
    if(_controller)
      {
      numPasses = qMax(numPasses, _controller->maxNumberOfPasses(numPasses));
      }

    for(xuint32 passIndex=0; passIndex<numPasses; ++passIndex)
      {
      _model->resetIterator(_iterator);
      while(_iterator->next())
        {
        const XAbstractDelegate *delegate = _model->delegateFor(_iterator, this);
        if(delegate)
          {
          delegate->paint(passIndex, this, _iterator, _model);
          }
        }

      if(_controller)
        {
        _controller->paint(passIndex);
        }
      }
    }
  }

void XAbstractCanvas::setModel(XAbstractRenderModel *m)
  {
  if(_iterator)
    {
    xAssert(_model);
    delete _iterator;
    _iterator = 0;
    }

  if(_model)
    {
    _model->_canvases.removeAll(this);
    }

  _model = m;
  if(_model)
    {
    _iterator = _model->createIterator();
    _model->_canvases << this;
    }
  }
