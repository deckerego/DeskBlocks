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

#include "basewidget.h"

BaseWidget::BaseWidget(QBitmap *bitmask, int width, int height)
  : QWidget(0, Qt::SubWindow | Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint)
{
  this->bitmask = bitmask;
  this->width = width;
  this->height = height;
  //a^2 + b^2 = c^2. I remembered something from elementary school!
  boundingLength = (int)sqrt((width* width) + (height * height));
  if(DEBUG) qDebug("Calculating size hint: %i", boundingLength);
}

void BaseWidget::setPosition(dReal *position) {
  //Update position
  int xPos = ABSOLUTE(position[0]);
  int yPos = ABSOLUTE(position[1]);
  
  if(DEBUG) qDebug("Position: %i (%f), %i (%f)", xPos, position[0], yPos, position[1]);
  
  // Update Qt's position
  move(xPos, yPos);
}

void BaseWidget::setPosition(QPoint position) {
  dReal xPos = (dReal)position.x();
  dReal yPos = (dReal)position.y();
  
  // Update ODE's position
  dGeomSetPosition(geometry, RELATIVE(xPos), RELATIVE(yPos), 0);
  if(DEBUG) qDebug("Moved to %f, %f", xPos, yPos);

  // Update Qt's position
  move(position);
}

void BaseWidget::resetRotation()
{
  dMatrix3 odeRotation;
  dRSetIdentity (odeRotation);
  dBodySetRotation(body, odeRotation);
}

void BaseWidget::setRotation(const dMatrix3 odeRotation)
{
  //ODE has its origin starting at the lower left-hand corner, while Qt's starts in the upper
  //right. Since a positive angle represents a counter-clockwise turn in ODE, we'll need to invert
  //the turn in Qt to make it clockwise (relative to Qt's coordinate system). The nice thing
  //is that since this is a matrix identity ([a b][-b a]) we just have to swap values.
  rotation = QMatrix(odeRotation[0], odeRotation[4], odeRotation[1], odeRotation[5], 0.0, 0.0);
}
