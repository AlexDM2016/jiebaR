#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include "MixSegment.hpp"
#include "MPSegment.hpp"
#include "HMMSegment.hpp"
#include "QuerySegment.hpp"
#include "PosTagger.hpp"
#include "Simhasher.hpp"

#include <Rcpp.h>
using namespace Rcpp;
using namespace CppJieba;

string itos(double i)  // convert int to string
{
  stringstream s;
  s << i;
  return s.str();
}

string int64tos(uint64_t i)  // convert int to string
{
  stringstream s;
  s << i;
  return s.str();
}
//////////segment
class mpseg
{
  public:
  const char *const dict_path;
  const char *const user_path;
  MPSegment mpsegment;
  mpseg(CharacterVector dict, CharacterVector user) :
  dict_path(dict[0]), user_path(user[0]), mpsegment(dict_path, user_path)
  {
  }
  ~mpseg() {};
  
  CharacterVector cut(CharacterVector x)
  {
    const char *const test_lines = x[0];
    vector<string> words;
    mpsegment.cut(test_lines, words);
    return wrap(words);
  }
};


RCPP_MODULE(mod_mpseg)
{
  class_<mpseg>( "mpseg")
  .constructor<CharacterVector, CharacterVector>()
  .method( "cut", &mpseg::cut)
  ;
}

class mixseg
{
  public:
  const char *const dict_path;
  const char *const model_path;
  const char *const user_path;
  MixSegment mixsegment;
  mixseg(CharacterVector dict, CharacterVector model, CharacterVector user) :
  dict_path(dict[0]), model_path(model[0]), user_path(user[0]), mixsegment(dict_path, model_path, user_path)
  {
  }
  ~mixseg() {};
  
  CharacterVector cut(CharacterVector x)
  {
    const char *const test_lines = x[0];
    vector<string> words;
    mixsegment.cut(test_lines, words);
    return wrap(words);
  }
};

RCPP_MODULE(mod_mixseg)
{
  class_<mixseg>( "mixseg")
  .constructor<CharacterVector, CharacterVector, CharacterVector>()
  .method( "cut", &mixseg::cut)
  ;
}

class queryseg
{
  public:
  const char *const dict_path;
  const char *const model_path;
  QuerySegment querysegment;
  queryseg(CharacterVector dict, CharacterVector model, int n) :
  dict_path(dict[0]), model_path(model[0]), querysegment(dict_path, model_path, n)
  {
  }
  ~queryseg() {};
  
  CharacterVector cut(CharacterVector x)
  {
    const char *const test_lines = x[0];
    vector<string> words;
    querysegment.cut(test_lines, words);
    return wrap(words);
  }
};


RCPP_MODULE(mod_query)
{
  class_<queryseg>( "queryseg")
  .constructor<CharacterVector, CharacterVector, int>()
  .method( "cut", &queryseg::cut)
  ;
}

class hmmseg
{
  public:
  const char *const model_path;
  HMMSegment hmmsegment;
  hmmseg(CharacterVector model) :
  model_path(model[0]), hmmsegment(model_path)
  {
  }
  ~hmmseg() {};
  
  CharacterVector cut(CharacterVector x)
  {
    const char *const test_lines = x[0];
    vector<string> words;
    hmmsegment.cut(test_lines, words);
    return wrap(words);
  }
};

RCPP_MODULE(mod_hmmseg)
{
  class_<hmmseg>( "hmmseg")
  .constructor<CharacterVector>()
  .method( "cut", &hmmseg::cut)
  ;
}

//////////keyword
class tagger
{
  public:
  const char *const dict_path;
  const char *const model_path;
  const char *const user_path;
  PosTagger  taggerseg;
  tagger(CharacterVector dict, CharacterVector model, CharacterVector user) :
  dict_path(dict[0]), model_path(model[0]), user_path(user[0]), taggerseg(dict_path, model_path, user_path)
  {
  }
  ~tagger() {};
  
  CharacterVector tag(CharacterVector x)
  {
    const char *const test_lines = x[0];
    vector<pair<string, string> > res;
    taggerseg.tag(test_lines, res);
    unsigned int it;
    CharacterVector m(res.size());
    CharacterVector atb(res.size());
    for (it = 0; it != res.size(); it++)
    {
      m[it] = res[it].first;
      atb[it] = res[it].second;
    }
    m.attr("names") = atb;
    return wrap(m);
  }
  
  CharacterVector file(CharacterVector x)
  {
    const char *const test_lines = x[0];
    vector<pair<string, string> > res;
    taggerseg.tag(test_lines, res);
    unsigned int it;
    vector<string> m;
    m.reserve(res.size()*2);
    for (it = 0; it != m.size(); it=it+2)
    {
      m.push_back(res[it/2].first);
      m.push_back(res[it/2].second);
    }
    
    return wrap(m);
  }
  
};

