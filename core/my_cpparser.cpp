
#include "logger/clogger.h"

#include "core/hook_functions.h"
#include "core/variables.h"



#define CALL_OLD_CPPARSER false


TValue* (*old_cpparser)(lua_State* instance, lua_CFunction dummy, void* ud);
TValue* my_cpparser(lua_State* L, lua_CFunction dummy, void* ud)
{
    //logMessage("Cpparser", "cpparser Called");
    if (CALL_OLD_CPPARSER)
    {
        //logMessage("Cpparser", "old cpparser Called");
        return old_cpparser(L, dummy, ud);
    }
        
    LexState* ls = (LexState*)ud;
    GCproto* pt;
    GCfunc* fn;
    int bc;
    UNUSED(dummy);
    cframe_errfunc(L->cframe) = -1;
    //logMessage("Cpparser", "lj_lex_setup");
    bc = AzurLaneL::lj_lex_setup(L, ls);
    //LOG_VALUE(value, "Bc value: %d", bc);
    if (ls->mode && !strchr(ls->mode, bc ? 'b' : 't')) {
        //logMessage(value, "LUA_ERROR");
        setstrV(L, L->top++, AzurLaneL::lj_err_str(L, LJ_ERR_XMODE));
        AzurLaneL::lj_err_throw(L, LUA_ERRSYNTAX);
    }
    //logMessage("Cpparser", "lj_bcread");
    //logValue("Cpparser", "LS: %p BC: %d", ls, bc);
    if (bc /* || !globvar::mod_loaded*/) {
        pt = AzurLaneL::lj_bcread(ls);
    }
    else {
        //logMessage("Cpparserp", "lj_parse");
        pt = (GCproto*)lj_parse(ls);
        //logMessage("Cpparserp", "lj_parse complete");
    }
    //LOG_VALUE(value, "PT value: %p", pt);
    //logMessage("Cpparser", "lj_func_newL_empty");
    fn = AzurLaneL::lj_func_newL_empty(L, pt, tabref(L->env));
    setfuncV(L, L->top++, fn);
    if (!bc) {
        //logMessage("Cpparserp", "return");
    }

    return NULL;//?
    //return old_cpparser(L, dummy, ud);
}