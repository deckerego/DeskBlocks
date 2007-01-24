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

#include <QtGui>
#include <ode/ode.h>

#include "conventions.h"
#include "block.h"

/**
	@author John T. Ellis <jtellis@alumni.indiana.edu>
 */
 
class QTimer;
class Block;
 
class Playground : public QWidget
{
  Q_OBJECT
      
  public:
    enum BlockType {
      SQUARE,
      CIRCLE
    };

    Playground(QWidget *parent = 0);
    ~Playground();
    
    void start();
    void detectCollision(dGeomID object1, dGeomID object2);
    void dropBlock(QPoint origin, Playground::BlockType type);
    
    dWorldID world;
    dSpaceID space;
    QSettings *settings;
    
  signals:
    void odeUpdated();
    
  private slots:
    void shutdown();
    void simLoop();
    void clear();
    
  private:
    void createBounds();
    void loadPrefs();
    
    QTimer *worldTimer;
    dJointGroupID contactGroup;
    Block *blocks[MAX_BLOCKS];
    int numBlocks;
    
    dReal gravity;
    dReal errorReduction;
    dReal contactDepth;
    dReal odeSteps;
};

#endif
