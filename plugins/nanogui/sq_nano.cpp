
#include "sq_nano.h"
#include "screen.h"
#include "window.h"
#include "layout.h"
#include "button.h"
#include "imageview.h"
#include "checkbox.h"
#include "cbml.h"


static      ssq::VM* PVM;
sq_api*     SQ_PTRS;
namespace   ng=nanogui ;
static      ng::Screen* PScreen;


void _recursize(Cbdler::Node* p, ng::Widget* parent);

namespace Sqrat
{
SQVM* vm = nullptr;
};


const char* sq_nano()
{
    return "ui/sq_nano";
}

static void buttcall(ng::Button* pb, bool press)
{
    //Sqrat::DefaultVM::Set(_vm);
    ssq::Function pf = PVM->findFunc("callback");
    if(!pf.isNull())
    {
        PVM->callFunc(pf, *PVM, pb->id(), press);
    }
}

static void checkcall(ng::CheckBox* pb, bool press)
{
    ssq::Function pf = PVM->findFunc("callback");
    if(!pf.isNull())
    {
        PVM->callFunc(pf, *PVM, pb->id(), press);
    }
}


template <class T=ng::Widget>
static void _attributes(const Cbdler::Node& p, T* w)
{
    const Cbdler::Node& t = p["size"];
    if(t.count()==2)
    {
        w->setSize(ng::Vector2i(t.ivalue(0),t.ivalue(1)));
        w->setFixedSize(ng::Vector2i(t.ivalue(0),t.ivalue(1)));
    }
    else
    {
        const Cbdler::Node& t = p["position"];
        if(t.count()>=2)
        {
            w->setPosition(ng::Vector2i(t.ivalue(0),t.ivalue(1)));
        }
        if(t.count()==4)
        {
            w->setSize(ng::Vector2i(t.ivalue(2),t.ivalue(3)));
        }
    }

    const Cbdler::Node& id = p["id"];
    if(id.count())
    {
        w->setId(id.value());
    }
    const Cbdler::Node& v = p["visible"];
    if(v.value()=="true")
    {
        w->setVisible(true);
    }
}

template <class T=ng::Widget>
static void _exattributes(const Cbdler::Node& p, T* w)
{
    const Cbdler::Node& fg = p["fg"];
    if(fg.count()==4)
    {
        ng::Color c(fg.ivalue(0), fg.ivalue(1), fg.ivalue(2), fg.ivalue(3));
        w->setTextColor(c);
    }
    const Cbdler::Node& bg = p["bg"];
    if(bg.count()==4)
    {
        ng::Color c(bg.ivalue(0), bg.ivalue(1), bg.ivalue(2), bg.ivalue(3));
        w->setBackgroundColor(c);
    }
    const Cbdler::Node& i = p["icon"];
    if(i.count())
    {
        int  icon = 0;
        ::sscanf(i.value(0).c_str(),"0x%X",&icon);
        w->setIcon(icon);
    }


    const Cbdler::Node& g = p["img"];
    if(g.count())
    {
        std::string src = g["up"].value();

        GLTexture* texture = new GLTexture(src);
        auto data = texture->load(src);
        GLuint tex[4]={texture->texture(),0,0,0};
        w->setImage(tex, ng::Vector2i(texture->w(), texture->h()));
        w->setSize(ng::Vector2i(texture->w(), texture->h()));
        w->setFixedSize(ng::Vector2i(texture->w(), texture->h()));
        w->setUserData("GLTexture1", texture);

        src = g["down"].value();
        if(src!="*")
        {
            GLTexture* texture = new GLTexture(src);
            auto data = texture->load(src);
            GLuint tex[4]={0,texture->texture(),0,0};
            w->setImage(tex, ng::Vector2i(texture->w(), texture->h()));
            w->setUserData("GLTexture2", texture);
        }
    }


    const Cbdler::Node& ty = p["type"];
    if(ty.count())
    {
        if(ty.value()=="radio-button") w->setFlags(ng::Button::RadioButton);
        if(ty.value()=="normal-button") w->setFlags(ng::Button::NormalButton);
        if(ty.value()=="toggle-button") w->setFlags(ng::Button::ToggleButton);
        if(ty.value()=="popup-button") w->setFlags(ng::Button::PopupButton);
    }
}

