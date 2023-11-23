#include "Evalvisitor.h"
#include "Python3Lexer.h"
#include "Python3Parser.h"
#include "int2048.h"
#include <any>
#include <functional>
#include <string>
#include <utility>
#include"Scope.h"


 std::any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx)  {
    
    auto stmtlist=ctx->stmt();
    auto linelist=ctx->NEWLINE();
    // std::cout<<"!!!";
    for(int i=0;(i<stmtlist.size()&&stmtlist[i])||(i<linelist.size()&&linelist[i]);i++){
      if((i<stmtlist.size()&&stmtlist[i]))
        visitStmt(stmtlist[i]);
      // std::cout<<"!!!";
    }
    return {};
  }

   std::any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx)  {
    std::string name= ctx->NAME()->getText();
    function f;
    f.point=(Python3Parser::FuncdefContext *)ctx;
    
    
    if(ctx->parameters()->typedargslist()){
      auto tfplist=ctx->parameters()->typedargslist()->tfpdef();
      auto v=ctx->parameters()->typedargslist();
      auto testlist=v->test();
      for(int i=0;i!=tfplist.size();i++){
        auto name=std::any_cast<std::string>(tfplist[i]->getText());
        std::any val={};
        if(i<testlist.size()){
          val=visitTest(testlist[i]);
          // std::cout<<'|';print(val);std::cout<<'|';
        }
        f.varylist.push_back(std::make_pair(name, val));
      }
    }
    Scope::find((std::string)name)->second=f;
    return {};
  }

   std::any EvalVisitor::visitParameters(Python3Parser::ParametersContext *ctx)  {
    
    return {};
  }

   std::any EvalVisitor::visitTypedargslist(Python3Parser::TypedargslistContext *ctx)  {
    return {};
  }

   std::any EvalVisitor::visitTfpdef(Python3Parser::TfpdefContext *ctx)  {
    return ctx->getText();
  }

   std::any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx)  {
    if(ctx->simple_stmt())
      return visitSimple_stmt(ctx->simple_stmt());
    else if(ctx->compound_stmt()){
      std::any a=visitCompound_stmt(ctx->compound_stmt());
      // exit(0);
      return a;
  
    }
    return {};
  }

   std::any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx)  {
    return visitSmall_stmt(ctx->small_stmt());
  }

   std::any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx)  {
    if(ctx->expr_stmt())return visitExpr_stmt(ctx->expr_stmt());
    else return visitFlow_stmt(ctx->flow_stmt());
  }
   std::any EvalVisitor::visitAugassign(Python3Parser::AugassignContext *ctx)  {
    if(ctx->ADD_ASSIGN()){
      return (int)ADD;
    }else if(ctx->SUB_ASSIGN()){
      return (int)SUB;
    }else if(ctx->MULT_ASSIGN()){
      return (int)MUL;
    }else if(ctx->DIV_ASSIGN()){
      return (int)DIVdouble;
    }else if(ctx->IDIV_ASSIGN()){
      return (int)DIV;
    }else if(ctx->MOD_ASSIGN()){
      return (int)MOD;
    }else return (int)NO;
  }

   std::any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx)  {
    // if(!ctx->augassign())return visitTestlist(ctx->testlist(0));
    if(!ctx->ASSIGN(0)&&!ctx->augassign())return visitTestlist(ctx->testlist(0));
    auto testlist=ctx->testlist();
    std::any result=visitTestlist(testlist.back());
    // std::cout<<"\n|";MYprint(result);std::cout<<"|\n";
    std::vector<std::any>val=std::any_cast<std::vector<std::any>>(result);
    // std::cout<<val.size();
    for(int j=0;j<val.size();j++)
      val[j]=Getvalue(val[j]);
    std::any Assign=(int)EQAL;
    if(ctx->augassign())Assign=visitAugassign(ctx->augassign());
    int *pp=std::any_cast<int>(&Assign);
    int p=*pp;
    if(p!=EQAL){
      result=visitTestlist(testlist.front());
      std::vector<std::any>varylist=std::any_cast<std::vector<std::any>>(result);
      for(int j=0;j<varylist.size()&&j<val.size();j++){
        if(p==ADD){
          Changevalue(varylist[j], Getvalue(varylist[j])+Getvalue(val[j]));
        }
        if(p==SUB){
          Changevalue(varylist[j], Getvalue(varylist[j])-Getvalue(val[j]));
        }
        if(p==MUL){
          Changevalue(varylist[j], Getvalue(varylist[j])*Getvalue(val[j]));
        }
        if(p==DIV){
          Changevalue(varylist[j], Getvalue(varylist[j])/Getvalue(val[j]));
        }
        if(p==DIVdouble){
          Changevalue(varylist[j], Getdivdoubleright(Getvalue(varylist[j]),Getvalue(val[j]) ));
        }
        if(p==MOD){
          Changevalue(varylist[j], Getvalue(varylist[j])%Getvalue(val[j]));
        }
      }
      return {};
    }
    for(int i=0;i+1<testlist.size();i++){
      result=visitTestlist(testlist[i]);
      std::vector<std::any>varylist=std::any_cast<std::vector<std::any>>(result);
      // std::cout<<varylist.size()<<' '<<val.size()<<'\n';
      for(int j=0;j<varylist.size()&&j<val.size();j++){
        Changevalue(varylist[j], Getvalue(val[j]));
      }
    }
    return {};
  }

  
   std::any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx)  {
    if(ctx->continue_stmt()){
      return Flowcontrol(Flowcontrol::controltype::continue_,{});
    }else if(ctx->break_stmt()){
      return Flowcontrol(Flowcontrol::controltype::break_,{});
    }else if(ctx->return_stmt()){
      auto pot=ctx->return_stmt();
      auto re=pot->testlist();
      if(re==nullptr)
        return Flowcontrol(Flowcontrol::controltype::return_,{});
      auto ans=std::any_cast<std::vector<std::any>>(visitTestlist(re));

      return Flowcontrol(Flowcontrol::controltype::return_,Getvalue(ans));
    }
    return {};
  }

   std::any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx)  {
    return Flowcontrol(Flowcontrol::controltype::break_,{});
  }

   std::any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx)  {
    return Flowcontrol(Flowcontrol::controltype::continue_,{});
  }

   std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx)  {
    std::any result=visitTestlist(ctx->testlist());
    auto res=std::any_cast<std::vector<std::any>>(result);
    for(auto u:res){
      u=Getvalue(u);
    }
    if(res.size()==1)
      return Flowcontrol(Flowcontrol::controltype::continue_,res[0]);
    else 
      return Flowcontrol(Flowcontrol::controltype::continue_,res);
  }

   std::any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx)  {
    if(ctx->if_stmt())
      return visitIf_stmt(ctx->if_stmt());
    else if(ctx->while_stmt()){
      std::any re=visitWhile_stmt(ctx->while_stmt());
      
      return re;
    }
    else if(ctx->funcdef())
      return visitFuncdef(ctx->funcdef());
    return {};
  }
   std::any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx)  {
    auto suitelist=ctx->suite();
    auto testlist=ctx->test();
    
    for(int i=0;i!=suitelist.size();i++){
      if(i>=testlist.size()){
        std::any result=visitSuite(suitelist[i]);
        auto u=std::any_cast<Flowcontrol>(&result);
        if(u)
          return result;
        break;
      }
      if(Getbool(Getvalue(visitTest(testlist[i])))){
        std::any result=visitSuite(suitelist[i]);
        auto u=std::any_cast<Flowcontrol>(&result);
        if(u){
          return result;
        }
        break;
      }
    }
    return {};
  }

   std::any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx)  {
    while(Getbool(visitTest(ctx->test()))){
      std::any result=visitSuite(ctx->suite());

      auto u=std::any_cast<Flowcontrol>(&result);
      if(u){
        
        if(u->controtype==Flowcontrol::controltype::break_){
          // std::cout<<"{}";

          return {};
        }
        if(u->controtype==Flowcontrol::controltype::return_)
          return result;
      }
    }
    return {};
  }

   std::any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx)  {
    if (ctx->simple_stmt()) {
      std::any result = visitSimple_stmt(ctx->simple_stmt());
      auto u=std::any_cast<Flowcontrol>(&result);
      if (u) 
        return result;
    }
    auto stmtlist=ctx->stmt();
    for (int i = 0; i!=stmtlist.size(); ++i) {
      std::any result = visitStmt(stmtlist[i]);
      auto u=std::any_cast<Flowcontrol>(&result);
      if (u) {
        // std::cout<<i<<':';
        // print(u->result);
        return *u;
      }
    }
    return {};
  }

   std::any EvalVisitor::visitTest(Python3Parser::TestContext *ctx)  {
    std::any t=visitOr_test(ctx->or_test());
    
    return t;
  }

   std::any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx)  {
    std::any result=visitAnd_test(ctx->and_test(0));
    auto testlist=ctx->and_test();
    auto oplist=ctx->OR();
    if(!oplist.size())return result;
    // std::cout<<"@@";
    result=Getbool(result);
    if(Getbool(result))
      return true;
    for(int i=0;i!=oplist.size();i++){
      std::any res=visitAnd_test(testlist[i+1]);
      // std::cout<<Getbool(result)<<' '<<Getbool(res)<<'\n';
      result=(Getbool(result)||Getbool(res));
      if(Getbool(result))
        return true;
    }
    return result;
  }

   std::any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx)  {
    std::any result=visitNot_test(ctx->not_test(0));
    // if(std::any_cast<variety>(&result))std::cout<<"97897";
    auto testlist=ctx->not_test();
    auto oplist=ctx->AND();
    if(!oplist.size())return result;
    result=Getbool(result);
    if(!Getbool(result))
      return false;
    for(int i=0;i!=oplist.size();i++){
      std::any res=visitNot_test(testlist[i+1]);
      result=(Getbool(result)&&Getbool(res));
      if(!Getbool(result))
        return false;
    }
    return result;
  }

   std::any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx)  {
    if(ctx->NOT()){
      return !Getbool(visitNot_test(ctx->not_test()));
    }
    return visitComparison(ctx->comparison());
  }

   std::any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx)  {
    std::any result=visitArith_expr(ctx->arith_expr(0));
    auto arilist=ctx->arith_expr();
    auto oplist=ctx->comp_op();
    if(!oplist.size())return result;
    result=Getvalue(result);
    bool st=true;
    for(int i=0;i!=oplist.size();i++){
      std::any res=visitArith_expr(arilist[i+1]);
      res=Getvalue(res);
      if(oplist[i]->EQUALS())
        st&=(result==res);
      if(oplist[i]->LESS_THAN())
        st&=(result<res);
      if(oplist[i]->GREATER_THAN())
        st&=(result>res);
      if(oplist[i]->LT_EQ())
        st&=(result<=res);
      if(oplist[i]->GT_EQ())
        st&=(result>=res);
      if(oplist[i]->NOT_EQ_2()){
        // print(result);
        // print(res);
        st&=(result!=res);
      }
      if(!st)
        return st; 
      result=res;
    }
    
    return st;
  }

   std::any EvalVisitor::visitComp_op(Python3Parser::Comp_opContext *ctx)  {
    return {};
  }
