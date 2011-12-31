import QtQuick 1.1
import "NodeCanvasContents"

Rectangle
  {
  id: nodecanvas
  color: "#343434"

  property real maxNodeZ: 0.0;

  function childIndex(index)
    {
    return visualModel.modelIndex(index);
    }

  function bringToTop(item)
    {
    maxNodeZ += 0.05;
    item.z = maxNodeZ;
    }

  function setRootToChildIndex(index)
    {
    var modelInd = childIndex(index);
    visualModel.rootIndex = modelInd;
    maxNodeZ = 0.0;
    }

  function setRootToParent()
    {
    var index = visualModel.parentModelIndex();
    visualModel.rootIndex = index;
    maxNodeZ = 0.0;
    }

  function setNodePosition(nodeIndex, pos)
    {
    var modelInd = childIndex(nodeIndex);
    db.setData(modelInd, "propertyPosition", pos);
    }

  function findPropertyItem(modelIndex)
    {
    if(!db.isValid(modelIndex))
      {
      return null;
      }

    if(db.isEqual(modelIndex, visualModel.rootIndex))
      {
      return [ nodecanvas ];
      }

    var parentIndex = db.parent(modelIndex);
    var parentItems = findPropertyItem(parentIndex);
    if(!parentItems)
      {
      // probably not displayed.
      return null;
      }

    var rowIndex = db.rowIndex(modelIndex);

    var nextItem = parentItems[parentItems.length - 1].getChildItem(rowIndex);
    if(!nextItem)
      {
      return null;
      }

    parentItems.push(nextItem);

    return parentItems;
    }

  function getChildItem(index)
    {
    return nodes.itemAt(index);
    }

  VisualDataModel
    {
    id: visualModel
    model: db
    delegate: Node
      {
      title: name
      x: propertyPosition.x
      y: propertyPosition.y
      colour: propertyColour
      }
    }

  MouseArea
    {
    anchors.fill: parent
    onDoubleClicked: nodecanvas.setRootToParent()
    }

  Repeater
    {
    id: nodes
    model: visualModel
    }
  }
