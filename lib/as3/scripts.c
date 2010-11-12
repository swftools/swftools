/* scripts.c

   Some hardcoded abc scripts.

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2008,2009 Matthias Kramm <kramm@quiss.org>
 
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

#include <time.h>
#include "abc.h"

void swf_AddButtonLinks(SWF*swf, char stop_each_frame, char events)
{
    int num_frames = 0;
    int has_buttons = 0;
    TAG*tag=swf->firstTag;

    unsigned int checksum = 0;
    while(tag) {
        if(tag->id == ST_SHOWFRAME)
            num_frames++;
        if(tag->id == ST_DEFINEBUTTON || tag->id == ST_DEFINEBUTTON2)
            has_buttons = 1;
	crc32_add_bytes(checksum, tag->data, tag->len);
        tag = tag->next;
    }
    int t = time(0);
    checksum = crc32_add_bytes(checksum, &t, sizeof(t));

    unsigned char h[16];
    unsigned char file_signature[33];
    sprintf((char*)file_signature, "%x", checksum);

    char scenename1[80], scenename2[80];
    sprintf(scenename1, "rfx.MainTimeline_%s", file_signature);
    sprintf(scenename2, "rfx::MainTimeline_%s", file_signature);

    abc_file_t*file = abc_file_new();
    abc_method_body_t*c = 0;
   
    abc_class_t*cls = abc_class_new2(file, scenename2, "flash.display::MovieClip");
  
    TAG*abctag = swf_InsertTagBefore(swf, swf->firstTag, ST_DOABC);
    
    tag = swf_InsertTag(abctag, ST_SYMBOLCLASS);
    swf_SetU16(tag, 1);
    swf_SetU16(tag, 0);
    swf_SetString(tag, scenename1);

    c = abc_class_getstaticconstructor(cls, 0)->body;
    c->old.max_stack = 1;
    c->old.local_count = 1;
    c->old.init_scope_depth = 9;
    c->old.max_scope_depth = 10;

    __ getlocal_0(c);
    __ pushscope(c);
    __ returnvoid(c);

    c = abc_class_getconstructor(cls, 0)->body;
    c->old.max_stack = 3;
    c->old.local_count = 1;
    c->old.init_scope_depth = 10;
    c->old.max_scope_depth = 11;
    
    debugfile(c, "constructor.as");

    __ getlocal_0(c);
    __ pushscope(c);

    __ getlocal_0(c);
    __ constructsuper(c,0);

    __ getlex(c, "[package]flash.system::Security");
    __ pushstring(c, "*");
    __ callpropvoid(c, "[package]::allowDomain", 1);
    
    if(stop_each_frame || has_buttons) {
        int frame = 0;
        tag = swf->firstTag;
        abc_method_body_t*f = 0; //frame script
        while(tag && tag->id!=ST_END) {
            char framename[80];
            char needs_framescript=0;
            char buttonname[80];
            char functionname[80];
            sprintf(framename, "[packageinternal]rfx::frame%d_%s", frame, file_signature);
            
            if(!f && (tag->id == ST_DEFINEBUTTON || tag->id == ST_DEFINEBUTTON2 || stop_each_frame)) {
                /* make the contructor add a frame script */
                __ findpropstrict(c,"[package]::addFrameScript");
                __ pushuint(c,frame);
                __ getlex(c,framename);
                __ callpropvoid(c,"[package]::addFrameScript",2);

                f = abc_class_method(cls, 0, multiname_fromstring(framename))->body;
                f->old.max_stack = 3;
                f->old.local_count = 1;
                f->old.init_scope_depth = 10;
                f->old.max_scope_depth = 11;
                __ debugfile(f, "framescript.as");
                __ debugline(f, 1);
                __ getlocal_0(f);
                __ pushscope(f);
                if(stop_each_frame) {
                    __ findpropstrict(f, "[package]::stop");
                    __ callpropvoid(f, "[package]::stop", 0);
                }
            }

            if(tag->id == ST_DEFINEBUTTON || tag->id == ST_DEFINEBUTTON2) {
                U16 id = swf_GetDefineID(tag);
                sprintf(buttonname, "::button%d", swf_GetDefineID(tag));
                __ getlex(f,buttonname);
                __ getlex(f,"flash.events::MouseEvent");
                __ getproperty(f, "::CLICK");
                sprintf(functionname, "::clickbutton%d_%s", swf_GetDefineID(tag), file_signature);
                __ getlex(f,functionname);
                __ callpropvoid(f, "::addEventListener" ,2);

                needs_framescript = 1;

                abc_method_body_t*h =
                    abc_class_method(cls, 0, multiname_fromstring(functionname))->body;
                list_append(h->method->parameters, multiname_fromstring("flash.events::MouseEvent"));

                h->old.max_stack = 6;
                h->old.local_count = 2;
                h->old.init_scope_depth = 10;
                h->old.max_scope_depth = 11;
                __ getlocal_0(h);
                __ pushscope(h);

                ActionTAG*oldaction = swf_ButtonGetAction(tag);
                if(oldaction && oldaction->op == ACTION__GOTOFRAME) {
                    int framenr = GET16(oldaction->data);
                    if(!events) {
                        __ findpropstrict(h,"[package]::gotoAndStop");
                        __ pushuint(h,framenr+1);
                        __ callpropvoid(h,"[package]::gotoAndStop", 1);
                    } else {
                        char framename[80];
                        sprintf(framename, "frame%d_%s", framenr, file_signature);
                        __ getlocal_0(h); //this
                        __ findpropstrict(h, "[package]flash.events::TextEvent");
                        __ pushstring(h, "link");
                        __ pushtrue(h);
                        __ pushtrue(h);
                        __ pushstring(h, framename);
                        __ constructprop(h,"[package]flash.events::TextEvent", 4);
                        __ callpropvoid(h,"[package]::dispatchEvent", 1);
                    }
                } else if(oldaction && oldaction->op == ACTION__GETURL) {
                    if(!events) {
                        __ findpropstrict(h,"flash.net::navigateToURL");
                        __ findpropstrict(h,"flash.net::URLRequest");
                        // TODO: target _blank
                        __ pushstring(h,(char*)oldaction->data); //url
                        __ constructprop(h,"flash.net::URLRequest", 1);
                        __ callpropvoid(h,"flash.net::navigateToURL", 1);
                    } else {
                        __ getlocal_0(h); //this
                        __ findpropstrict(h, "[package]flash.events::TextEvent");
                        __ pushstring(h, "link");
                        __ pushtrue(h);
                        __ pushtrue(h);
                        __ pushstring(h,(char*)oldaction->data); //url
                        __ constructprop(h,"[package]flash.events::TextEvent", 4);
                        __ callpropvoid(h,"[package]::dispatchEvent", 1);
                    }
                } else if(oldaction) {
                    fprintf(stderr, "Warning: Couldn't translate button code of button %d to flash 9 abc action\n", id);
                }
                __ returnvoid(h);
                swf_ActionFree(oldaction);
            }
            if(tag->id == ST_SHOWFRAME) {
                if(f) {
                    __ returnvoid(f);
                    f = 0;
                }
                frame++;
            }
            tag = tag->next;
        }
        if(f) {
            __ returnvoid(f);
        }
    }
    __ returnvoid(c);

    tag = swf->firstTag;
    while(tag) {
        if(tag->id == ST_DEFINEBUTTON || tag->id == ST_DEFINEBUTTON2) {
            char buttonname[80];
            sprintf(buttonname, "::button%d", swf_GetDefineID(tag));
            multiname_t*s = multiname_fromstring(buttonname);
            //abc_class_slot(cls, multiname_fromstring(buttonname), s);
            abc_class_slot(cls, multiname_fromstring(buttonname), 
                                multiname_fromstring("flash.display::SimpleButton"));
        }
        tag = tag->next;
    }


    abc_script_t*s = abc_initscript(file);
    c = s->method->body;
    c->old.max_stack = 2;
    c->old.local_count = 1;
    c->old.init_scope_depth = 1;
    c->old.max_scope_depth = 9;

    __ getlocal_0(c);
    __ pushscope(c);
    __ getscopeobject(c, 0);
    __ getlex(c,"::Object");
    __ pushscope(c);
    __ getlex(c,"flash.events::EventDispatcher");
    __ pushscope(c);
    __ getlex(c,"flash.display::DisplayObject");
    __ pushscope(c);
    __ getlex(c,"flash.display::InteractiveObject");
    __ pushscope(c);
    __ getlex(c,"flash.display::DisplayObjectContainer");
    __ pushscope(c);
    __ getlex(c,"flash.display::Sprite");
    __ pushscope(c);
    __ getlex(c,"flash.display::MovieClip");
    __ pushscope(c);
    __ getlex(c,"flash.display::MovieClip");
    __ newclass(c,cls);
    __ popscope(c);
    __ popscope(c);
    __ popscope(c);
    __ popscope(c);
    __ popscope(c);
    __ popscope(c);
    __ popscope(c);
    __ initproperty(c,scenename2);
    __ returnvoid(c);

    //abc_method_body_addClassTrait(c, "rfx:MainTimeline", 1, cls);
    multiname_t*classname = multiname_fromstring(scenename2);
    abc_initscript_addClassTrait(s, classname, cls);
    multiname_destroy(classname);

    swf_WriteABC(abctag, file);
}

