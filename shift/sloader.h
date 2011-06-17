#ifndef SLOADER_H
#define SLOADER_H

#include "sglobal.h"
#include "QHash"
#include "QTextStream"
#include "QDataStream"

class QString;
class SProperty;
class SPropertyContainer;
class SPropertyInformation;

class SLoader
  {
public:
  enum Mode
    {
    Text,
    Binary
    };

XProperties:
  XROProperty(Mode, streamMode);

public:
  void setStreamDevice(Mode m, QIODevice *d) { _streamMode = m; _ts.setDevice(d); _ds.setDevice(d); }

  virtual const SPropertyInformation *type() const = 0;

  virtual xsize childCount() const = 0;
  virtual void beginNextChild() = 0;
  virtual void endNextChild() = 0;

  virtual void read(SPropertyContainer *parent) = 0;

  virtual void beginAttribute(const char *) = 0;
  virtual void endAttribute(const char *) = 0;

  virtual void resolveInputAfterLoad(SProperty *, const QString &) = 0;

  QTextStream &textStream() { return _ts; }
  QDataStream &binaryStream() { return _ds; }

private:
  QTextStream _ts;
  QDataStream _ds;
  };

class SSaver
  {
public:
  enum Mode
    {
    Text,
    Binary
    };

XProperties:
  XROProperty(Mode, streamMode);

public:
  void setStreamDevice(Mode m, QIODevice *d) { _streamMode = m; _ts.setDevice(d); _ds.setDevice(d); }

  virtual void setType(const SPropertyInformation *) = 0;

  virtual void setChildCount(xsize) = 0;
  virtual void beginNextChild() = 0;
  virtual void endNextChild() = 0;

  virtual void write(const SProperty *) = 0;

  virtual void beginAttribute(const char *) = 0;
  virtual void endAttribute(const char *) = 0;

  QTextStream &textStream() { return _ts; }
  QDataStream &binaryStream() { return _ds; }

private:
  QTextStream _ts;
  QDataStream _ds;
  };

template <typename T> void writeValue(SSaver &s, const T &t)
  {
  if(s.streamMode() == SSaver::Text)
    {
    s.textStream() << t;
    }
  else
    {
    s.binaryStream() << t;
    }
  }

template <typename T> void readValue(SLoader &l, T &t)
  {
  if(l.streamMode() == SLoader::Text)
    {
    l.textStream() >> t;
    }
  else
    {
    l.binaryStream() >> t;
    }
  }

inline void writeValue(SSaver &s, const QByteArray &t)
  {
  if(s.streamMode() == SSaver::Text)
    {
    s.textStream() << t.toHex();
    }
  else
    {
    s.binaryStream() << t;
    }
  }

inline void readValue(SLoader &l, QByteArray &t)
  {
  if(l.streamMode() == SLoader::Text)
    {
    QByteArray temp;
    l.textStream() >> temp;
    t = QByteArray::fromHex(temp);
    }
  else
    {
    l.binaryStream() >> t;
    }
  }


#endif // SLOADER_H
