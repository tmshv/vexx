#include "XAbstractCanvas.h"
#include "XAbstractRenderModel.h"

XAbstractCanvas::XAbstractCanvas(XAbstractRenderModel *m) : _model(0), _iterator(0)
  {
  setModel(m);
  }

XAbstractCanvas::~XAbstractCanvas()
  {
  setModel(0);
  }

void XAbstractCanvas::update(XAbstractRenderModel::UpdateMode) const
  {
  }

void XAbstractCanvas::paint()
  {
  xAssert(_model && _iterator);
  if(_model && _iterator)
    {
    _model->resetIterator(_iterator);
    while(_iterator->next())
      {
      XAbstractDelegate *delegate = _model->delegateFor(_iterator);
      _model->paint(this, _iterator, delegate);
      }
    }
  }

void XAbstractCanvas::setModel(XAbstractRenderModel *m)
  {
  if(_iterator)
    {
    xAssert(_model);
    if(_model)
      {
      _model->destroyIterator(_iterator);
      }
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
