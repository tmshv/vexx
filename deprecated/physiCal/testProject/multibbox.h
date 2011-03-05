#ifndef MULTIBBOX_H
#define MULTIBBOX_H

#include "global.h"
#include "bbox.h"
#include "hardPhysics/particle.h"

#if 0
class doubleSpring : public cal::forceGenerator
    {
public:
    cal::particle *primary;
    cal::particle *secondary;
    JFLOAT constant;
    JFLOAT restLength;
    doubleSpring( ) : prevVector( 0.0 ) { }
    void update( JFLOAT time, cal::particle *p=0 )
        {
        sseTriple force = ( secondary->position() - primary->position() );
        JFLOAT magnitude = ( force.length() - restLength ) * constant;

        sseTriple forceNorm = force.normalise();

        //jDebugNeat<<prevVector<<" "<<(prevVector != 0.0)<<" "<<(prevVector == 0.0)<<endl;

        /*if( prevVector != 0.0 && forceNorm.dotProduct( prevVector ) < -0.9 )
            {
            jDebugNeat<<"Flip Spring "<<forceNorm.dotProduct( prevVector )<<" "<<forceNorm<<" "<<prevVector<<endl;
            secondary->setVelocity( 0 );
            secondary->setPosition( primary->position()  );
            magnitude = fabs( -restLength ) * constant;
            force = ( secondary->position() - primary->position() );
            magnitude = fabs( force.length() - restLength ) * constant;
            forceNorm = force.normalise();
            }*/

        sseTriple exert = magnitude * forceNorm;

        if( exert != 0 && fabs(force.length() - restLength) < (exert.length() * time * time) )
            {
            exert = ( fabs(force.length() - restLength) / ( time * time ) ) * forceNorm;
            //jDebugNeat<<"Spring could apply too much force here - "<<fabs(force.length() - restLength)<<" "<<(exert.length() * time * time)<<endl;
            }

        if( forceNorm.dotProduct( prevVector ) < -0.9 )
            {
            secondary->setPosition( primary->position() + force.negate() );
            //jDebugNeat<<"Spring could have flipped"<<endl;
            }

        /*if( (force.length() - restLength) > 0.01 )
            {
            jDebugNeat<<"spring too long "<<exert<<endl;
            }
        if( (force.length() - restLength) < -0.01 )
            {
            jDebugNeat<<"spring too short "<<exert<<endl;
            }*/

        /*if( exert != 0 )
            {
            jDebugNeat<<"Force Exert "<<force.length()<<" -> "<<exert<<" -> "<<sseTriple( force.normalise() ).dotProduct( exert )<<endl;
            }*/

        secondary->applyForce( -exert );
        primary->applyForce( exert );

        if( forceNorm != 0 )
            {
            prevVector = forceNorm;
            }
        }

private:
    jLib::math::sseTriple prevVector;
    };

class multiBBox : jLib::graphics::primitives::pointArray
    {
public:
    multiBBox( jLib::graphics::entity::entityG *, unsigned int, unsigned int, unsigned int );

    jLib::vector <cal::particle> &particles();
    void update( JFLOAT );
    void debugLines( jLib::graphics::primitives::lineCloud & );
private:
    inline cal::particle &particleAt( unsigned int x, unsigned int y, unsigned int z )
        {
        jAssert( ( z*( _zMax + 1 )+y*( _yMax + 1 )+x*( _xMax + 1 ) ) < _particles.size() );
        return _particles[ z * ( ( _yMax + 1 ) * ( _xMax + 1 ) ) + y * ( _xMax + 1 ) + x ];
        }
    jLib::vector <cal::particle> _particles;
    jLib::vector <bBox> _boxes;
    jLib::vector <doubleSpring> _springs;
    unsigned int _xMax;
    unsigned int _yMax;
    unsigned int _zMax;
    };

#endif

#endif
