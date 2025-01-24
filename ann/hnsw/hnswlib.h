#include <iostream>
#include <queue>
#include <vector>

namespace hnswlib {

typedef size_t labeltype;

class BaseFilterFunctor {
 public:
  virtual bool operator()(hnswlib::labeltype id) { return true; }
  virtual ~BaseFilterFunctor() {}
};

template <typename dist_t>
class BaseSearchStopCondition {
 public:
  virtual void add_point_to_result(labeltype label, const void *datapoint,
                                   dist_t dist) = 0;

  virtual void remove_point_from_result(labeltype label, const void *datapoint,
                                        dist_t dist) = 0;

  virtual bool should_stop_search(dist_t candidate_dist, dist_t lowerBound) = 0;

  virtual bool should_consider_candidate(dist_t candidate_dist,
                                         dist_t lowerBound) = 0;

  virtual bool should_remove_extra() = 0;

  virtual void filter_results(
      std::vector<std::pair<dist_t, labeltype>> &candidates) = 0;

  virtual ~BaseSearchStopCondition() {}
};

template <typename T>
class pairGreater {
 public:
  bool operator()(const T &p1, const T &p2) { return p1.first > p2.first; }
};

template <typename T>
static void writeBinaryPOD(std::ostream &out, const T &podRef) {
  out.write((char *)&podRef, sizeof(T));
}

template <typename T>
static void readBianryPOD(std::istream &in, T &podRef) {
  in.read((char *)&podRef, sizeof(T));
}

template <typename MTYPE>
using DISTFUNC = MTYPE (*)(const void *, const void *, size_t);

template <typename MTYPE>
class SpaceInterface {
  virtual size_t get_data_size() = 0;
  virtual DISTFUNC<MTYPE> get_dist_func() = 0;
  virtual void *get_dist_func_param() = 0;
  virtual ~SpaceInterface() {}
};

template <typename dist_t>
class AlgorithmInterface {};

}  // namespace hnswlib
