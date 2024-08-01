#ifndef APICODE
#define APICODE
#define DUMMY_FUNCTION \
"function forml_dummy()"\
"end"
#define PRINT_FUNCTION \
"function printMgr(content)"\
"	pg.TipsMgr.GetInstance():ShowTips(content)"\
"end"
#define INCLUDE_FUNCTION \
"function include(path)"\
"	resultString = string.gsub(path, '%.', '/')"\
"	resultString = resultString .. '.lua'"\
"	cinclude(resultString)"\
"end"
#define PATH_EXISTS_FUNCTION \
"function pathExists(path)"\
"	resultString = string.gsub(path, '%.', '/')"\
"	resultString = resultString .. '.lua'"\
"	return cpathExists(resultString)"\
"end"
#define TAB_TO_STRING_FUNCTION \
"function tabtostring(t, rlimit, indent)"\
"    indent = indent or 0"\
"    rlimit = rlimit or 0"\
"    if indent > rlimit then"\
"		return 'recursion-limited'"\
"    end'"\
"    local result = '{\\n'"\
"    for k, v in pairs(t) do"\
"        local keyStr = type(k) == 'number' and '' or tostring(k) .. ' = '"\
"        if type(v) == 'table' then"\
"            result = result .. string.rep('  ', indent + 1) .. keyStr .. tabtostring(v, rlimit, indent + 1)"\
"        else"\
"            result = result .. string.rep('  ', indent + 1) .. keyStr .. tostring(v)"\
"        end"\
"        result = result .. ',\\n'"\
"    end"\
"    result = result:sub(1, -3)"\
"    result = result .. '\\n' .. string.rep('  ', indent) .. '}'"\
"    return result"\
"end"
#define TABLE_TO_STRING_FUNCTION \
"function tableToString(t, rlimit)"\
"	rlimit = rlimit or 0"\
"	return tabtostring(t,rlimit,0)"\
"end"
#define ALOG_FUNCTION \
"function alog(val)"\
"	calog(tostring(val))"\
"end"
/*
"local function tableToString(t, indent)"\
"    indent = indent or 0"\
"    local result = '{\\n'"\
"    for k, v in pairs(t) do"\
"        local keyStr = type(k) == 'number' and '' or tostring(k) .. ' = '"\
"        if type(v) == 'table' then"\
"            result = result .. string.rep('  ', indent + 1) .. keyStr .. tableToString(v, indent + 1)"\
"        else"\
"            result = result .. string.rep('  ', indent + 1) .. keyStr .. tostring(v)"\
"        end"\
"        result = result .. ',\\n'"\
"    end"\
"    result = result:sub(1, -3)  -- Remove the trailing comma and newline"\
"    result = result .. '\\n' .. string.rep('  ', indent) .. '}'"\
"    return result"\
"end"
*/

#endif // !APICODE

