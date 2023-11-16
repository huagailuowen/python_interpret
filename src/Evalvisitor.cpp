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
    for(int i=0;ctx->stmt(i)||ctx->NEWLINE(i);i++){
      if(ctx->stmt(i))
        visitStmt(ctx->stmt(i));
    }
    return {};
  }

   std::any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx)  {
    std::string name= ctx->NAME()->getText();
    function f;
    f.point=ctx;
    // ctx->parameters()
    for(int i=0;ctx->parameters()->typedargslist()->tfpdef(i);i++){
      auto v=ctx->parameters()->typedargslist();
      auto name=std::any_cast<std::string>(v->tfpdef(i)->getText());
      std::any val={};
      if(v->test(i)){
        val=visitTest(v->test(i));
      }
      f.varylist.push_back(std::make_pair(name, val));
    }
    Scope::find(name)->second=f;
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
    else
      return visitCompound_stmt(ctx->compound_stmt());
  }

   std::any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx)  {
    return visitSmall_stmt(ctx->small_stmt());
  }

   std::any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx)  {
    return visitChildren(ctx);
  }
   std::any EvalVisitor::visitAugassign(Python3Parser::AugassignContext *ctx)  {
    if(ctx->ADD_ASSIGN()){
      return ADD;
    }else if(ctx->SUB_ASSIGN()){
      return SUB;
    }else if(ctx->MULT_ASSIGN()){
      return MUL;
    }else if(ctx->DIV_ASSIGN()){
      return DIV;
    }else if(ctx->IDIV_ASSIGN()){
      return DIVdouble;
    }else if(ctx->MOD_ASSIGN()){
      return MOD;
    }else return NO;
  }

   std::any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx)  {
    // if(!ctx->augassign())return visitTestlist(ctx->testlist(0));
    if(!ctx->ASSIGN(0))return visitTestlist(ctx->testlist(0));
    std::any result=visitTestlist(ctx->testlist().back());
    std::vector<std::any>val=std::any_cast<std::vector<std::any>>(result);
    int p=std::any_cast<int>(visitAugassign(ctx->augassign()));
    if(p!=EQAL){
      result=visitTestlist(ctx->testlist().front());
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
    for(int i=0;ctx->testlist(i+1);i++){
      result=visitTestlist(ctx->testlist(i));
      std::vector<std::any>varylist=std::any_cast<std::vector<std::any>>(result);
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
      std::any ans=visitTestlist(re);
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
    else if(ctx->while_stmt())
      return visitWhile_stmt(ctx->while_stmt());
    else if(ctx->funcdef())
      return visitFuncdef(ctx->funcdef());
    return {};
  }
   std::any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx)  {
    for(int i=0;ctx->suite(i);i++){
      if(!ctx->test(i)){
        std::any result=visitSuite(ctx->suite(i));
        auto u=std::any_cast<Flowcontrol>(&result);
        if(u)
          return u;
        break;
      }
      if(Getbool(Getvalue(visitTest(ctx->test(i))))){
        std::any result=visitSuite(ctx->suite(i));
        auto u=std::any_cast<Flowcontrol>(&result);
        if(u)
          return u;
        break;
      }
    }
    return {};
  }

   std::any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx)  {
    while(ctx->test()){
      std::any result=visitSuite(ctx->suite());
      auto u=std::any_cast<Flowcontrol>(&result);
      if(u){
        if(u->controtype==Flowcontrol::controltype::break_)
          return {};
        if(u->controtype==Flowcontrol::controltype::return_)
          return u;
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
    for (int i = 0; ctx->stmt(i); ++i) {
      std::any result = visitStmt(ctx->stmt(i));
      auto u=std::any_cast<Flowcontrol>(&result);
      if (u) 
        return result;
    }
    return {};
  }

   std::any EvalVisitor::visitTest(Python3Parser::TestContext *ctx)  {
    return visitOr_test(ctx->or_test());
  }

   std::any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx)  {
    std::any result=visitAnd_test(ctx->and_test(0));
    if(!ctx->OR(0))return result;
    result=Getbool(result);
    for(int i=0;ctx->OR(i);i++){
      std::any res=visitAnd_test(ctx->and_test(i+1));
      result=(Getbool(result)|Getbool(res));
      if(Getbool(result))
        return true;
    }
    return result;
  }

   std::any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx)  {
    std::any result=visitNot_test(ctx->not_test(0));
    if(!ctx->AND(0))return result;
    result=Getbool(result);
    for(int i=0;ctx->AND(i);i++){
      std::any res=visitNot_test(ctx->not_test(0));
      result=(Getbool(result)&Getbool(res));
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
    if(!ctx->comp_op(0))return result;
    bool st=true;
    for(int i=0;ctx->comp_op(i);i++){
      std::any res=visitArith_expr(ctx->arith_expr(i+1));
      if(ctx->comp_op(i)->EQUALS())
        st&=(result==res);
      if(ctx->comp_op(i)->LESS_THAN())
        st&=(result<res);
      if(ctx->comp_op(i)->GREATER_THAN())
        st&=(result>res);
      if(ctx->comp_op(i)->LT_EQ())
        st&=(result<=res);
      if(ctx->comp_op(i)->GT_EQ())
        st&=(result>=res);
      if(ctx->comp_op(i)->NOT_EQ_2())
        st&=(result!=res);
      if(!st)
        return st; 
      result=res;
      //指针？？？？？？
    }
    return st;
  }

   std::any EvalVisitor::visitComp_op(Python3Parser::Comp_opContext *ctx)  {
    return {};
  }
//-------------------------------------------------------

   std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx)  {
    std::any result=visitTerm(ctx->term(0));
    for(int i=0;ctx->addorsub_op(i);i++){
      std::any res=visitTerm(ctx->term(i+1));
      if(ctx->addorsub_op(i)->ADD())
        result+=res;
      if(ctx->addorsub_op(i)->MINUS())
        result-=res;
    }
    return result;
  }

   std::any EvalVisitor::visitAddorsub_op(Python3Parser::Addorsub_opContext *ctx)  {
    return {};
  }

   std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx)  {
    std::any result=visitFactor(ctx->factor(0));
    for(int i=0;ctx->muldivmod_op(i);i++){
      std::any res=visitFactor(ctx->factor(i+1));
      if(ctx->muldivmod_op(i)->STAR())
        result*=res;
      if(ctx->muldivmod_op(i)->DIV())
        result=Getdivdoubleright(result,res);
      if(ctx->muldivmod_op(i)->IDIV())
        result/=res;
      if(ctx->muldivmod_op(i)->MOD())
        result%=res;
    }
    return result;
  }

   std::any EvalVisitor::visitMuldivmod_op(Python3Parser::Muldivmod_opContext *ctx)  {
    return {};
  }

   std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx)  {
    if(ctx->atom_expr()){
      return visitAtom_expr(ctx->atom_expr());
    }
    if(ctx->MINUS()){
      return addminussignal(Getvalue(visitFactor(ctx->factor())));
    }else{
      return Getvalue(visitFactor(ctx->factor()));
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
      
    else if (ctx->NUMBER())
      return Getint(ctx->getText());
    else if (!ctx->STRING().empty()){
      std::string tmp=""; 
      for(auto s:ctx->STRING()){

        for(int i=1;i+1<s->getText().size();i++){
          tmp+=s->getText()[i];
        }
      }
      return tmp;
    }
    else if(ctx->NAME()){
      // std::any a=(*Scope::find(ctx->getText()));
      // if(!td::any_cast<>(&a))std::cout<<"98493574";
      // auto u=std::any_cast<function>(&a);
      // if(u)std::cout<<3248947;
      variety a=*Scope::find(ctx->getText());
      return a;
    }
    return {};
  }
   std::any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx)  {
    std::vector<std::any> list;
    std::vector<std::any>::iterator it;
    for(int i=0;ctx->test(i);i++){
      std::any tmp=visitTest(ctx->test(i));
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
    // std::cout<<"743987348975";
    // exit(0);
    // return {};
    std::vector<std::any>list;
    for(int i=0;ctx->argument(i);i++){
      list.push_back(visitArgument(ctx->argument(i)));
    }
    return list;
  }

   std::any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx)  {
    if(ctx->test(1)){
      std::any result=visitTest(ctx->test(1));
      return std::make_pair(ctx->test(0)->getText(),Getvalue(result));
    }else{
      return Getvalue(visitTest(ctx->test(0)));
    }
  }