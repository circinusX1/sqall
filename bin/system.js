


load("libsq_system.so");


function prdir(f)
{
    print(f);
}


function main(a)
{
    print(current_path());
    directory_iterator(prdir, "./");
}
