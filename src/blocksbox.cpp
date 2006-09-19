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
#include <QVBoxLayout>
#include "blocksbox.h"

BlocksBox::BlocksBox(DeskBlocks *desktop, QWidget *parent)
  : QWidget(parent)
{
  setFixedSize(200, 120);
  setWindowTitle(tr("DeskBlocks"));

  QPushButton *quit = new QPushButton(tr("Quit"), this);
  quit->setFont(QFont("Times", 12, QFont::Bold));
  connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
  
  QPushButton *dropBlock = new QPushButton(tr("Drop Block"), this);
  dropBlock->setFont(QFont("Times", 12, QFont::Bold));
  connect(dropBlock, SIGNAL(clicked()), desktop, SLOT(dropBlock()));
  
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(quit);
  layout->addWidget(dropBlock);
  setLayout(layout);
}