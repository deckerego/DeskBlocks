/***************************************************************************
 *   Copyright (C) 2006 by John T. Ellis   *
 *   jtellis@alumni.indiana.edu   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QTimer>

#include "deskblocks.h"

DeskBlocks::DeskBlocks(QWidget *parent)
  : QWidget(parent)
{
  //Do the ODE inits
  world = dWorldCreate();
  space = dSimpleSpaceCreate (0);
  contactGroup = dJointGroupCreate (0);
  dWorldSetGravity (world,0,2.0,0);
  dWorldSetCFM (world,1e-5);
  dWorldSetAutoDisableFlag (world,1);
  dWorldSetContactMaxCorrectingVel (world,0.1);
  dWorldSetContactSurfaceLayer (world,0.001);
  dCreatePlane (space,0,-1,0,-450); // normal on Y axis pointing backward
  
  worldTimer = new QTimer(this);
  
  connect(worldTimer, SIGNAL(timeout()), this, SLOT(simLoop()));
  
  block = new Block(this);
  connect(worldTimer, SIGNAL(timeout()), block, SLOT(updatePosition()));
}

DeskBlocks::~DeskBlocks()
{
  qDebug("Destroying DeskBlocks");
  dJointGroupDestroy (contactGroup);
  dSpaceDestroy (space);
  dWorldDestroy (world);
}

void DeskBlocks::start()
{
  worldTimer->start(100);
  block->show();
}

static void nearCallback(void *data, dGeomID object1, dGeomID object2)
{
  int i = 0;
  DeskBlocks *thisDeskBlock = (DeskBlocks*)data;
  dBodyID body1 = dGeomGetBody(object1);
  dBodyID body2 = dGeomGetBody(object2);
  dContact contact[MAX_CONTACTS];
  
  for (i=0; i<MAX_CONTACTS; i++) {
    contact[i].surface.mode = dContactBounce | dContactSoftCFM;
    contact[i].surface.mu = dInfinity;
    contact[i].surface.mu2 = 0;
    contact[i].surface.bounce = 0.1;
    contact[i].surface.bounce_vel = 0.1;
    contact[i].surface.soft_cfm = 0.01;
  }
  
  if (int numCollisions = dCollide(object1, object2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact))) {
    qDebug("Worlds collided %i times!", numCollisions);
    
    for (i=0; i<numCollisions; i++) {
      dJointID contactJoint = dJointCreateContact(thisDeskBlock->world, thisDeskBlock->contactGroup, contact+i);
      dJointAttach(contactJoint, body1, body2);
    }
  }
}
    
void DeskBlocks::simLoop()
{
  dSpaceCollide(space, this, &nearCallback);
  dWorldQuickStep(world, 0.5);
  dJointGroupEmpty(contactGroup);
  block->updatePosition();
}

