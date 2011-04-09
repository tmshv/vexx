#include "application.h"

Application::Application() : _environment(&_dataInterface, this)
  {
  }

void Application::onRequestComplete(const XEnvironment::Request &request)
  {
  emit requestComplete(request);
  }

void Application::createContainer(XEnvironment::ItemID parentID)
  {
  XEnvironment::ItemID id = _environment.createItem(XEnvironment::ContainerType);

  XEnvironment::Container *parent = 0;
  // find the parent first
  if(id != 0)
    {
    parent = _environment.container(parentID);
    if(!parent)
      {
      XEnvironment::Request req(XEnvironment::ContainerType, parentID);
      _environment.requestItem(req, true);
      parent = _environment.container(parentID);
      }
    }

  XEnvironment::Container *cont = 0;
  xAssert(id == 0 || parent);
  if(parent || id == 0)
    {
    cont = _environment.container(id);
    if(!cont)
      {
      XEnvironment::Request req(XEnvironment::ContainerType, id);
      _environment.requestItem(req, true);
      cont = _environment.container(id);
      }
    xAssert(cont);
    }

  XEnvironment::Container *oldParent = 0;
  // find the old parent
  if(cont && id != 0)
    {
    oldParent = _environment.container(cont->parentID());
    if(!oldParent)
      {
      XEnvironment::Request req(XEnvironment::ContainerType, cont->parentID());
      _environment.requestItem(req, true);
      oldParent = _environment.container(cont->parentID());
      }
    }

  xAssert(id == 0 || (parent != 0 && oldParent != 0));
  xAssert(cont);
  if(id == 0 || (parent != 0 && oldParent != 0))
    {
    cont->setName("New Container");
    if(id != 0)
      {
      cont->reparent(oldParent, parent);
      }
    }

  if(cont)
    {
    if(id != 0)
      {
      emit dataChanged(XEnvironment::Request(XEnvironment::ContainerType, oldParent->ID()));
      emit dataChanged(XEnvironment::Request(XEnvironment::ContainerType, parent->ID()));

      _environment.syncData(XEnvironment::Request(XEnvironment::ContainerType, oldParent->ID()));
      _environment.syncData(XEnvironment::Request(XEnvironment::ContainerType, parent->ID()));
      }
    emit dataChanged(XEnvironment::Request(XEnvironment::ContainerType, id));
    _environment.syncData(XEnvironment::Request(XEnvironment::ContainerType, id));
    }

  }
