#include "spropertyinformation.h"

SPropertyInformation::~SPropertyInformation()
  {
  if(dynamic && childMetaData)
    {
    for(xsize i=0; i<propertyCount; ++i)
      {
      delete childMetaData[i].affects;
      }
    delete [] childMetaData;
    }
  }

const SPropertyInformation::Child *SPropertyInformation::child(xsize index) const
  {
  const SPropertyInformation *metaData = this;
  xAssert(index < (metaData->propertyOffset + metaData->propertyCount));
  while(metaData && index < metaData->propertyOffset)
    {
    metaData = metaData->parentTypeInformation;
    }

  if(metaData)
    {
    xAssert(index >= metaData->propertyOffset && index < (metaData->propertyCount+metaData->propertyOffset));
    return &(metaData->childMetaData[index-metaData->propertyOffset]);
    }

  return 0;
  }

SPropertyInformation::DataKey g_maxKey = 0;
SPropertyInformation::DataKey SPropertyInformation::newDataKey()
  {
  return g_maxKey++;
  }

SPropertyInformation::Child::DataKey g_maxChildKey = 0;
SPropertyInformation::Child::DataKey SPropertyInformation::newChildDataKey()
  {
  return g_maxChildKey++;
  }
