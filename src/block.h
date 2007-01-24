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
#ifndef BLOCK_H
#define BLOCK_H

#include <QtGui>
#include <ode/ode.h>

#include "basewidget.h"
#include "conventions.h"
#include "playground.h"

class Playground;
class QPoint;
class QLinearGradient;

/**
  @author John T. Ellis <jtellis@alumni.indiana.edu>
 */
class Block : public BaseWidget
{
  Q_OBJECT
      
  public:
    Block(Playground *parent, QPoint position, QBitmap *bitmask, int width, int height);
    
    dReal getMass() const;
    QSize sizeHint() const;

  public slots:
    void updatePosition();

  protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    
    int density;

    QLinearGradient *gradient;
    
  private:
    QPoint dragPosition;
    QPoint lastPosition;
};

#endif
