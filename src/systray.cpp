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

#include "systray.h"

SysTray::SysTray(Playground *playground)
{
  this->playground = playground;
  
  setIcon(QIcon(":/box/box.svg"));
  
  //Quit Action
  quitAction = new QAction(tr("&Quit"), this);
  connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
  
  //Clear Action
  clearAction = new QAction(tr("C&lear"), this);
  connect(clearAction, SIGNAL(triggered()), playground, SLOT(clear()));
  
  //Group of shape actions
  shapeActions = new QActionGroup(this);
  
  //Drop boxes - enabled by default
  dropSquareAction = new QAction(tr("&Squares"), shapeActions);
  dropSquareAction->setCheckable(true);
  dropSquareAction->setChecked(true);
  connect(dropSquareAction, SIGNAL(triggered()), this, SLOT(setSquares()));
  setSquares();

  //Drop circle
  dropCircleAction = new QAction(tr("&Circles"), shapeActions);
  dropCircleAction->setCheckable(true);
  connect(dropCircleAction, SIGNAL(triggered()), this, SLOT(setCircles()));
  
  //Build the menu
  trayIconMenu = new QMenu();
  trayIconMenu->addAction(dropSquareAction);
  trayIconMenu->addAction(dropCircleAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(clearAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(quitAction);
  setContextMenu(trayIconMenu);
  
  //Tells us if a mouse click on the tray necessitates block creation
  connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(activation(QSystemTrayIcon::ActivationReason)));
  
  //Shut things down
  connect(this, SIGNAL(closed()), playground, SLOT(shutdown()));
}

void SysTray::setSquares() { currentBlockType = Playground::SQUARE; }
void SysTray::setCircles() { currentBlockType = Playground::CIRCLE; }

void SysTray::closeEvent (QCloseEvent *event)
{
  emit closed();
  event->accept();
}

void SysTray::activation (QSystemTrayIcon::ActivationReason reason)
{
  switch (reason) {
    case QSystemTrayIcon::Trigger: //left clicked
      playground->dropBlock(QPoint(LENGTH, LENGTH), currentBlockType);
      break;
    case QSystemTrayIcon::DoubleClick: //left double-clicked
      break;
    case QSystemTrayIcon::MiddleClick: //middle clicked
      break;
    case QSystemTrayIcon::Context: //context menu requested
      break;
    case QSystemTrayIcon::Unknown: //who knows
      break;
    default:
      ;
  }
}
