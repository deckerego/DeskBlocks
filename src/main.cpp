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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QApplication>

#include "playground.h"
#include "systray.h"

int main(int argc, char *argv[])
{
  QCoreApplication::setOrganizationName("Desktop Distractions");
  QCoreApplication::setOrganizationDomain("desktopdistractions.com");
  QCoreApplication::setApplicationName("DeskBlocks");
  
  QApplication app(argc, argv);
  
  Playground *desktop = new Playground(); // The world blocks exist in
  SysTray *box = new SysTray(desktop); // The window the user interacts with
  
  box->show();
  desktop->start();
  return app.exec();
}