TAG*swf_AddAS3FontDefine(TAG*tag, U16 id, char*fontname)
{
    tag = swf_InsertTag(tag, ST_DOABC);
    abc_file_t*file = abc_file_new();

    //abc_class_t*cls = abc_class_new2(file, fontname, "flash.display::MovieClip");
    //abc_class_slot(cls, multiname_fromstring(fontname), multiname_fromstring("flash.text::Font"));

    abc_class_t*cls = abc_class_new2(file, fontname, "flash.text::Font");

    abc_script_t*s = abc_initscript(file);
    code_t*c = s->method->body->code;
    c = abc_getlocal_0(c);
    c = abc_pushscope(c);
    c = abc_getscopeobject(c, 0);
    c = abc_getlex(c,"flash.text::Font");
    c = abc_pushscope(c);
    c = abc_getlex(c,"flash.text::Font");
    c = abc_newclass(c,cls);
    c = abc_popscope(c);
    c = abc_initproperty(c, fontname);
    c = abc_returnvoid(c);
    s->method->body->code = c;

    abc_initscript_addClassTrait(s, multiname_fromstring(fontname), cls);
    swf_WriteABC(tag, file);
	
    tag = swf_InsertTag(tag, ST_SYMBOLCLASS);
    swf_SetU16(tag, 1);
    swf_SetU16(tag, id);
    swf_SetString(tag, fontname);

    return tag;
}

