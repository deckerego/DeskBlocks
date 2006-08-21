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
#ifndef DESKBLOCKS_H
#define DESKBLOCKS_H

#include <QWidget>
#include <ode.h>
#include "block.h"

#ifdef dDOUBLE
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCappedCylinder dsDrawCappedCylinderD
#endif

/**
	@author John T. Ellis <jtellis@alumni.indiana.edu>
 */
 
class QTimer;
 
class DeskBlocks : public QWidget
{
  Q_OBJECT
      
  public:
    DeskBlocks();
    ~DeskBlocks();
    void start();

  protected:
    void simLoop();
    
  private:
    dWorldID world;
    dSpaceID space;
    dJointGroupID contactGroup;
    Block *block;
    QTimer *timer;
};

#endif
