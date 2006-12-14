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

Block::Block(DeskBlocks *parent)
  : QWidget(0, Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint)
{
  if(! parent) return;
  
  texture = QPixmap(":/blocks/square.png");
  
  bitmask = QBitmap(":/blocks/square.png");
  bitmask.fill(Qt::black); //Make everything the background
  
  dMass mass;
  dReal length = RELATIVE(LENGTH);
  dReal density = RELATIVE(DENSITY);
  
  body = dBodyCreate(parent->world);
  
  //Use ODE's new constraint for 2D
  dJointID planeJoint = dJointCreatePlane2D(parent->world, 0);
  dJointAttach(planeJoint, body, 0);
  
  //Set to top of screen
  int xPos = frameGeometry().topLeft().x();
  int yPos = frameGeometry().topLeft().y();
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
  dMatrix3& odeRotation = *(dMatrix3*)dGeomGetRotation(geometry);
  // * The story of dx, dy *
  //We need to change the pivot point of the rotation, so we need to translate the object to
  //the origin and then translate it back. That means the matrix operation is 
  //T(x, y) * R(theta) * T(-x, -y)
  //if we multiply that out:
  //dx = x * (1 - cos(theta)) + y * sin(theta)
  //dy = y * (1 - cos(theta)) - x * sin(theta)
  //Of course, we already know sin(theta), -sin(theta) and cos(theta). So we'll just reuse those.
  //int midpoint = LENGTH >> 1;
  int midpoint = 34;
  qreal dx = midpoint * (1 - odeRotation[0]);
  dx += midpoint * odeRotation[4];
  qreal dy = midpoint * (1 - odeRotation[5]);
  dy += midpoint * odeRotation[1];

  //* The story of m11, m12, m22 and m23 *
  //ODE has its origin starting at the lower left-hand corner, while Qt's starts in the upper right.
  //Since a positive angle represents a counter-clockwise turn in ODE, we'll need to invert
  //the turn in Qt to make it clockwise (relative to Qt's coordinate system). The nice thing
  //is that since this is a matrix identity ([a b][-b a]) we just have to swap values.
  canvasRotation = QMatrix(odeRotation[0], odeRotation[4], odeRotation[1], odeRotation[5], dx, dy);
  maskRotation = QMatrix(odeRotation[0], odeRotation[4], odeRotation[1], odeRotation[5], 10, 10);
  
  //Update position
  dReal *position = (dReal*)dGeomGetPosition(geometry);
  int xPos = ABSOLUTE(position[0]), yPos = ABSOLUTE(position[1]);
  
  if(DEBUG) qDebug("Position: %i (%f), %i (%f)", xPos, position[0], yPos, position[1]);
  
  // Update Qt's position
  move(xPos, yPos);
  update();
}

void Block::mouseMoveEvent(QMouseEvent *event)
{
  if (event->buttons() & Qt::LeftButton) {
    //Move the block on Qt's desktop
    QPoint currentPosition = event->globalPos() - dragPosition;
    
    //Move the object in ODE space
    dReal xPos = (dReal)currentPosition.x();
    dReal yPos = (dReal)currentPosition.y();
    
    //Reset the rotation - make it "flat" again
    dMatrix3 odeRotation;
    dRSetIdentity (odeRotation);
    dBodySetRotation(body, odeRotation);
    
    //Reset any velocity the object had, replace it with mouse velocity.
    //Since we're hard-coding the frequency of updates, use that value
    //to calculate the time delta instead of actually watching the clock
    int xDelta = (currentPosition.x() - lastPosition.x()) * FRAMES_SEC;
    int yDelta = (currentPosition.y() - lastPosition.y()) * FRAMES_SEC;
    
    //Commit object changes at once
    move(currentPosition);
    dGeomSetPosition(geometry, RELATIVE(xPos), RELATIVE(yPos), 0);
    dBodySetLinearVel(body, RELATIVE((dReal)xDelta), RELATIVE((dReal)yDelta), 0.0);
    if(DEBUG) qDebug("Moved to %f, %f", xPos, yPos);
    lastPosition = currentPosition;
    
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
  //a^2 + b^2 = c^2. I remembered something from elementary school!
  //In all fairness, this is actually square(side * (side * 2))
  //int boundingLength = (int)sqrt(LENGTH * (LENGTH << 1));
  int boundingLength = 68;
  if(DEBUG) qDebug("Calculating size hint: %i", boundingLength);
  return QSize(boundingLength, boundingLength);
}

void Block::paintEvent(QPaintEvent *)
{
  QRegion maskedRegion(bitmask.transformed(maskRotation));
  setMask(maskedRegion);
  
  QLinearGradient linearGradient(0, 0, 100, 100);
  linearGradient.setColorAt(0.0, Qt::white);
  linearGradient.setColorAt(0.2, Qt::green);
  linearGradient.setColorAt(1.0, Qt::black);
  
  QPainter painter(this);
  painter.save();
  painter.setMatrix(canvasRotation, false);
  painter.fillRect(10, 10, LENGTH, LENGTH, linearGradient);
  painter.restore();
}