RCPP_MODULE(mod_tag)
{
  class_<tagger>( "tagger")
  .constructor<CharacterVector, CharacterVector, CharacterVector>()
  .method( "tag", &tagger::tag)
  .method( "tag", &tagger::file)
  ;
}

class keyword
{
  public:
  size_t topN;
  const char *const dict_path;
  const char *const model_path;
  const char *const stop_path;
  const char *const idf_path;


  
  KeywordExtractor extractor;
  keyword(unsigned int n, CharacterVector dict, CharacterVector model, CharacterVector idf, CharacterVector stop) :
  topN(n), dict_path(dict[0]), model_path(model[0]), stop_path(stop[0]),
  idf_path(idf[0]), extractor(dict_path, model_path, idf_path, stop_path)
  {
  }
  ~keyword() {};
  
  CharacterVector tag(CharacterVector x)
  {
    const char *const test_lines = x[0];
    vector<pair<string, double> > res;
    extractor.extract(test_lines, res, topN);
    unsigned int it;
    CharacterVector m(res.size());
    CharacterVector atb(res.size());
    for (it = 0; it != res.size(); it++)
    {
      m[it] = res[it].first;
      atb[it] = itos(res[it].second);
    }
    m.attr("names") = atb;
    return wrap(m);
  }
  
  CharacterVector cut(CharacterVector x)
  {
    const char *const test_lines = x[0];
    vector<string> words;
    extractor.extract(test_lines, words, topN);
    return wrap(words);
  }
};

RCPP_MODULE(mod_key)
{
  class_<keyword>( "keyword")
  .constructor<unsigned int, CharacterVector, CharacterVector, CharacterVector, CharacterVector>()
  .method( "tag", &keyword::tag)
  .method( "cut", &keyword::cut)
  ;
}

//////////simhash

class sim
{
  public:
  const char *const dict_path;
  const char *const model_path;
  const char *const stop_path;
  const char *const idf_path;

  Simhash::Simhasher hash;
  sim(CharacterVector dict, CharacterVector model, CharacterVector idf, CharacterVector stop) : dict_path(dict[0]), model_path(model[0]), stop_path(stop[0]),
  idf_path(idf[0]), hash(dict_path, model_path, idf_path, stop_path) {}
  ~sim() {};
  
  List simhash(CharacterVector code, int topn)
  {
    const char *const code_path = code[0];
    vector<pair<string, double> > lhsword;
    uint64_t hashres;
    hash.make(code_path, topn, hashres, lhsword);
    CharacterVector lhsm(lhsword.size());
    CharacterVector lhsatb(lhsword.size());
    unsigned int it;
    for (it = 0; it != lhsword.size(); it++)
    {
      lhsm[it] = lhsword[it].first;
      lhsatb[it] = itos(lhsword[it].second);
    }
    lhsm.attr("names") = lhsatb;
    CharacterVector hashvec;
    hashvec.push_back(int64tos(hashres));
    return List::create( Named("simhash") = hashvec,
    Named("keyword") = lhsm);
  }
  
  List distance(CharacterVector lhs, CharacterVector rhs, int topn)
  {
    uint64_t lhsres;
    uint64_t rhsres;
    vector<pair<string, double> > lhsword;
    vector<pair<string, double> > rhsword;
    const char *const lhs_path = lhs[0];
    const char *const rhs_path = rhs[0];
    hash.make(lhs_path, topn, lhsres, lhsword);
    hash.make(rhs_path, topn, rhsres, rhsword);
    CharacterVector lhsm(lhsword.size());
    CharacterVector lhsatb(lhsword.size());
    unsigned int it;
    for (it = 0; it != lhsword.size(); it++)
    {
      lhsm[it] = lhsword[it].first;
      lhsatb[it] = itos(lhsword[it].second);
    }
    lhsm.attr("names") = lhsatb;
    
    CharacterVector rhsm(rhsword.size());
    CharacterVector rhsatb(rhsword.size());
    for (it = 0; it != rhsword.size(); it++)
    {
      rhsm[it] = rhsword[it].first;
      rhsatb[it] = itos(rhsword[it].second);
    }
    rhsm.attr("names") = rhsatb;
    CharacterVector hashvec;
    hashvec.push_back(int64tos(hash.distances(lhsres, rhsres)));
    return List::create( Named("distance") = hashvec,
    Named("lhs") = lhsm,
    Named("rhs") = rhsm
    );
    
  }
  
  
};


RCPP_MODULE(mod_sim)
{
  class_<sim>( "sim")
  .constructor<CharacterVector, CharacterVector, CharacterVector, CharacterVector>()
  .method( "simhash", &sim::simhash)
  .method( "distance", &sim::distance)
  ;
}