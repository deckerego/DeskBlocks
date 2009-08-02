/***************************************************************************
 *   Copyright (C) 2006 by John T. Ellis                                   *
 *   jtellis@alumni.indiana.edu                                            *
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
#ifndef PREFSWINDOW_H
#define PREFSWINDOW_H

#include "ui_preferences.h"
#include "playground.h"

class PrefsWindow : public QDialog
{
  Q_OBJECT;

  public:
    PrefsWindow(Playground *desktop, QWidget *parent = 0);
  
  private slots:
    void showPreferences();
    void on_gravitySlider_valueChanged(int value);
    void on_erpSlider_valueChanged(int value);
    void on_collisionERPSlider_valueChanged(int value);
    void on_stepsSlider_valueChanged(int value);
    
  private:
    Playground *playground;
    Ui::PrefsWindow ui;
};

#endif
