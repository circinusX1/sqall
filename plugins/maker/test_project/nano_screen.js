screen{
    name{'my screen'}
    size{800,600}
    render{EGL}
    title{my-screen}


    window{
        position{20,20,400,400}
        title{my-window}
        visible{true}
        box-layout{
            orientation:horizontal;
            alignment:middle;
            margin:10;
            spacing:10;
        }

        button{
            title{'image button'}
            id{204}
            callback{callback}
            visible{true}
            img{
                up:'rc/icon3.png';
                down:'rc/icon2.png';
            }
        }

        image{
            src:'rc/icon1.png';
            visible{true}
            size{256,256}
        }
    }

    widget{
        position{150,150,100,100}
        box-layout{
            orientation:horizontal;
            alignment:middle;
            margin:10;
            spacing:10;
        }
        button{
            title{'My third button'}
            type{popup-button}
            id{104}
            callback{callback}
            visible{true}
            style{NormalButton}
        }

        button{
            title{'My 4 button'}
            id{105}
            callback{callback}
            visible{true}
            style{NormalButton}
        }
        checkbox{
            title{'checkbox 1'}
            id{107}
            callback{ckcallback}
            visible{true}
        }

    }
}

