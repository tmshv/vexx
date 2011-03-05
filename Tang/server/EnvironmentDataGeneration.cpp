
  for( ItemID id=0; id<=5; ++id )
    {
    QString itemDir = dataDir + QDir::separator() + QString::number(id) + QDir::separator();
    dir.mkdir(QString::number(id));
    for( int level=0; level<XEnvironmentItem::LevelMaximum; ++level )
      {
      QByteArray arr;
      if( level <= XEnvironmentItem::Renderable )
        {
        Item item;
        if(id == 0)
          {
          int size = 5;
          float spacing = 100;

          for(int z=-size; z<=size; ++z)
            {
            for(int x=-size; x<=size; ++x)
              {
              XTransform tr;
              tr.translate(x*spacing, 0, z*spacing);
              item.children() << Child(1, tr);
              }
            }

          item.bounds().setMinimum(XVector3D(-size*spacing-0.2, -0.2, -size*spacing-0.2));
          item.bounds().setMaximum(XVector3D(size*spacing+0.2, 0.2, size*spacing+0.2));
          }
        else if(id == 1)
          {
          int size = 5;
          float spacing = 10;

          item.children() << Child(4, XTransform(), 0, 200) << Child(3, XTransform(), 200, 1000);

          item.bounds().setMinimum(XVector3D(-size*spacing-1.0f, 0.0f, -size*spacing-1.5f));
          item.bounds().setMaximum(XVector3D((size-1)*spacing+1.0f, 3.2f, (size-1)*spacing+1.5f));
          }
        else if(id == 2)
          {
          // Cube
            {
            XShape shape;
              {
              XModeller mod(&shape.geometry());
              mod.drawCube(XVector3D(0.2,0,0),XVector3D(0,0.2,0),XVector3D(0,0,0.2));
              }

            item.bounds() = shape.geometry().computeBounds();
            shape.setShader( XShader( XShader::Default ) );
            item.shapes() << shape;
            }
          }
        else if(id == 3)
          {
          int size = 5;
          float spacing = 10;

          // Plane
            {
            XShape shape;
              {
              XModeller mod(&shape.geometry());
              float quadSize = 2*size*spacing-0.1f;
              mod.drawQuad(XVector3D(quadSize,0,0),XVector3D(0,0,-quadSize));
              }
            item.bounds() = shape.geometry().computeBounds();
            shape.setShader( XShader( XShader::Default ) );
            item.shapes() << shape;
            }
          }
        else if(id == 4)
          {
          int size = 5;
          float spacing = 10;

          for(int z=-size; z<size; ++z)
            {
            for(int x=-size; x<size; ++x)
              {
              XTransform tr;
              tr.translate(x*spacing, 0, z*spacing);
              item.children() << Child(2, tr, 50, 300) << Child(5, tr, 0, 50);
              }
            }

          item.bounds().setMinimum(XVector3D(-size*spacing-1.0f, 0.0f, -size*spacing-1.5f));
          item.bounds().setMaximum(XVector3D((size-1)*spacing+1.0f, 3.2f, (size-1)*spacing+1.5f));
          }
        else if(id == 5)
          {
          // Duck
            {
            XShape shape;

            shape.setShader( XShader( XShader::AmbientShader ) );

            shape.shader()->getVariable("ambientTexture")->setValue( XTexture( QImage("../Eks3D/testProject/duckCM.png") ) );

            // get the duck into a geometry object
            XColladaFile col("../Eks3D/testProject/duck.dae");
            XGeometry geo( col.geometry( col.geometryNames().front() ) );

            // Scale duck down a bit.
              {
              XModeller mod(&shape.geometry());

              XTransform tr;
              tr.scale( 0.02 );

              mod.setTransform( tr );
              mod.drawGeometry(geo);
              }

            item.bounds() = shape.geometry().computeBounds();
            item.shapes() << shape;
            }
          }

        // stream the item to the array
          {
          QDataStream subStream(&arr, QIODevice::ReadWrite);
          item.save( subStream, (Level)level );
          }
        }
      else if( level == XEnvironmentItem::Name )
        {
        QDataStream subStream(&arr, QIODevice::ReadWrite);
        subStream << QString("Wang Chung?");
        }
      else if( level == XEnvironmentItem::CubeMap )
        {
        QDataStream subStream(&arr, QIODevice::ReadWrite);
          {
          subStream << QImage("../Eks3D/testProject/cubeMap2.png");
          }
        }

      _itemData[id][(Level)level] = arr;
      }
    }