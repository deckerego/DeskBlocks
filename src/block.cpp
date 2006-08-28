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

#include "block.h"

Block::Block(dSpaceID space, dWorldID world)
  : QWidget(0, Qt::FramelessWindowHint)
{
  dMass mass;  
  dMatrix3 rotation;
  body = dBodyCreate(world);
  
  //Set to top of screen
  dReal xPos = (dReal)frameGeometry().topLeft().x();
  dReal yPos = (dReal)frameGeometry().topLeft().y();
  dBodySetPosition(body, xPos, yPos, yPos);
  
  //No initial velocity
  dBodySetLinearVel(body, 0.0, 0.0, 0.0);
  
  //No initial rotation
  //dRFromAxisAndAngle(rotation, 0, 0, 1, dRandReal()*10.0-5.0);
  //dBodySetRotation(body, rotation);
  
  //User data for body
  size_t i = 0;
  dBodySetData(body, (void*)i);
  
  //Density of 5.0
  qDebug("Set mass for %f, %f, %f", (dReal)width(), (dReal)height(), (dReal)width());
  dMassSetBox(&mass, 5.0, (dReal)width(), (dReal)height(), (dReal)width());
  dBodySetMass(body, &mass);
  
  //Set collision space
  geometry = dCreateBox(space, (dReal)width(), (dReal)height(), (dReal)width());
  dGeomSetBody(geometry, body);
  dBodyEnable(body);
  
  qDebug("Created Block");
}

void Block::updatePosition()
{
  const dReal *position = dBodyGetPosition(body);
  //qDebug("New position at %f, %f, %f.", &position[0], &position[1], &position[2]);
  //QPoint *position = new QPoint((int)position[0], (int)position[1]);
  //move(position);
  update();
}

void Block::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) {
    dragPosition = event->globalPos() - frameGeometry().topLeft();
    event->accept();
  }
}

void Block::mouseMoveEvent(QMouseEvent *event)
{
  if (event->buttons() & Qt::LeftButton) {
    move(event->globalPos() - dragPosition);
    event->accept();
    dReal xPos = (dReal)frameGeometry().topLeft().x();
    dReal yPos = (dReal)frameGeometry().topLeft().y();
    dBodySetPosition(body, xPos, yPos, 1);
    const dReal *position = dBodyGetPosition(body);
    qDebug("Moved position at %f, %f, %f, %f, %f.", xPos, yPos, &position[0], &position[1], &position[2]);
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
  return QSize(48, 48);
}

void Block::paintEvent(QPaintEvent *)
{
  QPixmap pixmap(":/blocks/square.png");
  int side = qMin(width(), height());
  
  QPainter painter(this);
  painter.drawPixmap(width() / 2 - side / 2, height() / 2 - side / 2, pixmap);
}
