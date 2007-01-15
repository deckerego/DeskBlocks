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
#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#include <QWidget>
#include <ode/ode.h>

#include "conventions.h"

/**
	@author John T. Ellis <jtellis@alumni.indiana.edu>
 */
 
class QTimer;
class Block;
 
class Playground : public QWidget
{
  Q_OBJECT
      
  public:
    Playground(QWidget *parent = 0);
    ~Playground();
    
    void start();
    void detectCollision(dGeomID object1, dGeomID object2);
    void dropBlock(QPoint origin);
    
    dWorldID world;
    dSpaceID space;
    
  signals:
    void odeUpdated();
    
  private slots:
    void shutdown();
    void simLoop();
    
  private:
    void createBounds();
    
    QTimer *worldTimer;
    dJointGroupID contactGroup;
    Block *blocks[MAX_BLOCKS];
    int numBlocks;
};

#endif
