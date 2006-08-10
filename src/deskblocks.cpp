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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QApplication>
#include <ode.h>

#include "block.h"

#ifdef dDOUBLE
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCappedCylinder dsDrawCappedCylinderD
#endif

static dWorldID world;
static dSpaceID space;
static dJointGroupID contactGroup;

int main(int argc, char *argv[])
{
  int retVal = -1;
  
  world = dWorldCreate();
  space = dHashSpaceCreate (0);
  contactGroup = dJointGroupCreate (0);
  dWorldSetGravity (world,0,0,-0.5);
  dWorldSetCFM (world,1e-5);
  dWorldSetAutoDisableFlag (world,1);
  dWorldSetContactMaxCorrectingVel (world,0.1);
  dWorldSetContactSurfaceLayer (world,0.001);
  dCreatePlane (space,0,0,1,0);
  
  QApplication app(argc, argv);
  Block block;
  block.show();
  
  retVal = app.exec();
  dJointGroupDestroy (contactGroup);
  dSpaceDestroy (space);
  dWorldDestroy (world);
}
