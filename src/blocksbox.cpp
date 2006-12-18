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

#include "blocksbox.h"

BlocksBox::BlocksBox(DeskBlocks *deskBlocks, QWidget *parent)
  : QSvgWidget(":/box/box.svg", parent)
{
  setWindowTitle(tr("DeskBlocks"));
  setWindowFlags(Qt::FramelessWindowHint);
  
  //Tells us if a mouse click on the tray necessitates block creation
  connect(this, SIGNAL(mousePressed(QPoint)), deskBlocks, SLOT(dropBlock(QPoint)));
  
  //Shut things down
  connect(this, SIGNAL(closed()), deskBlocks, SLOT(shutdown()));
}

void BlocksBox::closeEvent (QCloseEvent *event)
{
  emit closed();
  event->accept();
}  

void BlocksBox::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton) {
    emit mousePressed(event->globalPos());
    event->accept();
  }
}
