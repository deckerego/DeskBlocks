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
#ifndef QBASEWIDGET_H
#define QBASEWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QBitmap>
#include <QMatrix>
#include <ode/ode.h>

#include "conventions.h"

class DeskBlocks;
class QMatrix;
class QBitmap;

/**
  @author John T. Ellis <jtellis@alumni.indiana.edu>
 */
class BaseWidget : public QWidget
{
  Q_OBJECT
      
  public:
    BaseWidget(QBitmap *mask, int length, int width);
    
    void resetRotation();
    void setRotation(const dMatrix3 rotation);
    
    void setPosition(dReal *position);
    void setPosition(const QPoint position);
    
  protected:
    dBodyID body;
    dGeomID geometry;
    
    QMatrix rotation;
    QBitmap *bitmask;
    
    int boundingLength;
    int width;
    int height;
};

#endif