/*
    layout{
        box{orientation:vertical,alignment:middle,margin:0,spacing:0}
    }
*/
template <class T=ng::Widget>
static void _layout(const Cbdler::Node& p, T* w)
{
    static const char* lays[]={"box-layout","group-layout","grid-layout",nullptr};

    for(const char** pw = lays; *pw!=nullptr; *pw++)
    {
        const Cbdler::Node& l = p[*pw];
        ng::Alignment al;
        ng::Orientation o;
        int margin=0,spacing=0,grp_indent=20,gp_spacing=15,resolution=0,cols=0,rows=0;
        int colst=0, rowst=0;
        if(l.count() >= 4)
        {
            for (auto a: l.values())
            {
                OOO << a->key() << "\n";
                if(a->key()=="orientation")
                {
                    if(a->value(0)=="horizontal") o = ng::Orientation::Horizontal;
                    else if(a->value(0)=="vertical") o = ng::Orientation::Vertical;
                }
                else if(a->key()=="alignment")
                {
                    if(a->value(0)=="minimum") al = ng::Alignment::Minimum;
                    else if(a->value(0)=="middle") al = ng::Alignment::Middle;
                    else if(a->value(0)=="maximum") al = ng::Alignment::Maximum;
                    else if(a->value(0)=="fill") al = ng::Alignment::Fill;
                }
                else if(a->key()=="margin")
                {
                    margin = a->ivalue(0);
                }
                else if(a->key()=="spacing")
                {
                    spacing = a->ivalue(0);
                }
                else if(a->key()=="group-indent")
                {
                    grp_indent = a->ivalue(0);
                }
                else if(a->key()=="group-spacing")
                {
                    gp_spacing = a->ivalue(0);
                }
                else if(a->key()=="resolution")
                {
                    resolution = a->ivalue(0);
                }
                else if(a->key()=="rows")
                {
                    rows = a->ivalue(0);
                }
                else if(a->key()=="cols")
                {
                    cols = a->ivalue(0);
                }
                else if(a->key()=="col-stretch")
                {
                    colst = a->ivalue(0);
                }
                else if(a->key()=="row-stretch")
                {
                    rowst = a->ivalue(0);
                }
            }
            if(*pw == lays[0])  // box
            {
                w->setLayout(new ng::BoxLayout(o, al, margin, spacing));
            }
            else if(*pw == lays[1]) // grp
            {
                w->setLayout(new ng::GroupLayout(margin, spacing, gp_spacing, grp_indent));
            }
            else if(*pw == lays[2]) // grid
            {
                w->setLayout(new ng::GridLayout(o, resolution, al, margin, spacing));
            }
        }
    }
}


static ng::Widget* _screen(const Cbdler::Node& p)
{
    const Cbdler::Node& r = p["size"];
    int w = ::atoi(r.value(0).c_str());
    int h = ::atoi(r.value(1).c_str());
    const Cbdler::Node& t = p["title"];
    std::string title = t.value();
    PScreen = new ng::Screen(Eigen::Vector2i(w, h), title);
    if(PScreen)
    {
        PScreen->setVisible(true);
        return PScreen;
    }
    return nullptr;
}

static ng::Widget* _widget(const Cbdler::Node& p, ng::Widget* parent)
{
    ng::Widget* win = new ng::Widget(parent);
    if(win)
    {
        _attributes(p, win);
        _layout(p, win);
        return win;
    }
    return parent;
}

static ng::Widget* _window(const Cbdler::Node& p, ng::Widget* parent)
{
    const Cbdler::Node& t = p["title"];
    std::string title = t.value();
    ng::Window* win = new ng::Window(parent, title);
    if(win)
    {
        _attributes(p, win);
        _layout(p,win);
        return win;
    }
    return parent;
}

static ng::Widget* _button(const Cbdler::Node& p, ng::Widget* parent)
{
    const Cbdler::Node& t = p["title"];
    std::string title = t.value();
    ng::Button* win = new ng::Button(parent, title);
    if(win)
    {
        _attributes(p,win);
        _exattributes(p,win);


        const Cbdler::Node& c = p["callback"];
        if(c.count())
        {

            ssq::Function pf = PVM->findFunc(c.value().c_str());

            if(!pf.isNull())
            {
                win->setUserData("callback", &pf);
                win->setCallback2(&buttcall);
            }
        }
    }
    return parent;
}