//-------------------------------------------------------

   std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx)  {
    std::any result=visitTerm(ctx->term(0));
    auto termlist=ctx->term();
    auto oplist=ctx->addorsub_op();
    if(!oplist.size())return result;
    result=Getvalue(result);
    // print(result);
    for(int i=0;i!=oplist.size();i++){
      std::any res=Getvalue(visitTerm(termlist[i+1]));
      // print(res);
      if(oplist[i]->ADD())
        result+=res;
      if(oplist[i]->MINUS())
        result-=res;
    }
    return result;
  }

   std::any EvalVisitor::visitAddorsub_op(Python3Parser::Addorsub_opContext *ctx)  {
    return {};
  }

   std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx)  {
    std::any result=visitFactor(ctx->factor(0));
    auto factorlist=ctx->factor();
    auto oplist=ctx->muldivmod_op();
    if(!oplist.size())return result;
    result=Getvalue(result);
    for(int i=0;i!=oplist.size();i++){
      std::any res=Getvalue(visitFactor(factorlist[i+1]));
      if(oplist[i]->STAR())
        result*=res;
      if(oplist[i]->DIV())
        result=Getdivdoubleright(result,res);
      if(oplist[i]->IDIV())
        result/=res;
      if(oplist[i]->MOD())
        result%=res;
    }
    return result;
  }

   std::any EvalVisitor::visitMuldivmod_op(Python3Parser::Muldivmod_opContext *ctx)  {
    return {};
  }

   std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx)  {
    
    if(ctx->atom_expr()){
      std::any t=visitAtom_expr(ctx->atom_expr());
      return t;
    }
    if(ctx->MINUS()){
      return addminussignal(Getvalue(visitFactor(ctx->factor())));
    }else{
      return visitFactor(ctx->factor());
    }
  }

   std::any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx)  {
    // std::cout<<'(';
    if(ctx->trailer()){
      std::any tmp=visitAtom(ctx->atom());
      auto u=std::any_cast<variety>(&tmp);
      
      variety v=std::any_cast<variety>(tmp);
      function* f=std::any_cast<function>(&v.second);
      std::vector<std::any> V;V.push_back((*f).point);
      // if(std::any_cast<>(&V[0]))
      // print((*f).point);
      // return {};
      
      return usefunction(*f,ctx->trailer()->arglist());
    }else{
    //   std::cout<<')';
      // return {};
      return visitAtom(ctx->atom());
    }
  }

   std::any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx)  {
    return ctx->arglist();
  }

   std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx)  {
    if (ctx->NONE())
      return {};
    else if (ctx->TRUE())
      return true;
    else if (ctx->FALSE())
      return false;
    else if (ctx->test()){
      return visitTest(ctx->test());
    }
      
    else if (ctx->NUMBER()){
      for(auto s:ctx->getText())
      {
        if(s=='.')
          return Getdouble(ctx->getText());
      }
      return Getint(ctx->getText());
    }
    else if (!ctx->STRING().empty()){
      std::string tmp=""; 
      auto S=ctx->STRING();
      for(auto s:S){
        auto textlist=s->getText();
        for(int i=1;i+1<textlist.size();i++){
          tmp+=textlist[i];
        }
      }
      return tmp;
    }
    else if(ctx->NAME()){
      // std::any a=(*Scope::find(ctx->getText()));
      // if(!td::any_cast<>(&a))std::cout<<"98493574";
      // auto u=std::any_cast<function>(&a);
      // if(u)std::cout<<3248947;
      // std::cout<<ctx->getText()<<'\n';
      variety a=*Scope::find((std::string)ctx->getText());
      // /*if(std::any_cast<variety>(&t))*/std::cout<<"97897";
      return a;
    }
    return {};
  }
   std::any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx)  {
    std::vector<std::any> list;
    std::vector<std::any>::iterator it;
    auto testlist=ctx->test();
    for(int i=0;i!=testlist.size();i++){
      std::any tmp=visitTest(testlist[i]);
      auto p=std::any_cast<std::vector<std::any>>(&tmp);
      if(p){
        
        std::vector<std::any> v=*p;
        for(it=v.begin();it!=v.end();it++){
          list.push_back(*it);
        }
      }else{
        list.push_back(tmp);
      }
    }
    return list;
  }

   std::any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx)  {
    
    // std::cout<<"*";
    // exit(0);
    // return {};
    std::vector<std::any>list;
    if(!ctx)return list;
    auto arglist=ctx->argument();
    for(int i=0;i!=arglist.size();i++){
      std::any t=visitArgument(arglist[i]);
      if(std::any_cast<std::pair<std::string,std::any>>(&t)){
        if(std::any_cast<std::pair<std::string,std::any>>(t).first=="!")
          t=std::any_cast<std::pair<std::string,std::any>>(t).second;
      }
      list.push_back(t);
    }
    // auto p=std::any_cast<variety>(&list[0]);
    // if(p)std::cout<<"3u4984";
    // std::cout<<list.size();
    return list;
  }

   std::any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx)  {
    if(ctx->test(1)){
      std::any result=visitTest(ctx->test(1));
      return std::make_pair(ctx->test(0)->getText(),Getvalue(result));
    }else{
      //bug!
      //getvalue 出一个variety
      std::any t=visitTest(ctx->test(0));
      if(std::any_cast<variety>(&t))
        t=std::make_pair((std::string)"!", t);
      // std::cout<<"}}";
      // if(std::any_cast<variety>(&t))std::cout<<std::any_cast<variety>(t).first<<' ';

      return t;
    }
  }