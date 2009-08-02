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

#include "playground.h"
#include "blocks/square.h"
#include "blocks/circle.h"

Playground::Playground(QWidget *parent)
  : QWidget(parent)
{
  numBlocks = 0;
  
  //Do the ODE inits
  world = dWorldCreate();
  space = dHashSpaceCreate (0);
  contactGroup = dJointGroupCreate (0);
  dWorldSetGravity (world,0,gravity,0);
  dWorldSetERP (world,errorReduction);
  dWorldSetAutoDisableFlag (world,1);
  dWorldSetContactSurfaceLayer (world,contactDepth);
  
  //Create the screen boundries
  createBounds();
  
  //Load preferences file
  settings = new QSettings();
  loadPrefs();

  //Connect the Qt timer with the ODE timer
  worldTimer = new QTimer(this);
  connect(worldTimer, SIGNAL(timeout()), this, SLOT(simLoop()));
  
  //Connect other Qt signals
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(shutdown()));
  
  dInitODE();
  qDebug("Created new playground");
}

//TODO Doing destructors explicity in Qt seems to result in a lot of memory leaks
Playground::~Playground()
{
  savePrefs();
  clear();
  dJointGroupDestroy (contactGroup);
  dSpaceDestroy (space);
  dWorldDestroy (world);
}

void Playground::clear()
{
  for(int i = 0; i < numBlocks; i++)
  {
    dBodyDestroy(blocks[i]->body);
    dGeomDestroy(blocks[i]->geometry);
    delete blocks[i];
  }
  
  numBlocks = 0;
}

// Let the record show I hate getter's and setter's. Yet sometimes they're
// actually useful when you want to obscure the object's actual property

void Playground::setFramesPerSecond(int framesPerSecond)
{
  qDebug("FPS set to: %i", framesPerSecond);
  this->framesPerSecond = framesPerSecond;
}

int Playground::getFramesPerSecond()
{
  return this->framesPerSecond;
}

void Playground::setMaximumBlocks(int maximumBlocks)
{
  qDebug("Max blocks set to: %i", maximumBlocks);
  this->maximumBlocks = maximumBlocks;
}

void Playground::setGravity(double gravity)
{
  qDebug("Gravity set to: %f", gravity);
  this->gravity = gravity;
  dWorldSetGravity (world,0,gravity,0);
}

double Playground::getGravity()
{
  return this->gravity;
}

void Playground::setErrorReduction(double errorReduction)
{
  qDebug("Error Reduction set to: %f", errorReduction);
  this->errorReduction = errorReduction;
  dWorldSetERP (world,errorReduction);
}

double Playground::getErrorReduction()
{
  return this->errorReduction;
}

void Playground::setCollisionErrorReduction(double collisionErrorReduction)
{
  qDebug("Collision Error Reduction set to: %f", collisionErrorReduction);
  this->collisionErrorReduction = collisionErrorReduction;
}

double Playground::getCollisionErrorReduction()
{
  return this->collisionErrorReduction;
}

void Playground::setODESteps(double odeSteps)
{
  qDebug("ODE Steps set to: %f", odeSteps);
  if(odeSteps <= 0.0) {
    qWarning("ODE Steps must be larger than 0, resetting to 0.01");
    this->odeSteps = 0.01;
  } else {
    this->odeSteps = odeSteps;
  }
}

double Playground::getODESteps()
{
  return this->odeSteps;
}

void Playground::loadPrefs()
{
  //Since ODE flips between single and double precision, warn if we're different than Qt
#ifdef dSINGLE
  qWarning("WARNING: ODE is using single precision");
#endif

  //Restore ODE settings
  settings->beginGroup("ODE");
  setGravity(settings->value("gravity", 10.0).toDouble());
  setErrorReduction(settings->value("error_reduction", 0.1).toDouble());
  contactDepth = settings->value("contact_depth", 0.01).toDouble();
  setODESteps(settings->value("steps", 0.01).toDouble());
  setCollisionErrorReduction(settings->value("collision_error_reduction", 0.09).toDouble());
  settings->endGroup();

  //Restore animation settings
  settings->beginGroup("Qt");
  setFramesPerSecond(settings->value("fps", 60).toInt());
  setMaximumBlocks(settings->value("maximumBlocks", 10).toInt());
  settings->endGroup();
}

