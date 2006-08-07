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

Block::Block(QWidget *parent)
  : QWidget(parent, Qt::FramelessWindowHint)
{
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(1000);
  
  setWindowTitle(tr("DeskBlocks"));
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
