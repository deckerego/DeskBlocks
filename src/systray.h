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
#ifndef SYSTRAY_H
#define SYSTRAY_H

#include <QApplication>
#include <QFont>
#include <QPushButton>
#include <QSystemTrayIcon>

#include "playground.h"
#include "block.h"

/**
	@author John T. Ellis <jtellis@alumni.indiana.edu>
*/
class SysTray : public QSystemTrayIcon
{
  Q_OBJECT
      
  public:
    SysTray(Playground *desktop);
    
  signals:
    void closed();
    
  private slots:
    void activation(QSystemTrayIcon::ActivationReason reason);
    void setCircles();
    void setSquares();

  protected:
    void closeEvent (QCloseEvent *event);
    
  private:
    Playground *playground;
    
    //SysTray
    QActionGroup *shapeActions;
    QAction *quitAction;
    QAction *clearAction;
    QAction *dropSquareAction;
    QAction *dropCircleAction;
    QMenu *trayIconMenu;
    QSystemTrayIcon *trayIcon;
    
    Playground::BlockType currentBlockType;
};

#endif
