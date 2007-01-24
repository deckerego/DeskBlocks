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

#include "square.h"

Square::Square(Playground *parent, QPoint position)
  : Block(parent, position, new QBitmap(":/blocks/square.bmp"), LENGTH, LENGTH)
{
  dMass mass;
  
  dReal length = RELATIVE(LENGTH);
  dReal density = RELATIVE(this->density);

  //Define initial mass
  dMassSetBox(&mass, density, length, length, length);
  dBodySetMass(body, &mass);
  
  //Set collision space
  geometry = dCreateBox(parent->space, length, length, length);
  dGeomSetBody(geometry, body);
  
  //Procedurally texture the object. Actual pixmaps take too long to draw.
  gradient->setColorAt(0.0, Qt::white);
  gradient->setColorAt(0.2, Qt::blue);
  gradient->setColorAt(1.0, Qt::black);
}
