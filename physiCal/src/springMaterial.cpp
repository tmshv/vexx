#include "springMaterial.h"
#include "frameLog.h"

BEGIN_PHYSICAL_NAMESPACE

springMaterial::springMaterial( pReal dens, pReal rest, pReal d, pReal damp, pReal elastic, pReal uts )
        : _springConstant( d ), _springDamping( damp ), _elasticLimit( elastic ), _ultimateTensileStress( uts ),
        _restitution( rest ), _density( dens )
    {
    }

pReal springMaterial::constant( pReal ext, springMode mode ) const
    {
    // find the positive extension percent
    pReal absLen( fabs( ext - 1 ) );
    // if more than elastic limit
    if( absLen > _elasticLimit && mode == Loading )
        {
        // return a smaller spring constant
        return _springConstant * 0.1;
        }
    // otherwise normal constant
    return _springConstant;
    }

pReal springMaterial::calculatePlasticLength( pReal len, pReal rest ) const
    {
    // if the length is more than rest length
    if( len > rest )
        {
        // find the positive limit in percent
        pReal limit( rest * ( 1 + _elasticLimit ) );
        // if over
        if( len > limit )
            {
            // out new length is our current length ( we could add a gui springback type value
            return len;
            }
        }
    else
        {
        // find the negative limit in percent
        pReal limit( rest * ( 1 - _elasticLimit ) );
        if( len < limit )
            {
            // out new length is our current length ( we could add a gui springback type value
            return len;
            }
        }
    return rest;
    }

springMaterial springMaterial::infinite( )
    {
    // an infinite material
    return springMaterial( jLib::floatLimits::infinity(), 0.1, 0 );
    }

END_PHYSICAL_NAMESPACE
