/* swfbutton.c

   Button functions

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2000, 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#include "../rfxswf.h"

int swf_ButtonSetRecord(TAG * t,U8 state,U16 id,U16 layer,MATRIX * m,CXFORM * cx)

{ swf_SetU8(t,state);
  swf_SetU16(t,id);
  swf_SetU16(t,layer);
  swf_SetMatrix(t,m);
  if (swf_GetTagID(t)==ST_DEFINEBUTTON2) swf_SetCXForm(t,cx,1);
  return 0;
}

int swf_ButtonSetCondition(TAG * t,U16 condition)
{ swf_SetU16(t,0); // dummy for Action Offset -> later set by ButtonPostProcess
  swf_SetU16(t,condition);
  return 0;
}

int swf_ButtonSetFlags(TAG * t,U8 flags)
{ if (swf_GetTagID(t)==ST_DEFINEBUTTON2)
  { swf_SetU8(t,flags);
    swf_SetU16(t,0); // dummy for Action Offset -> later set by ButtonPostProcess
  }
  return 0;
}

void swf_SetButtonOffset(TAG * t,U32 offsetpos)
{ U32 now = swf_GetTagPos(t);
  U16 diff = now-offsetpos;
  swf_SetTagPos(t,offsetpos);
  t->data[t->pos++] = (U8)(diff&0xff);
  t->data[t->pos++] = (U8)(diff>>8);
  swf_SetTagPos(t,now);
}

int swf_ButtonPostProcess(TAG * t,int anz_action)
{ if (swf_GetTagID(t)==ST_DEFINEBUTTON2)
  { U32 oldTagPos;
    U32 offsetpos;

    oldTagPos = swf_GetTagPos(t);

    // scan DefineButton2 Record
    
    swf_GetU16(t);          // Character ID
    swf_GetU8(t);           // Flags;

    offsetpos = swf_GetTagPos(t);  // first offset
    swf_GetU16(t);

    while (swf_GetU8(t))      // state  -> parse ButtonRecord
    { swf_GetU16(t);          // id
      swf_GetU16(t);          // layer
      swf_GetMatrix(t,NULL);  // matrix
      swf_GetCXForm(t,NULL,1);// CXForm
    }

    swf_SetButtonOffset(t,offsetpos);

    while(anz_action)
    { U8 a;
        
      offsetpos = swf_GetTagPos(t); // offset
      swf_GetU16(t);

      swf_GetU16(t);                // condition
      
      while ((a=swf_GetU8(t)))        // skip action records
      { if (a&0x80)
        { U16 l = swf_GetU16(t);
          swf_GetBlock(t,NULL,l);
        }
      }
      
      if (--anz_action) swf_SetButtonOffset(t,offsetpos);
    }
    
    swf_SetTagPos(t,oldTagPos);
  }
  return 0;
}

ActionTAG* swf_Button1GetAction(TAG*tag)
{
    swf_GetU16(tag); //button id
    while(1)
    {
        U8 flags = swf_GetU8(tag);
        if(!flags)
            break; 
        swf_GetU16(tag); //char
        swf_GetU16(tag); //layer
        swf_ResetReadBits(tag);
        swf_GetMatrix(tag, NULL);
    }
    return swf_ActionGet(tag);
}

ActionTAG* swf_Button2GetAction(TAG*tag)
{
    swf_GetU16(tag); //button id
    swf_GetU8(tag); //flag
    U16 offset = swf_GetU16(tag); //offset
    swf_SetTagPos(tag, offset);
    swf_GetU16(tag); // next offset
    swf_GetU16(tag); // condition

    /* notice: this only returns the *first* action block.
       For the current appliances, this is enough.
     */
    return swf_ActionGet(tag);
}

ActionTAG* swf_ButtonGetAction(TAG*t)
{
    if(t->id == ST_DEFINEBUTTON) {
        return swf_Button1GetAction(t);
    } else if(t->id == ST_DEFINEBUTTON2) { 
        return swf_Button2GetAction(t);
    } else {
        fprintf(stderr, "error in buttongetaction: not a button tag\n");
        return 0;
    }
}

