/* XMLOutputDev.h
   
   This file is part of swftools.

   Swftools is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Swftools is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with swftools; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#ifndef __xmloutputdev_h__
#define __xmloutputdev_h__

#include "OutputDev.h"
#include "TextOutputDev.h"

class XMLOutputDev: public TextOutputDev
{
    public:
	XMLOutputDev(char*filename);
        virtual void startPage(int pageNum, GfxState *state);
	virtual void endPage();
	virtual ~XMLOutputDev();
    private:
	FILE*out;
};
#endif //__xmloutputdev_h__
