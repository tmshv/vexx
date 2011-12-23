function assert(exp)
  {
  if(!exp)
    {
    script.enableDebugging(true);

    throw arguments.length > 1 ? arguments[1] : "Assertion failed";
    return false;
    }
  return true;
  }

/**
 * Function : dump()
 * Arguments: The data - array,hash(associative array),object
 *    The level - OPTIONAL
 * Returns  : The textual representation of the array.
 * This function was inspired by the print_r function of PHP.
 * This will accept some data as the argument and return a
 * text that will be a more readable version of the
 * array/hash/object that is given.
 * Docs: http://www.openjs.com/scripts/others/dump_function_php_print_r.php
 */
function dumpInternal(arr,level)
  {
  var dumped_text = "";
  if(!level)
    {
    level = 0;
    }

  //The padding given at the beginning of the line.
  var level_padding = "";
  for(var j=0;j<level+1;j++)
    {
    level_padding += "  ";
    }

  if(typeof(arr) == 'object') //Array/Hashes/Objects
    {
    for(var item in arr)
      {
      var value = arr[item];

      if(typeof(value) == 'object') //If it is an array,
        {
        dumped_text += level_padding + "'" + item + "' ...\n";
        dumped_text += dump(value,level+1);
        }
      else
        {
        dumped_text += level_padding + "'" + item + "' => \"" + value + "\"\n";
        }
      }
    }
  else //Stings/Chars/Numbers etc.
    {
    dumped_text = "===>"+arr+"<===("+typeof(arr)+")";
    }
  return dumped_text;
  }

var dump = function()
  {
  for(var i=0; i<dump.arguments.length; ++i)
    {
    print(dumpInternal(dump.arguments[i]));
    }
  }

/*

// example node
db.addType({
  name: "NodeName",
  parent: SEntity,
  properties:
    [
      {
      name: "AttrName",
      type: FloatProperty,
      affects: [ "Output" ]
      },
      {
      name: "AttrName2",
      type: IntProperty,
      affects: [ "Output" ]
      },
      {
      name: "Output",
      type: FloatProperty,
      compute: function()
        {
        this.Output = this.AttrName * this.AttrName2;
        }
      }
    ]
  });

  */

SProperty.cancelBlock = function(fn)
  {
  this.beginBlock();
  try
    {
    fn()
    }
  catch(e)
    {
    this.endBlock(true);
    throw e;
    }
  this.endBlock(true);
  }

script.include("JSPartExporter.js");
script.includeFolder("jsEntities");
