#ifndef INEFIXTURE_H
#define INEFIXTURE_H

#include "sentity.h"
#include "sproperty.h"

class IneFixtureInstance;

class IneFixture : public SEntity
  {
  S_COMPLEX_ENTITY(IneFixture, SEntity);

public:
  IneFixture();

  enum
    {
    Manufacturer = SEntity::LastProperty,
    LastProperty
    };

  enum FormatType
    {
    Byte,
    Shoft,
    Word
    };

  enum Attributes
    {
    Format = SProperty::LastAttribute,
    Output,
    Offset,
    LastAttribute
    };


  SPropertyID addParameter(QString);
  void removeParameter(QString);

  virtual xuint32 numberOfProperties() const;
  virtual xuint32 numberOfAttributes(SPropertyID) const;

  QString manufacturer() const;

  void serialise(IneFixtureInstance *instance, QByteArray &data);

private:
  virtual XVariant getData( SPropertyID id, SAttributeID attribute ) const;
  virtual bool setData( SPropertyID id, SAttributeID attribute, const XVariant &value );

  struct Parameter
    {
    QString name;
    XVariant value;
    xuint32 format;

    // starting offset from fixture DMX address
    xuint32 offset;
    };

  QList<Parameter> _params;
  QString _manufacturer;
  };

#endif // INEFIXTURE_H
