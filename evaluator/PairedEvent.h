#ifndef EVALUATOR_PAIRED_EVENT_H_
#define EVALUATOR_PAIRED_EVENT_H_

#include <memory>

namespace kws {
namespace evaluator {

template <class RE, class HE>
class PairedEvent {
 public:
  typedef RE RefEvent;
  typedef HE HypEvent;

  PairedEvent(float tp, float fp, float fn, const RefEvent& ref,
              const HypEvent& hyp) :
      tp_(tp), fp_(fp), fn_(fn) {
    if (ref != nullptr) ref_.reset(new RefEvent(ref));
    if (hyp != nullptr) hyp_.reset(new HypEvent(hyp));
  }

  PairedEvent(float tp, float fp, float fn, const RefEvent* ref,
              const HypEvent* hyp) :
      tp_(tp), fp_(fp), fn_(fn) {
    if (ref != nullptr) ref_.reset(new RefEvent(*ref));
    if (hyp != nullptr) hyp_.reset(new HypEvent(*hyp));
  }

  inline float Tp() const { return tp_; }  // True positive (fraction of)
  inline float Fp() const { return fp_; }  // False positive (fraction of)
  inline float Fn() const { return fn_; }  // False negative (fraction of)

  inline bool HasRef() const { return ref_; }
  inline bool HasHyp() const { return hyp_; }

  inline const RefEvent& Ref() const { assert(ref_ != nullptr); return *ref_; }
  inline const HypEvent& Hyp() const { assert(hyp_ != nullptr); return *hyp_; }

 private:
  float tp_, fp_, fn_;
  std::unique_ptr<RE> ref_;
  std::unique_ptr<HE> hyp_;
};

}  // namespace evaluator
}  // namespace kws

#endif  // EVALUATOR_PAIRED_EVENT_H_
