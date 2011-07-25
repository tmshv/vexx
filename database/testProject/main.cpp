#include "database.h"
#include "iostream"

using namespace std;
using namespace database;

struct manStruct
{
  int a;
  float b;
  void *cake;
};

TYPEDEF_DATAFIELD( manField, manStruct )

int main()
{
  DECLARE_FIELD_TYPE( intField, "int" )
  DECLARE_FIELD_TYPE( floatField, "float" )
  DECLARE_FIELD_TYPE( stringField, "string" )
  DECLARE_FIELD_TYPE( table, "table" )
  DECLARE_FIELD_TYPE( manField, "manStruct" )
  
  table manTable( "ManTable" );

  manTable.addChild( new manField( "firstMan" ) );

  manStruct tempMan;
  tempMan.a = 4;
  tempMan.b = 5.55;
  tempMan.cake = &manTable;

  manTable.getChildAs<manField>( "firstMan" )->set( tempMan );
  
  binaryStream st;
  manTable.getAsStream( st );
  
  st.writeOut( "temp.bin" );
  
  binaryStream st2;
  st2.readIn( "temp.bin" );
  
  table manTable2("SomeOtherMan");
  
  manTable2.setFromStream( st2 );
  
  manStruct finalMan = manTable2.getChildAs<manField>( "firstMan" )->get();
  
  std::cout<<"final Man: "<<finalMan.a<<",  "<<finalMan.b<<", "<<finalMan.cake<<std::endl;
  
  int x;
  std::cin >> x;
  return 1;
  fieldManager::empty();
}
