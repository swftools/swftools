/* swfbutton.c

   Button functions

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2000, 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

int ButtonSetRecord(TAG * t,U8 state,U16 id,U16 layer,MATRIX * m,CXFORM * cx)

{ SetU8(t,state);
  SetU16(t,id);
  SetU16(t,layer);
  SetMatrix(t,m);
//  SetCXForm(t,cx,0);
  return 0;
}

int ButtonSetCondition(TAG * t,U16 condition)
{ SetU16(t,0); // dummy for Action Offset -> later set by ButtonPostProcess
  SetU16(t,condition);
  return 0;
}

int ButtonSetFlags(TAG * t,U8 flags)
{ if (GetTagID(t)==ST_DEFINEBUTTON2)
  { SetU8(t,flags);
    SetU16(t,0); // dummy for Action Offset -> later set by ButtonPostProcess
  }
  return 0;
}

void SetButtonOffset(TAG * t,U32 offsetpos)
{ U32 now = GetTagPos(t);
  U16 diff = now-offsetpos;
  SetTagPos(t,offsetpos);
  t->data[t->pos++] = (U8)(diff&0xff);
  t->data[t->pos++] = (U8)(diff>>8);
  SetTagPos(t,now);
}

int ButtonPostProcess(TAG * t,int anz_action)
{ if (GetTagID(t)==ST_DEFINEBUTTON2)
  { U32 oldTagPos;
    U32 offsetpos;

    oldTagPos = GetTagPos(t);

    // scan DefineButton2 Record
    
    GetU16(t);          // Character ID
    GetU8(t);           // Flags;

    offsetpos = GetTagPos(t);  // first offset
    GetU16(t);

    while (GetU8(t))      // state  -> parse ButtonRecord
    { GetU16(t);          // id
      GetU16(t);          // layer
      GetMatrix(t,NULL);  // matrix
      // evtl.: CXForm
    }

    SetButtonOffset(t,offsetpos);

    while(anz_action)
    { U8 a;
        
      offsetpos = GetTagPos(t); // offset
      GetU16(t);

      GetU16(t);                // condition
      
      while (a=GetU8(t))        // skip action records
      { if (a&0x80)
        { U16 l = GetU16(t);
          GetBlock(t,NULL,l);
        }
      }
      
      if (--anz_action) SetButtonOffset(t,offsetpos);
    }
    
    SetTagPos(t,oldTagPos);
  }
  return 0;
}
