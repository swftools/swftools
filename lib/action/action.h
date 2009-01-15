/* action.h
 *
 * $Id: action.h,v 1.1 2004/05/08 17:09:35 kramm Exp $
 * 
 * Notice: This header file contains declarations of functions and types that
 * are just used internally. All library functions and types that are supposed
 * to be publicly accessable are defined in ./src/ming.h.
 */

#ifndef SWF_COMPILER_ACTION_H_INCLUDED
#define SWF_COMPILER_ACTION_H_INCLUDED

#include "ming.h"

enum
{
  SWFACTION_END        = 0x00,

/* v3 actions */
  SWFACTION_NEXTFRAME     = 0x04,
  SWFACTION_PREVFRAME     = 0x05,
  SWFACTION_PLAY          = 0x06,
  SWFACTION_STOP          = 0x07,
  SWFACTION_TOGGLEQUALITY = 0x08,
  SWFACTION_STOPSOUNDS    = 0x09,
  SWFACTION_GOTOFRAME     = 0x81, /* >= 0x80 means record has args */
  SWFACTION_GETURL        = 0x83,
  SWFACTION_WAITFORFRAME  = 0x8A,
  SWFACTION_SETTARGET     = 0x8B,
  SWFACTION_GOTOLABEL     = 0x8C,

/* v4 actions */
  SWFACTION_ADD                     = 0x0A,
  SWFACTION_SUBTRACT                = 0x0B,
  SWFACTION_MULTIPLY                = 0x0C,
  SWFACTION_DIVIDE                  = 0x0D,
  SWFACTION_EQUAL                   = 0x0E,
  SWFACTION_LESSTHAN                = 0x0F,
  SWFACTION_LOGICALAND              = 0x10,
  SWFACTION_LOGICALOR               = 0x11,
  SWFACTION_LOGICALNOT              = 0x12,
  SWFACTION_STRINGEQ                = 0x13,
  SWFACTION_STRINGLENGTH            = 0x14,
  SWFACTION_SUBSTRING               = 0x15,
  SWFACTION_POP                     = 0x17,
  SWFACTION_INT                     = 0x18,
  SWFACTION_GETVARIABLE             = 0x1C,
  SWFACTION_SETVARIABLE             = 0x1D,
  SWFACTION_SETTARGETEXPRESSION     = 0x20,
  SWFACTION_STRINGCONCAT            = 0x21,
  SWFACTION_GETPROPERTY             = 0x22,
  SWFACTION_SETPROPERTY             = 0x23,
  SWFACTION_DUPLICATECLIP           = 0x24,
  SWFACTION_REMOVECLIP              = 0x25,
  SWFACTION_TRACE                   = 0x26,
  SWFACTION_STARTDRAGMOVIE          = 0x27,
  SWFACTION_STOPDRAGMOVIE           = 0x28,
  SWFACTION_STRINGCOMPARE           = 0x29,
  SWFACTION_RANDOM                  = 0x30,
  SWFACTION_MBLENGTH                = 0x31,
  SWFACTION_ORD                     = 0x32,
  SWFACTION_CHR                     = 0x33,
  SWFACTION_GETTIMER                = 0x34,
  SWFACTION_MBSUBSTRING             = 0x35,
  SWFACTION_MBORD                   = 0x36,
  SWFACTION_MBCHR                   = 0x37,

  SWFACTION_WAITFORFRAMEEXPRESSION  = 0x8D,
  SWFACTION_PUSHDATA                = 0x96,
  SWFACTION_BRANCHALWAYS            = 0x99,
  SWFACTION_GETURL2                 = 0x9A,
  SWFACTION_BRANCHIFTRUE            = 0x9D,
  SWFACTION_CALLFRAME               = 0x9E,
  SWFACTION_GOTOEXPRESSION          = 0x9F,

/* v5 actions */
  SWFACTION_DELETEVAR               = 0x3A,/*not used yet*/
  SWFACTION_DELETE                  = 0x3B,
  SWFACTION_VAREQUALS               = 0x3C,
  SWFACTION_CALLFUNCTION            = 0x3D,
  SWFACTION_RETURN                  = 0x3E,
  SWFACTION_MODULO                  = 0x3F,
  SWFACTION_NEW                     = 0x40,
  SWFACTION_VAR                     = 0x41,
  SWFACTION_INITARRAY               = 0x42,
  SWFACTION_INITOBJECT              = 0x43,
  SWFACTION_TYPEOF                  = 0x44,
  SWFACTION_TARGETPATH              = 0x45,
  SWFACTION_ENUMERATE               = 0x46,
  SWFACTION_NEWADD                  = 0x47,
  SWFACTION_NEWLESSTHAN             = 0x48,
  SWFACTION_NEWEQUALS               = 0x49,
  SWFACTION_TONUMBER                = 0x4A,
  SWFACTION_TOSTRING                = 0x4B,
  SWFACTION_DUP                     = 0x4C,
  SWFACTION_SWAP                    = 0x4D,
  SWFACTION_GETMEMBER               = 0x4E,
  SWFACTION_SETMEMBER               = 0x4F,
  SWFACTION_INCREMENT               = 0x50,
  SWFACTION_DECREMENT               = 0x51,
  SWFACTION_CALLMETHOD              = 0x52,
  SWFACTION_NEWMETHOD               = 0x53,/*not used yet*/
  SWFACTION_INSTANCEOF              = 0x54,
  SWFACTION_ENUM2                   = 0x55,/*not used yet*/
  SWFACTION_BITWISEAND              = 0x60,
  SWFACTION_BITWISEOR               = 0x61,
  SWFACTION_BITWISEXOR              = 0x62,
  SWFACTION_SHIFTLEFT               = 0x63,
  SWFACTION_SHIFTRIGHT              = 0x64,
  SWFACTION_SHIFTRIGHT2             = 0x65,
  SWFACTION_STRICTEQ                = 0x66,
  SWFACTION_CONSTANTPOOL            = 0x88,
  SWFACTION_WITH                    = 0x94,
  SWFACTION_DEFINEFUNCTION          = 0x9B,

  SWFACTION_SETREGISTER             = 0x87
};

#endif /* SWF_COMPILER_ACTION_H_INCLUDED */
