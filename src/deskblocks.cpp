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
  space = dHashSpaceCreate (0);
  contactGroup = dJointGroupCreate (0);
  dWorldSetGravity (world,0,0,5.0);
  dWorldSetERP (world,0.1);
  dWorldSetAutoDisableFlag (world,0);
  dWorldSetContactSurfaceLayer (world,0.01);
  
  //Create the screen boundries
  createBounds();
  
  worldTimer = new QTimer(this);
  
  //TODO Create a menu-driven system for adding blocks
  for(numBlocks=0; numBlocks < 3; numBlocks++) {
    blocks[numBlocks] = new Block(this);
  }
  
  connect(worldTimer, SIGNAL(timeout()), this, SLOT(simLoop()));
}

DeskBlocks::~DeskBlocks()
{
  if(DEBUG) qDebug("Destroying DeskBlocks");
  dJointGroupDestroy (contactGroup);
  dSpaceDestroy (space);
  dWorldDestroy (world);
}

void DeskBlocks::start()
{
  int b;
  
  worldTimer->start(100);
  
  for(b = 0; b < numBlocks; b++) {
    blocks[b]->show();
  }
}

/**
  Create ODE boundries for the screen.
  ODE's coordinate system is the inverse of Qt's, 
  so the translation from Qt to ODE is:
  y = z, x = x and (when necessary) z = -y
 */
void DeskBlocks::createBounds()
{
  //TODO Replace the static boundries with ones pulled directly from Qt
  dCreatePlane (space,0,0,-1,RELATIVE(-954)); // normal on Y axis pointing backward, bottom bounds
  dCreatePlane (space,0,0,1,RELATIVE(0)); // normal on Y axis pointing forward, top bounds
  dCreatePlane (space,1,0,0,RELATIVE(0)); // normal on X axis pointing forward, left bounds
  dCreatePlane (space,-1,0,0,RELATIVE(-1300)); // normal on X axis pointing backward, right bounds
}

void DeskBlocks::detectCollision(dGeomID object1, dGeomID object2)
{
  int i = 0;
  dBodyID body1 = dGeomGetBody(object1);
  dBodyID body2 = dGeomGetBody(object2);
  if (body1 && body2 && dAreConnectedExcluding (body1, body2, dJointTypeContact)) return;
  
  dContact contact[MAX_CONTACTS];
  for (i=0; i<MAX_CONTACTS; i++) {
    contact[i].surface.mode = dContactBounce | dContactSoftERP;
    contact[i].surface.mu = dInfinity;
    contact[i].surface.mu2 = 0;
    contact[i].surface.bounce = 0.1;
    contact[i].surface.bounce_vel = 0.1;
    contact[i].surface.soft_erp = 0.9;
  }
  
  if (int numCollisions = dCollide(object1, object2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact))) {
    if(DEBUG) qDebug("%i Collision(s)!", numCollisions);
    
    for (i=0; i<numCollisions; i++) {
      dJointID contactJoint = dJointCreateContact(world, contactGroup, contact+i);
      dJointAttach(contactJoint, body1, body2);
    }
  }
}

static void nearCallback(void *data, dGeomID object1, dGeomID object2)
{
  DeskBlocks *thisDeskBlock = (DeskBlocks*)data;
  thisDeskBlock->detectCollision(object1, object2);
}
    
void DeskBlocks::simLoop()
{
  int b;
  dSpaceCollide(space, this, &nearCallback);
  dWorldQuickStep(world, 0.05);
  dJointGroupEmpty(contactGroup);
  
  for(b=0; b < numBlocks; b++) {
    blocks[b]->updatePosition();
  }
}

