#include "scio.h"
#include "QIODevice"

ScFile::ScFile(const QString& file) : QFile(file)
  {
  }

bool ScFile::open(const QString &strMode)
  {
  QIODevice::OpenMode mode;
  if(strMode == "read")
    {
    mode = QIODevice::ReadOnly;
    }
  else if(strMode == "write")
    {
    mode = QIODevice::WriteOnly;
    }
  else
    {
    return false;
    }
  return QFile::open(mode);
  }

void ScFile::close()
  {
  QFile::close();
  }


ScIO::ScIO()
  {
  setObjectName("io");
  }

QObject *ScIO::createFile(const QString &path)
  {
  return new ScFile(path);
  }
