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
  dWorldSetGravity (world,0,1.0,0);
  dWorldSetCFM (world,1e-5);
  dWorldSetAutoDisableFlag (world,1);
  dWorldSetContactMaxCorrectingVel (world,0.1);
  dWorldSetContactSurfaceLayer (world,0.001);
  dCreatePlane (space,0,0,1,0);
  
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
}
    
void DeskBlocks::simLoop()
{
  dSpaceCollide(space, 0, &nearCallback);
  dWorldQuickStep(world, 0.5);
  dJointGroupEmpty(contactGroup);
  block->updatePosition();
}

