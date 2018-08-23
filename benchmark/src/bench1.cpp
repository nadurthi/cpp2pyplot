#include "benchmark/benchmark.h"
#include <array>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>

#include <set>
#include <map>

#include <unordered_set>
#include <unordered_map>

#include <string>
struct A{
	std::string s{"123456789123456789123456789123456789"};

};

template <class Q> 
void BM_push_back(benchmark::State& state) {
  Q q;
  typename Q::value_type v;
  for (auto _ : state) {
    for (int i = state.range(0); i--; )
      q.push_back(v);
  }
  // actually messages, not bytes:
  state.SetBytesProcessed(
      static_cast<int64_t>(state.iterations())*state.range(0));
  state.SetComplexityN(state.range(0));
}
BENCHMARK_TEMPLATE(BM_push_back, std::vector<A>)->Range(1<<0, 1<<20);
BENCHMARK_TEMPLATE(BM_push_back, std::list<A>)->Range(1<<0, 1<<20);
BENCHMARK_TEMPLATE(BM_push_back, std::deque<A>)->Range(1<<0, 1<<20);


template <class Q> 
void BM_resize(benchmark::State& state) {
  Q q;
  typename Q::value_type v;
  for (auto _ : state) {
      q.resize(state.range(0));
  }
  // actually messages, not bytes:
  state.SetBytesProcessed(
      static_cast<int64_t>(state.iterations())*state.range(0));
  state.SetComplexityN(state.range(0));
}
BENCHMARK_TEMPLATE(BM_resize, std::vector<A>)->Range(1<<0, 1<<20);
BENCHMARK_TEMPLATE(BM_resize, std::deque<A>)->Range(1<<0, 1<<20);



BENCHMARK_MAIN();