void Playground::savePrefs()
{
  //Since ODE flips between single and double precision, warn if we're different than Qt
  #ifdef dSINGLE
    qWarning("WARNING: ODE is using single precision");
  #endif

  //Save ODE settings
  settings->beginGroup("ODE");
  settings->setValue("gravity", gravity);
  settings->setValue("error_reduction", errorReduction);
  settings->setValue("contact_depth", contactDepth);
  settings->setValue("steps", odeSteps);
  settings->setValue("collision_error_reduction", collisionErrorReduction);
  settings->endGroup();
  
  //Save animation settings
  settings->beginGroup("Qt");
  settings->setValue("fps", framesPerSecond);
  settings->setValue("maximumBlocks", maximumBlocks);
  settings->endGroup();
  
  settings->sync();
}

void Playground::start()
{
  worldTimer->start((int) 1000/framesPerSecond);
}

void Playground::dropBlock(QPoint origin, Playground::BlockType type)
{
  if(numBlocks >= maximumBlocks) return; //No more blocks!
  
  switch(type) {
    case Playground::SQUARE:
      blocks[numBlocks] = new Square(this, origin);
      break;
    case Playground::CIRCLE:
      blocks[numBlocks] = new Circle(this, origin);
      break;
  }
  
  connect(this, SIGNAL(odeUpdated()), blocks[numBlocks], SLOT(updatePosition()));
  blocks[numBlocks++]->show();
}

void Playground::shutdown()
{
  qDebug("Destroying Playground");
  delete this;
}

/**
 * The Plane2D joint is what changes ODE's coordinate system around -
 * instead of having to map ODE's z axis to Qt's y axis, we use ODE's
 * 2D constraint. It was around since 0.35 evidentally, but just merged
 * in since release version 0.7.
 */
void Playground::createBounds()
{
  QDesktopWidget *screen = QApplication::desktop();
  int halfLength = LENGTH / 2;
  int height = screen->height();
  int width = screen->width();
  qDebug("Window dimensions are: %i x %i", height, width);
  
  // Our point in space used for collision is the midpoint of the shape - so we add
  // half the length to ensure we create boundries that exist on either extremity
  dCreatePlane (space,0,-1,0,RELATIVE(-height + halfLength)); // normal on Y axis pointing backward, bottom bounds
  dCreatePlane (space,0,1,0,RELATIVE(halfLength)); // normal on Y axis pointing forward, top bounds
  dCreatePlane (space,1,0,0,RELATIVE(-halfLength)); // normal on X axis pointing forward, left bounds
  dCreatePlane (space,-1,0,0,RELATIVE(-width + halfLength)); // normal on X axis pointing backward, right bounds
}

void Playground::detectCollision(dGeomID object1, dGeomID object2)
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
    contact[i].surface.soft_erp = collisionErrorReduction;
  }
  
  if (int numCollisions = dCollide(object1, object2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact))) {
    qDebug("%i Collision(s)!", numCollisions);
    
    for (i=0; i<numCollisions; i++) {
      dJointID contactJoint = dJointCreateContact(world, contactGroup, contact+i);
      dJointAttach(contactJoint, body1, body2);
    }
  }
}

static void nearCallback(void *data, dGeomID object1, dGeomID object2)
{
  Playground *thisDeskBlock = (Playground*)data;
  thisDeskBlock->detectCollision(object1, object2);
}
    
void Playground::simLoop()
{
  dSpaceCollide(space, this, &nearCallback);
  dWorldQuickStep(world, odeSteps);
  dJointGroupEmpty(contactGroup);
  emit odeUpdated();
}

