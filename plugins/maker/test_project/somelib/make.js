
load("libsq_maker.so");


print("compiling library\n");

local env = ENV("somelib.so");
local v = env.get();

v["CUR_DIR"]="";
v["SRC_DIR"]="./somelib";
v["OUT_DIR"]="lib";
v["INCLUDES"]="-Isomelib";

print("cur dir = " + pwd() + " \n");

v["FILES"]=ls(v,".c");

tprint(v);

local objs = compile(v,"CC");
v["CC_LINKER_FLAGS"]="-shared -o";

link(v,objs,"CC");

