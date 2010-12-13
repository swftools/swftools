#include "state.h"
#include "common.h"

state_t* state = 0;

void new_state()
{
    NEW(state_t, s);
    state_t*oldstate = state;
    if(state)
        memcpy(s, state, sizeof(state_t)); //shallow copy
    if(!s->imports) {
        s->imports = dict_new();
    }
    if(!s->import_toplevel_packages) {
        s->import_toplevel_packages = dict_new(); 
    }
    state = s;
    state->level++;
    state->has_own_imports = 0;    
    state->vars = dict_new(); 
    state->old = oldstate;
    state->new_vars = 0;

    state->namespaces = dict_new();
   
    if(oldstate)
        state->active_namespace_urls = list_clone(oldstate->active_namespace_urls);
}

void state_destroy(state_t*state)
{
    if(state->has_own_imports) {
        list_free(state->wildcard_imports);
        dict_destroy(state->imports);state->imports=0;
    }
    if(state->imports && (!state->old || state->old->imports!=state->imports)) {
        dict_destroy(state->imports);state->imports=0;
    }
    if(state->vars) {
        dict_destroy(state->vars);state->vars=0;
    }
    
    list_free(state->active_namespace_urls)
    state->active_namespace_urls = 0;
    
    free(state);
}

void old_state()
{
    if(!state || !state->old)
        syntaxerror("invalid nesting");
    state_t*leaving = state;
    
    state = state->old;

    if(as3_pass>1 && leaving->method && leaving->method != state->method && !leaving->method->inner) {
        methodstate_destroy(leaving->method);leaving->method=0;
    }
    if(as3_pass>1 && leaving->cls && leaving->cls != state->cls) {
        free(leaving->cls);
        leaving->cls=0;
    }

    state_destroy(leaving);
}

methodstate_t*methodstate_new()
{
    NEW(methodstate_t,m);
    m->allvars = dict_new();
    return m;
}

void methodstate_destroy(methodstate_t*m) 
{
    dict_destroy(m->unresolved_variables); m->unresolved_variables = 0;
    list_free(m->innerfunctions);m->innerfunctions=0;

    if(m->allvars) {
        DICT_ITERATE_DATA(m->allvars, void*, data) {free(data);}
        m->allvars = 0;
    }
}

dict_t*definitions=0;
void as3_set_define(const char*c)
{
    if(!definitions) 
        definitions = dict_new();
    if(!dict_contains(definitions,c))
        dict_put(definitions,c,0);
}

