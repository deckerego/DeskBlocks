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

BlocksBox::BlocksBox(DeskBlocks *deskBlocks)
{
  this->deskBlocks = deskBlocks;
  
  setIcon(QIcon(":/box/box.svg"));
  
  //Quit Action
  quitAction = new QAction(tr("&Quit"), this);
  connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
  
  //Build the menu
  trayIconMenu = new QMenu();
  trayIconMenu->addAction(quitAction);
  setContextMenu(trayIconMenu);
  
  //Tells us if a mouse click on the tray necessitates block creation
  connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(activation(QSystemTrayIcon::ActivationReason)));
  
  //Shut things down
  connect(this, SIGNAL(closed()), deskBlocks, SLOT(shutdown()));
}

void BlocksBox::closeEvent (QCloseEvent *event)
{
  emit closed();
  event->accept();
}

void BlocksBox::activation (QSystemTrayIcon::ActivationReason reason)
{
  switch (reason) {
    case QSystemTrayIcon::Trigger: //left clicked
      deskBlocks->dropBlock(QPoint(LENGTH, LENGTH));
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
