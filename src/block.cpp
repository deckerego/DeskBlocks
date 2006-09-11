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
  dRSetIdentity (rotation);
  dBodySetRotation(body, rotation);
  
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
    move(currentPosition);
    
    //Move the object in ODE space
    dReal xPos = (dReal)currentPosition.x();
    dReal yPos = (dReal)currentPosition.y();
    dGeomSetPosition(geometry, RELATIVE(xPos), RELATIVE(yPos), 0);
    if(DEBUG) qDebug("Moved to %f, %f", xPos, yPos);
    
    //Reset any velocity the object had, replace it with mouse velocity.
    //Since we're hard-coding the frequency of updates, use that value
    //to calculate the time delta instead of actually watching the clock
    int xDelta = (currentPosition.x() - lastPosition.x()) * FRAMES_SEC;
    int yDelta = (currentPosition.y() - lastPosition.y()) * FRAMES_SEC;
    dBodySetLinearVel(body, RELATIVE((dReal)xDelta), RELATIVE((dReal)yDelta), 0.0);
    
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

void Block::resizeEvent(QResizeEvent *)
{
  int side = qMin(width(), height());
  QRegion maskedRegion(width() / 2 - side / 2, height() / 2 - side / 2, side, side, QRegion::Rectangle);
  setMask(maskedRegion);
}

QSize Block::sizeHint() const 
{
  return QSize(LENGTH, LENGTH);
}

void Block::paintEvent(QPaintEvent *)
{
  QPixmap pixmap(":/blocks/square.png");
  int side = qMin(width(), height());
  
  QPainter painter(this);
  painter.drawPixmap(width() / 2 - side / 2, height() / 2 - side / 2, pixmap);
}
