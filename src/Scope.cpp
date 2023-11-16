#include"Scope.h"
// #include "Python3ParserVisitor.h"
#include "Evalvisitor.h"
#include "int2048.h"

using namespace Scope;
namespace Scope {
std::vector<std::map<std::string ,std::any>>varietymap
    {{{"int",function(0)},{"string",function(1)},
    {"double",function(2)},{"bool",function(3)},{"print",function(4)}}};


std::map<std::string ,std::any>::iterator find(std::any object)
    {
        auto p=std::any_cast<std::string>(&object);
        auto pot=varietymap.back().find(*p);
        if(pot!=varietymap.back().end()){
            return pot;
        }
        pot=varietymap.front().find(*p);
        if(pot!=varietymap.front().end()){
            return pot;
        }
        varietymap.back().insert({*p,{}});
        return varietymap.back().find(*p);
    }
    
}    


std::any MYprint(std::any list)
{
    std::vector<std::any> v;
    v=std::any_cast<std::vector<std::any>>(list);
    for(auto u:v){
        print(u);
    }
    return {};
}
std::any usefunction(function f,Python3Parser::ArglistContext *para)
{
    // if(!para){std::cout<<"478973489";}
    
    //0 int 1 string 2 double 3 bool 4 print
    int2048 *p=std::any_cast<int2048>(&f.point);
    EvalVisitor vis;




/*

{{{
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}}}*/













    auto F=std::any_cast<Python3Parser::FuncdefContext>(&f.point);
    if(p){
        if(*p==int2048(0)){
            return Getint(vis.visit(para));
        }
        if(*p==int2048(1)){
            return Getstring(vis.visit(para));
        }
        if(*p==int2048(2)){
            return Getdouble(vis.visit(para));
        }
        if(*p==int2048(3)){
            return Getbool(vis.visit(para));
        }
        if(*p==int2048(4)){
            // std::cout<<4;return {};
            return MYprint(vis.visit(para));
        }
        
        return {};
        
    }else{
        Python3Parser::FuncdefContext *p=std::any_cast<Python3Parser::FuncdefContext> (&f.point);
        std::map<std::string, std::any> newvarietymap;
        if(para){
            for(int i=0;para->argument(i);i++){
                std::any result=vis.visit(para->argument(i));
                std::pair<std::string,std::any>*p=
                    std::any_cast<std::pair<std::string,std::any>>(&result);
                if(p){
                    newvarietymap.insert(*p);
                }else{
                    std::any tmp=
                    //----------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    //!
                    //!

                    F->parameters()->typedargslist()->tfpdef(i)->getText();
                    auto ptmp=std::any_cast<std::string>(&tmp);
                    newvarietymap[*ptmp]=result;
                }
                
            }
            for(auto u:f.varylist){
                if(newvarietymap.find(u.first)==newvarietymap.end()){
                    newvarietymap.insert(u);
                }
            }
        }
        Scope::varietymap.push_back(newvarietymap);
        std::any result=vis.visit(F->suite());
        Scope::varietymap.pop_back();
        auto pres=std::any_cast<Flowcontrol>(&result);
        if(!pres)return {};
        if(pres->controtype==Flowcontrol::controltype::return_){
            std::any res=pres->result;
            //add
            return res;
        }else return{};

    }
}

void Changevalue(std::any &L,std::any R)
{
    variety* pl=std::any_cast<variety>(&L);
    variety* pr=std::any_cast<variety>(&R);
    
    if(!pl)return;
    std::any val=R;
    if(pr)val=(*pr).second;
    std::any t=(*pl).first;
    (*Scope::find(t)).second=val;
    (*pl).second=val;
}
std::any Getvalue(std::any object)
{
    auto plist=std::any_cast<std::vector<std::any>>(&object);
    auto pvariety=std::any_cast<variety>(&object);
    if(pvariety){
        return *(Scope::find(pvariety->first));
    }else if(plist){
        std::vector<std::any> v;
        std::vector<std::any>::iterator it;
        for(it=plist->begin();it!=plist->end();it++){
            v.push_back(Getvalue(*it));
        }
        if(v.size()==1)
            return v[0];
        return v;
        //list add
    }else return object;
}