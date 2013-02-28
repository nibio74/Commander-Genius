#include "CBlooglet.h"

#include <engine/galaxy/common/ai/CPlayerLevel.h>


namespace galaxy {
    
    
enum BLOOGLETACTIONS
{
A_BLOOGLET_RUN = 0,	/* Ordinary run action */
A_BLOOGLET_STUNNED = 4
};

const int RUN_SPEED = 45;


CBlooglet::CBlooglet(CMap *pmap, const Uint16 foeID, const int actionOff, const bool carriesGem,
		     const Uint32 x, const Uint32 y) :
CStunnable(pmap, foeID, x, y),
mTimer(0),
mCarriesGem(carriesGem)
{
	mActionMap[A_BLOOGLET_RUN] = (void (CStunnable::*)()) &CBlooglet::processRunning;
	mActionMap[A_BLOOGLET_STUNNED] = &CStunnable::processGettingStunned;
	
	setupGalaxyObjectOnMap(actionOff, A_BLOOGLET_RUN);
	
	xDirection = LEFT;
}



void CBlooglet::processRunning()
{
	// Move normally in the direction
	if( xDirection == RIGHT )
	{
		moveRight( RUN_SPEED );
	}
	else
	{
		moveLeft( RUN_SPEED );
	}
}


void CBlooglet::getTouchedBy(CSpriteObject &theObject)
{
	if(dead || theObject.dead)
		return;

	CStunnable::getTouchedBy(theObject);

	// Was it a bullet? Than make it stunned.
	if( dynamic_cast<CBullet*>(&theObject) )
	{
	    if(mCarriesGem)
	    {
		// TODO: If blooglet carried a gem it should fall that case it must jump out! That is a sprite item which can fall on the floor.
	    }
	    
	    setAction(A_BLOOGLET_STUNNED);
	    dead = true;
	    theObject.dead = true;
	}
	
	if( CPlayerLevel *player = dynamic_cast<CPlayerLevel*>(&theObject) )
	{
		player->push(*this);
	}
}


int CBlooglet::checkSolidD( int x1, int x2, int y2, const bool push_mode )
{
	turnAroundOnCliff( x1, x2, y2 );

	return CGalaxySpriteObject::checkSolidD(x1, x2, y2, push_mode);
}


void CBlooglet::process()
{
	performCollisions();
	
	performGravityHigh();

	if( blockedl )
		xDirection = RIGHT;
	else if(blockedr)
		xDirection = LEFT;

	if(!processActionRoutine())
	    exists = false;
	
	(this->*mp_processState)();

}

    
    
};