static ng::Widget* _checkbox(const Cbdler::Node& p, ng::Widget* parent)
{
    const Cbdler::Node& t = p["title"];
    std::string title = t.value();
    ng::CheckBox* win = new ng::CheckBox(parent, title);
    if(win)
    {
        _attributes(p,win);
        //_exattributes(p,win);

        const Cbdler::Node& c = p["callback"];
        if(c.count())
        {
            ssq::Function pf = PVM->findFunc(c.value().c_str());
            if(!pf.isNull())
            {
                win->setUserData("callback", &pf);
                win->setCallback(&checkcall);
            }
        }
    }
    return parent;
}

static ng::Widget* _image(const Cbdler::Node& p, ng::Widget* parent)
{
    const Cbdler::Node& t = p["image"];
    if(t.count())
    {
        std::string src = t["src"].value();

        GLTexture* texture = new GLTexture(src);
        auto data = texture->load(src);
        ng::ImageView* win = new ng::ImageView(parent, texture->texture());
        if(win)
        {
            _attributes(p,win);
            _layout(p,win);
            win->center();
            win->setGridThreshold(20);
            win->setPixelInfoThreshold(20);
            const ng::Vector2i& textureSize = win->imageSize();
            win->setSize(textureSize);
            win->setUserData("GLTexture", texture);
        }

    }
    return parent;
}

void _recursize(Cbdler::Node* p, ng::Widget* parent)
{
    if(p->values().size())
    {
        std::cout << p->nv() << "\n";

        if(p->nv() == "screen")
        {
            parent = _screen(*p);
        }
        else if(p->nv() == "widget")
        {
            parent = _widget(*p,parent);
        }
        else if(p->nv() == "window")
        {
            parent = _window(*p,parent);
        }
        else if(p->nv() == "button")
        {
            parent = _button(*p,parent);
        }
        else if(p->nv() == "checkbox")
        {
            parent = _checkbox(*p,parent);
        }
        else if(p->nv() == "image")
        {
            parent = _image(*p,parent);
        }
        for(const auto& a : p->values())
        {
            std::cout << a->key() << "\n";
            if(a->key()=="screen" ||
                a->key()=="widget" ||
                a->key().find("layout")!=std::string::npos ||
                a->key()=="window" ||
                a->key()=="button" ||
                a->key()=="image" ||
                a->key()=="checkbox")
                _recursize(a,parent);
        }
    }
}

void build_ui(const char* ui)
{
    ng::Widget* parent = nullptr;
    if(ui && *ui)
    {
        Cbdler cdbl;
        //std::stringstream ss;
        //ss.write(ui,::strlen(ui));

        cdbl.parse(ui);
        _recursize(cdbl.root(), parent);
        PScreen->performLayout();
    }
}

void main_loop(const std::string& gui)
{
    build_ui(gui.c_str());
    ng::mainloop();
}

void use(int flag)
{
}

void _export_widgets(ssq::VM* pvm)
{
    //SQ_PTRS->sq_setconsttable(PVM->getHandle());
    ssq::Enum enm = PVM->addEnum("Orientation");
    enm.addSlot("Horizontal", (int)ng::Orientation::Horizontal);
	enm.addSlot("Vertical",  (int)ng::Orientation::Vertical);

	ssq::Enum enm1 = PVM->addEnum("Alignment");

	enm1.addSlot("Fill",  (int)ng::Alignment::Fill);
	enm1.addSlot("Maximum",  (int)ng::Alignment::Maximum);
	enm1.addSlot("Middle",  (int)ng::Alignment::Middle);
	enm1.addSlot("Minimum",  (int)ng::Alignment::Minimum);




    auto cls = pvm->addClass("BoxLayout", [](int orientation,
                                             int alignment= ng::Alignment::Middle,
                                             int margin = 0, int spacing = 0) -> ng::BoxLayout*
    {
        return new ng::BoxLayout((ng::Orientation)orientation, (ng::Alignment)alignment, margin, spacing);
    });

    auto widl = pvm->addClass("Widget", ssq::Class::Ctor<ng::Widget()>());
    widl.addFunc("setLayout",&ng::Widget::setLayout);
}

bool _init_apis(ssq::VM* pvm, sq_api* ptrs)
{
    PVM = pvm;
    SQ_PTRS = ptrs;
    pvm->addFunc("sq_nano",sq_nano);
    pvm->addFunc("main_loop",main_loop);
    pvm->addFunc("use",use);

    _export_widgets(pvm);
    ng::init();
    return false;
}

void _deinit_apis(ssq::VM* pvm, sq_api* ptrs)
{
    ng::shutdown();
}


