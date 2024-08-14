
load("libsq_maker.so");


v <- Variables("mybinary");

function main(d)
{
    print ("Maker  default variables");
    local vars = v.get();
    foreach(k,val in vars)
    {
        print("*" + k +"=" + val);
    }
    make(vars);
}


function make(vars)
{
    vars["PWD"] = "/sdaone/GIT_HUB/sqechosystem";
    vars["FILES"] = ls(vars,".cpp");
    vars["CXX_FLAGS"] += " -I./ -Isq/include -Isq/squirrel -Isq/sqrat/include -Isq/sqrat -Isq/sqrat/include/sqrat"
    print("----------------------------\n");
    tprint(vars);
    print("----------------------------\n");
    local objs = compile(vars,"CXX");
    aprint(objs);

    if(objs.len())
    {
        link(vars, objs, "CXX");
    }
}
