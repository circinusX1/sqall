
load("libsq_maker.so");


//dofile("somelib/make.js");


function main(vm,params)
{
    //local env = ENV("somelib.so");
    //local v = env.get();
    //tprint(v);

    pushd("./somelib");
        print("loading \n");
        ::run("./make.js");
        print("done \n");
    popd();


    return 0;
}
