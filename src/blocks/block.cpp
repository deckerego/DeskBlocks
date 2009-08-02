/***************************************************************************
 *   Copyright (C) 2006 by John T. Ellis                                   *
 *   jtellis@alumni.indiana.edu                                            *
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

Block::Block(Playground *parent, QPoint position, QBitmap *bitmask, int width, int height)
  : BaseWidget(bitmask, width, height) 
{
  if(! parent) return;

  playground = parent;
  
  playground->settings->beginGroup("ODE");
  density = playground->settings->value("density", 250).toInt();;
  playground->settings->endGroup();
  
  body = dBodyCreate(playground->world);
  
  //Use ODE's new constraint for 2D
  dJointID planeJoint = dJointCreatePlane2D(playground->world, 0);
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
  
  //Procedurally texture the object. Actual pixmaps take too long to draw.
  gradient = new QLinearGradient(0, 0, LENGTH, LENGTH);
  gradient->setColorAt(0.0, Qt::white);
  gradient->setColorAt(1.0, Qt::black);

  qDebug("Created Block");
}

void Block::updatePosition()
{
  //TODO I need to test and ensure the geometry is set, exit if not
  
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
    int xDelta = (currentPosition.x() - lastPosition.x()) * playground->getFramesPerSecond();
    int yDelta = (currentPosition.y() - lastPosition.y()) * playground->getFramesPerSecond();
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
  //TODO If ODE auto-disables an object I should skip the repaint... or at least the transformation
  qDebug("Repainting Block");
  
  //Rotate the bitmap to correspond to what ODE sees
  QBitmap regionMask = bitmask->transformed(rotation);
  
  //We just changed the width & height of the bitmap by rotating it. Next we should 
  //determine how far to shift the bitmap so it's centered inside of the window
  int xMargin = boundingLength - regionMask.width(); //window width - bitmap width
  xMargin >>= 1; // divide by 2
  int yMargin = boundingLength - regionMask.height(); //window height - bitmap height
  yMargin >>= 1; // divide by 2
  QRegion maskedRegion(regionMask);
  maskedRegion.translate(xMargin, yMargin);

  //Draw the object and mask the rest
  QPainterPath maskedPath;
  setMask(maskedRegion);
  maskedPath.addRegion(maskedRegion);
  QPainter painter(this);
  painter.fillPath(maskedPath, *gradient);
}
