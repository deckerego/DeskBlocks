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

#include "circle.h"

Circle::Circle(Playground *parent, QPoint position)
  : Block(parent, position, QBitmap(":/blocks/circle.bmp"), LENGTH, LENGTH)
{
  dReal radius = RELATIVE(LENGTH) / 2;
  dReal density = RELATIVE(this->density);

  //Define initial mass
  dMass mass;
  dMassSetSphereTotal(&mass, density, radius);
  qDebug("Creating circle with density %f, mass %f and radius %f", density, mass.mass, radius);
  dBodySetMass(body, &mass);
  
  //Set collision space
  geometry = dCreateSphere(parent->space, radius);
  dGeomSetBody(geometry, body);
  
  //We should determine how far to shift the bitmap so it's centered inside of the window
  xMargin = boundingLength - width; //window width - block width
  xMargin >>= 1; // divide by 2
  yMargin = boundingLength - height; //window height - block height
  yMargin >>= 1; // divide by 2
  
  //Procedurally texture the object. Actual pixmaps take too long to draw.
  gradient.setColorAt(0.0, Qt::white);
  gradient.setColorAt(0.2, Qt::red);
  gradient.setColorAt(1.0, Qt::black);
}

QRegion Circle::getRegion()
{
  if(! dBodyIsEnabled(body)) return currentRegion;
  qDebug("Recalculating Circle Region");

  //No need to rotate. It's a freakin' circle.
  currentRegion = QRegion(bitmask);
  currentRegion.translate(xMargin, yMargin);
  return currentRegion;
}
