/*
load("libsq_maker.so");


v <- ENV("mybinary");

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
    vars["CUR_DIR"] = "";
    vars["SRC_DIR"] = "";
    vars["FILES"] += ls(vars["CUR_DIR"],".cpp");
    //vars["FILES"] += ls(vars["CUR_DIR"]+"sq/sqrat",".h");
    vars["INCLUDES"] += "-I./ -I./sq/include -I./sq/squirrel -I./sq/sqrat/include -I./sq/sqrat -I./sq/sqrat/include/sqrat";
    vars["LIBS"] += "-ldl -lpthread -L"+vars["CUR_DIR"]+"/lib/ -lsquirrel-x86_64-Linux -lsqstdlib-x86_64-Linux";
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

*/


load("libsq_maker.so");


print("llllllllllllllllll\n");
