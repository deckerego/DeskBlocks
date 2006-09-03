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
  : QWidget(0, Qt::FramelessWindowHint)
{
  if(! parent) return;
  
  dMass mass;
  dReal length = RELATIVE(LENGTH);
  
  //dMatrix3 rotation;
  body = dBodyCreate(parent->world);
  
  //Set to top of screen
  int xPos = frameGeometry().topLeft().x();
  int yPos = frameGeometry().topLeft().y();
  dBodySetPosition(body, RELATIVE(xPos), 0, RELATIVE(yPos));
  
  //No initial velocity
  //dBodySetLinearVel(body, 0.0, 0.0, 0.0);
  
  //No initial rotation
  //dRFromAxisAndAngle(rotation, 0, 0, 1, dRandReal()*10.0-5.0);
  //dBodySetRotation(body, rotation);
  
  //Define initial mass
  dMassSetBox(&mass, DENSITY, length, length, length);
  dBodySetMass(body, &mass);
  
  //Set collision space
  geometry = dCreateBox(parent->space, length, length, length);
  dGeomSetBody(geometry, body);
  
  qDebug("Created Block");
}


/**
  ODE's coordinate system is the inverse of Qt's, 
  so the translation from Qt to ODE is:
  y = z, x = x and (when necessary) z = -y
 */
void Block::updatePosition()
{
  dReal *position = (dReal*)dGeomGetPosition(geometry);
  int xPos = ABSOLUTE(position[0]), yPos = ABSOLUTE(position[2]);
  
  qDebug("Position: %i (%f), %i (%f)", xPos, position[0], yPos, position[2]);
  
  move(xPos, yPos);
  update();
}

void Block::mouseMoveEvent(QMouseEvent *event)
{
  if (event->buttons() & Qt::LeftButton) {
    move(event->globalPos() - dragPosition);
    event->accept();
    dReal xPos = (dReal)frameGeometry().topLeft().x();
    dReal yPos = (dReal)frameGeometry().topLeft().y();
    dGeomSetPosition(geometry, RELATIVE(xPos), 0, RELATIVE(yPos));
  }
}

void Block::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) {
    dragPosition = event->globalPos() - frameGeometry().topLeft();
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
