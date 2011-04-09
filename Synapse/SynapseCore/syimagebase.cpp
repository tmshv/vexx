#include "syimagebase.h"
#include "QImage"

S_PROPERTY_CONTAINER_DEFINITION(SyImageBase, SPropertyContainer)
    S_PROPERTY_DEFINITION(SFloatArrayProperty, image)
S_PROPERTY_CONTAINER_END_DEFINITION(SyImageBase, SPropertyContainer, saveContainer, loadContainer)

SyImageBase::SyImageBase()
  {
  }

void SyImageBase::loadImage(const QImage &imageIn)
{
    int width = imageIn.width();
    int height = imageIn.height();

    image.resize(width, height);

    for(int i = 0; i < height; ++i )
    {
        for(int j = 0; j < width; ++j)
        {
           image.setIndex(j, i, qRed(imageIn.pixel(i, j)));
        }
    }
}
