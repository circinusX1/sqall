::load("libsq_nano.so");



function callback(id,down)
{
     printf("called "+ id +" "+ down + "\n");
    return 0;
}

function ckcallback(id, checked)
{
    printf("called "+ id +" "+ checked + "\n");
    return 0;
}


main_loop("nano_screen.js");

