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

#ifndef CONVENTIONS_H
#define CONVENTIONS_H

/**
  As far as lengths and masses, from the ODE FAQ:
  "In general, length and mass values around 0.1..1.0 are 
  better as the factorizer may not lose so much precision. This 
  guideline is especially helpful when single precision is 
  being used."

  We'll assume that Qt's coordinate range is generally going to
  be x between 0 and 3200 and y between 0 and 1200 (targeting
  a maximum of a dual-screen setup with two 1600x1200 monitors).
  We'll also assume a "shape" won't be greater than 64x64.
  Given the above, the translation of coordinates from 
  Qt to ODE is 100 to 1.

  Density and length are given to Qt's scale, *not* ODE's.
  I'm assuming that (for the sake of Newtonian forces) 
  density would scale relative to size
 */
//I should probably replace these with inline functions
#define ABSOLUTE(position)  lrint(position*100.0)
#define RELATIVE(position)  ((dReal)position)/100.0

//Debug or no?
#ifndef DEBUG
#define DEBUG               FALSE
#endif

//What is the "normal" size of an object?
#define LENGTH              48
//How many objects can we spew at the same time?
#define MAX_BLOCKS          10
//How many times per second should we calculate object positions?
#define FRAMES_SEC          30
//How many ODE contact points should an object have?
#define MAX_CONTACTS        4

#endif
