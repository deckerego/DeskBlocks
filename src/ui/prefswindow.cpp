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

//QtDesigner is probably one of the coolest things ever.
#include "prefswindow.h"

PrefsWindow::PrefsWindow(Playground *playground, QWidget *parent)
 : QDialog(parent)
{
  this->playground = playground;
  
  ui.setupUi(this);
}

void PrefsWindow::showPreferences()
 {
  ui.gravitySlider->setValue((int) playground->getGravity());
  ui.erpSlider->setValue((int) (playground->getErrorReduction() * 10.0));
  ui.collisionERPSlider->setValue((int) (playground->getCollisionErrorReduction() * 10.0));
  ui.stepsSlider->setValue((int) (playground->getODESteps() * 100.0));
  
  show();
 }
 
void PrefsWindow::on_gravitySlider_valueChanged(int value)
{
  playground->setGravity(value);
}

void PrefsWindow::on_erpSlider_valueChanged(int value)
{
  double actualValue = (double)value / 10;
  playground->setErrorReduction(actualValue);
}

void PrefsWindow::on_collisionERPSlider_valueChanged(int value)
{
  double actualValue = (double)value / 10;
  playground->setCollisionErrorReduction(actualValue);
}

void PrefsWindow::on_stepsSlider_valueChanged(int value)
{
  double actualValue = (double)value / 100;
  playground->setODESteps(actualValue);
}
