#ifndef SCORER_SCORER_H_
#define SCORER_SCORER_H_

namespace kws {
namespace scorer {

template <class RE, class HE>
class Scorer {
 public:
  typedef RE RefEvent;
  typedef HE HypEvent;
  struct Result {
    float tp, fp, fn;

    Result() : tp(0.0f), fp(0.0f), fn(0.0f) {}

    Result(float tp, float fp, float fn) : tp(tp), fp(fp), fn(fn) {}

    bool operator==(const Result& other) const {
      return tp == other.tp && fp == other.fp && fn == other.fn;
    }

    bool operator!=(const Result& other) const {
      return tp != other.tp || fp != other.fp || fn != other.fn;
    }
  };

  Scorer() {}

  virtual ~Scorer() {}

  virtual Result operator()(const RefEvent& ref, const HypEvent& hyp) = 0;
};

template <class RE, class HE>
std::ostream& operator<<(std::ostream& os, const typename Scorer<RE, HE>::Result& r) {
  os << "tp=" << r.tp << " fp=" << r.fp << " fn=" << r.fn;
  return os;
}

}  // namespace scorer
}  // namespace kws

#endif  // SCORER_SCORER_H_
