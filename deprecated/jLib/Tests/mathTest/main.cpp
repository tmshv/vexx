#include <QtCore/QCoreApplication>
#include "jLib.h"
#include "jLib.testEnvironment.h"

#include "xmmintrin.h"

using namespace jLib::math;

int main( )
    {
    //--------------------------------------------------------------------------------------------------------------
    // multi-Matrix tests
    //--------------------------------------------------------------------------------------------------------------

    //Multi Matrix begin, no need for multiple tests as it only really checks for correct uniform
    //transposes and conversion
        {
        matrix4x3 test4x3 = matrix4x3::IDENTITY;
        matrix4x4 test4x4 = matrix4x4::IDENTITY;
        matrix3x3 test3x3 = matrix3x3::IDENTITY;
        triple translate = triple( random::getF( -100, 100 ), random::getF( -10, 10 ), random::getF( -10, 10 ) );
        quaternion quat( random::getF( -100, 100 ), random::getF( -100, 100 ), random::getF( -100, 100 ), random::getF( -100, 100 ) );
        quat = quat.normalise();
        TESTEQUAL( test4x3.asMatrix4x4(), test4x4, "multi matrix equality test 1a", "" );
        TESTEQUAL( test4x4.asMatrix4x3(), test4x3, "multi matrix equality test 1b", "" );

        test4x3.setTranslation( translate );
        test4x4.setTranslation( translate );

        TESTEQUAL( test4x3.asMatrix4x4(), test4x4, "multi matrix equality test 2a", "" );
        TESTEQUAL( test4x4.asMatrix4x3(), test4x3, "multi matrix equality test 2b", "" );

        test3x3.setOrientation( quat );
        test4x3.setOrientation( quat );
        test4x4.setOrientation( quat );

        TESTEQUAL( test4x3.asMatrix4x4(), test4x4, "multi matrix equality test 3a", "" );
        TESTEQUAL( test4x4.asMatrix4x3(), test4x3, "multi matrix equality test 3b", "" );
        TESTEQUAL( test4x4.asMatrix3x3(), test3x3, "multi matrix equality test 3c", "" );
        TESTEQUAL( test4x3.asMatrix3x3(), test3x3, "multi matrix equality test 3d", "" );
        }

    //--------------------------------------------------------------------------------------------------------------
    // matrix4x4
    //--------------------------------------------------------------------------------------------------------------

    BEGINTESTLOOP( "Matrix4x4 Brute Force Test", x )
        {
        matrix4x4 matrix = matrix4x4::IDENTITY;

        do
            {
            for( unsigned int x=0; x<3; x++ )
                {
                triple v = triple( random::getF( -100, 100 ), random::getF( -10, 10 ), random::getF( -10, 10 ) ).normalise();
                for( unsigned int y=0; y<3; y++ )
                    {
                    matrix.CM( x, y ) = v[y];
                    }
                }
            } while( fcmp( matrix.determinant(), 0 ) );

        matrix.setTranslation( triple( random::getF( -100, 100 ), random::getF( -100, 100 ), random::getF( -100, 100 ) ) );

        LOOPTESTEQUAL( matrix, matrix.multiply( matrix4x4::IDENTITY ), "Matrix4x4 multiply 1", "" );
        LOOPTESTEQUAL( matrix.transpose().transpose(), matrix, "Matrix4x4 transpose", "" );
        for( unsigned int x=0; x<16; x++ )
            {
            LOOPTESTTRUE( fcmp( matrix.inverse().multiply( matrix ).ptr()[0], matrix4x4::IDENTITY.ptr()[0], 0.01 ), "Matrix multiply / inverse test", "" );
            }
        }
    ENDTESTLOOP

    //--------------------------------------------------------------------------------------------------------------
    // quaternion
    //--------------------------------------------------------------------------------------------------------------

    BEGINTESTLOOP( "Quaternion Euler Angle Brute Force Test", x )
        {
        triple angles = triple( random::getF( -M_PI_2+0.05, M_PI_2-0.05 ), random::getF( -M_PI_2+0.05, M_PI_2-0.05 ), random::getF( -M_PI_2+0.05, M_PI_2-0.05 ) );
        quaternion angleQuat = quaternion::fromEulerAngles( angles );
        LOOPTESTEQUAL( angles, angleQuat.eulerAngles(), "Quaternion Euler Angle Test", angles<<" "<<angleQuat<<" "<<angleQuat.eulerAngles() );

        }
    ENDTESTLOOP

    BEGINTESTLOOP( "Quaternion Brute Force Test", x )
        {
        quaternion quat( random::getF( -100, 100 ), random::getF( -100, 100 ), random::getF( -100, 100 ), random::getF( -100, 100 ) );
        quat = quat.normalise();
        matrix4x3 matrix = matrix4x3::IDENTITY;
        matrix.setOrientation( quat );
        triple matScale = matrix.scale();
        quaternion quat2 = matrix.orientation();
        float quatLength = quat2.length();
        quat2 = quat2.normalise();
        LOOPTESTTRUE( ( quat == quat2 ) || ( quat.negate() == quat2 ), "Quaternion test "<<x, "Return Quaternion = "<<quat2<<", Quat "<<quat<<", matScale"<<matScale<<", quatLength "<<quatLength );
        }
    ENDTESTLOOP

    //--------------------------------------------------------------------------------------------------------------
    // matrix3x3
    //--------------------------------------------------------------------------------------------------------------

    BEGINTESTLOOP( "Matrix3x3 Brute Force Test", x )
        {
        matrix3x3 matrix = matrix3x3::IDENTITY;

        do
            {
            for( unsigned int x=0; x<3; x++ )
                {
                triple v = triple( random::getF( -100, 100 ), random::getF( -100, 100 ), random::getF( -100, 100 ) ).normalise();
                for( unsigned int y=0; y<3; y++ )
                    {
                    matrix.CM( x, y ) = v[y];
                    }
                }
            } while( fcmp( matrix.determinant(), 0 ) );

        LOOPTESTEQUAL( matrix, matrix.multiply( matrix3x3::IDENTITY ), "Matrix3x3 multiply 1", "" );
        LOOPTESTEQUAL( matrix.transpose().transpose(), matrix, "Matrix3x3 transpose", "" );
        LOOPTESTEQUAL( matrix.inverse().multiply( matrix ), matrix3x3::IDENTITY, "Matrix inverse / multiply 2", "" );
        }
    ENDTESTLOOP

    //--------------------------------------------------------------------------------------------------------------
    // matrix4x3
    //--------------------------------------------------------------------------------------------------------------
    {
    matrix4x3 matrix = matrix4x3::IDENTITY;
    matrix4x3 matrix2 = matrix4x3::IDENTITY;
    triple zero;
    triple translate( 1, 2, 3 );
    triple scale( 5.5, 43, 32 );
    quaternion quat = quaternion( 100, 200, 300, 400 ).normalise();
    
    matrix.setTranslation( translate );

    TESTEQUAL( matrix.translation(), translate, "Matrix4x3 translation", matrix );
    TESTEQUAL( matrix.vectorMultiply( zero ), triple(), "Matrix4x3 no vector translation", matrix );
    TESTEQUAL( matrix.pointMultiply( zero ), matrix.translation(), "Matrix4x3 point translation", matrix.pointMultiply( zero ) );

    matrix2 = matrix.multiply( matrix );
    TESTEQUAL( matrix2.translation(), translate * 2, "Matrix4x3 Multiplication test 1", matrix.translation()<<" "<<(translate * 2) );

    matrix2 = matrix4x3::IDENTITY;
    matrix2.setOrientation( quat );
    matrix = matrix.multiply( matrix2 );
    matrix2 = matrix4x3::IDENTITY;
    matrix2.setScale( scale );
    matrix = matrix.multiply( matrix2 );
    matrix2 = matrix4x3::IDENTITY;
    matrix2.setTranslation( translate );
    matrix2.setScale( scale );
    matrix2.setOrientation( quat );

    TESTEQUAL( matrix2, matrix, "Matrix4x3 Multiplication test 2", "" );

    matrix2 = matrix.transpose();

    TESTEQUAL( matrix2.transpose(), matrix, "Matrix4x3 Transpose test", "" );

    matrix = matrix4x3::IDENTITY;
    matrix.setOrientation( quat );
    matrix.setTranslation( translate );

    TESTEQUAL( matrix.multiply( matrix.inverse() ), matrix4x3::IDENTITY, "Matrix Inverse test", "" );
    }
    return EXIT_SUCCESS;
    }
