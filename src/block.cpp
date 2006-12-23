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
#include <math.h>

#include "block.h"

Block::Block(DeskBlocks *parent, QPoint position)
  : BlockWidget(new QBitmap(":/blocks/square.bmp"), LENGTH, LENGTH)
{
  if(! parent) return;
  
  dMass mass;
  dReal length = RELATIVE(LENGTH);
  dReal density = RELATIVE(DENSITY);
  
  body = dBodyCreate(parent->world);
  
  //Use ODE's new constraint for 2D
  dJointID planeJoint = dJointCreatePlane2D(parent->world, 0);
  dJointAttach(planeJoint, body, 0);
  
  //Set to top of screen
  int xPos = position.x();
  int yPos = position.y();
  dBodySetPosition(body, RELATIVE(xPos), RELATIVE(yPos), 0);
  
  //No initial velocity
  dBodySetLinearVel(body, 0.0, 0.0, 0.0);
  
  //No initial rotation
  dMatrix3 odeRotation;
  dRSetIdentity (odeRotation);
  dBodySetRotation(body, odeRotation);
  
  //Define initial mass
  dMassSetBox(&mass, density, length, length, length);
  dBodySetMass(body, &mass);
  
  //Set collision space
  geometry = dCreateBox(parent->space, length, length, length);
  dGeomSetBody(geometry, body);
  
  if(DEBUG) qDebug("Created Block");
}

void Block::updatePosition()
{
  //Update rotation
  setRotation(*(dMatrix3*)dGeomGetRotation(geometry));
  
  //Update position
  setPosition((dReal*)dGeomGetPosition(geometry));
  
  update();
}

void Block::mouseMoveEvent(QMouseEvent *event)
{
  if (event->buttons() & Qt::LeftButton) {
    //Move the block on Qt's desktop
    QPoint currentPosition = event->globalPos() - dragPosition;
    
    //Reset the rotation - make it "flat" again
    resetRotation();
    
    //Reset any velocity the object had, replace it with mouse velocity.
    //Since we're hard-coding the frequency of updates, use that value
    //to calculate the time delta instead of actually watching the clock
    int xDelta = (currentPosition.x() - lastPosition.x()) * FRAMES_SEC;
    int yDelta = (currentPosition.y() - lastPosition.y()) * FRAMES_SEC;
    dBodySetLinearVel(body, RELATIVE((dReal)xDelta), RELATIVE((dReal)yDelta), 0.0);
    lastPosition = currentPosition;
    
    //Move the object
    setPosition(currentPosition);
    
    event->accept();
  }
}

void Block::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) {
    dragPosition = event->globalPos() - frameGeometry().topLeft();
    lastPosition = event->globalPos() - dragPosition;
    dBodyEnable(body); //Wake up the ODE object if it was auto-disabled
    event->accept();
  }
}

QSize Block::sizeHint() const 
{
  return QSize(boundingLength, boundingLength);
}

void Block::paintEvent(QPaintEvent *)
{
  QLinearGradient linearGradient(0, 0, LENGTH, LENGTH);
  linearGradient.setColorAt(0.0, Qt::white);
  linearGradient.setColorAt(0.2, Qt::blue);
  linearGradient.setColorAt(1.0, Qt::black);
  
  QRegion maskedRegion(bitmask->transformed(rotation));
  QPainterPath maskedPath;
  maskedPath.addRegion(maskedRegion);
  setMask(maskedRegion);
  
  QPainter painter(this);
  painter.save();
  painter.fillPath(maskedPath, linearGradient);
  painter.restore();
}
