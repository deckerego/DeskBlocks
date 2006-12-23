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

#include <QtGui>

#include "deskblocks.h"

DeskBlocks::DeskBlocks(QWidget *parent)
  : QWidget(parent)
{
  numBlocks = 0;
  
  //Do the ODE inits
  world = dWorldCreate();
  space = dHashSpaceCreate (0);
  contactGroup = dJointGroupCreate (0);
  dWorldSetGravity (world,0,GRAVITY,0);
  dWorldSetERP (world,ERROR_REDUCTION);
  dWorldSetAutoDisableFlag (world,1);
  dWorldSetContactSurfaceLayer (world,CONTACT_DEPTH);
  
  //Create the screen boundries
  createBounds();
  
  worldTimer = new QTimer(this);
  connect(worldTimer, SIGNAL(timeout()), this, SLOT(simLoop()));
}

DeskBlocks::~DeskBlocks()
{
  if(DEBUG) qDebug("Destroying DeskBlocks");
  for(int i = 0; i < numBlocks; i++)
    delete blocks[i];
  dJointGroupDestroy (contactGroup);
  dSpaceDestroy (space);
  dWorldDestroy (world);
}

void DeskBlocks::start()
{
  worldTimer->start(1000/FRAMES_SEC);
}

void DeskBlocks::dropBlock(QPoint origin)
{
  if(numBlocks >= MAX_BLOCKS) return; //No more blocks!
  
  blocks[numBlocks] = new Block(this, origin);
  connect(this, SIGNAL(odeUpdated()), blocks[numBlocks], SLOT(updatePosition()));
  blocks[numBlocks++]->show();
}

void DeskBlocks::shutdown()
{
  delete this;
}

/**
 * The Plane2D joint is what changes ODE's coordinate system around -
 * instead of having to map ODE's z axis to Qt's y axis, we use ODE's
 * 2D constraint. It was around since 0.35 evidentally, but just merged
 * in since release version 0.7.
 */
void DeskBlocks::createBounds()
{
  QDesktopWidget *screen = QApplication::desktop();
  int halfLength = LENGTH / 2;
  int height = screen->height();
  int width = screen->width();
  if(DEBUG) qDebug("Window dimensions are: %i x %i", height, width);
  
  // Our point in space used for collision is the midpoint of the shape - so we add
  // half the length to ensure we create boundries that exist on either extremity
  dCreatePlane (space,0,-1,0,RELATIVE(-height + halfLength)); // normal on Y axis pointing backward, bottom bounds
  dCreatePlane (space,0,1,0,RELATIVE(halfLength)); // normal on Y axis pointing forward, top bounds
  dCreatePlane (space,1,0,0,RELATIVE(-halfLength)); // normal on X axis pointing forward, left bounds
  dCreatePlane (space,-1,0,0,RELATIVE(-width + halfLength)); // normal on X axis pointing backward, right bounds
}

void DeskBlocks::detectCollision(dGeomID object1, dGeomID object2)
{
  int i = 0;
  dBodyID body1 = dGeomGetBody(object1);
  dBodyID body2 = dGeomGetBody(object2);
  if (body1 && body2 && dAreConnectedExcluding (body1, body2, dJointTypeContact)) return;
  
  dContact contact[MAX_CONTACTS];
  for (i=0; i<MAX_CONTACTS; i++) {
    contact[i].surface.mode = dContactSoftERP;
    contact[i].surface.mu = dInfinity;
    contact[i].surface.mu2 = 0;
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
  dSpaceCollide(space, this, &nearCallback);
  dWorldQuickStep(world, ODE_STEPS);
  dJointGroupEmpty(contactGroup);
  emit odeUpdated();
}

