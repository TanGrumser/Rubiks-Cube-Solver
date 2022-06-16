#pragma once

#include <cstddef>
#include <array>
#include <bitset>
#include <iostream>

using std::size_t;
using std::array;
using std::bitset;
using std::cout;
using std::endl;

// Calculate nPk: n!/(n-k)!.
unsigned pick(unsigned n, unsigned k);

/**
 * Class for generating sequential indexes for permutations of size N picked K
 * ways.
 */
template <size_t N, size_t K = N>
class PermutationIndexer {
  // Precomputed table containing the number of ones in the binary
  // representation of each number.  The largest N-bit number is
  // 2^N-1 = (1 << N) - 1.
  static array<unsigned, (1 << N) - 1> onesCountLookup;

  // Precomputed table of factorials (or "picks" if N != K).  They're in
  // reverse order.
  static array<unsigned, K> factorials;

public:
  /**
   * Popluate static factorials and ones-count tables.
   */
  PermutationIndexer()
  {
    for (unsigned i = 0; i < (1 << N) - 1; ++i)
    {
      bitset<N> bits(i);
      this->onesCountLookup[i] = bits.count();
    }

    for (unsigned i = 0; i < K; ++i)
      this->factorials[i] = pick(N - 1 - i, K - 1 - i); 
  }

  /**
   * Calculate the lexicographic rank (the index) of a permutation in O(n)
   * complexity.
   */
  unsigned rank(const array<unsigned, K>& perm) const
  {
    // This will hold the Lehmer code (in a factorial number system).
    array<unsigned, K> lehmer;

    // Set of "seen" digits in the permutation.
    bitset<N> seen;

    // The first digit of the Lehmer code is always the first digit of
    // the permutation.
    lehmer[0] = perm[0];

    // Mark the digit as seen (bitset uses right-to-left indexing).
    seen[N - 1 - perm[0]] = 1;

    for (unsigned i = 1; i < K; ++i)
    {
      seen[N - 1 - perm[i]] = 1;

      // The number of "seen" digits to the left of this digit is the
      // count of ones left of this digit.
      unsigned numOnes = this->onesCountLookup[seen.to_ulong() >> (N - perm[i])];

      lehmer[i] = perm[i] - numOnes;
    }

    // Convert the Lehmer code to base-10.
    unsigned index = 0;

    for (unsigned i = 0; i < K; ++i)
      index += lehmer[i] * this->factorials[i];

    return index;
  }
};

// Static member definitions.
template <size_t N, size_t K>
array<unsigned, (1 << N) - 1> PermutationIndexer<N, K>::onesCountLookup;

template <size_t N, size_t K>
array<unsigned, K> PermutationIndexer<N, K>::factorials;

/*
int main(int argc, char* argv[])
{
  // All permutations of 4 numbers, in order.
  array<array<unsigned, 4>, 24> permsOf4 =
  {{
    {0, 1, 2, 3}, {0, 1, 3, 2}, {0, 2, 1, 3}, {0, 2, 3, 1},
    {0, 3, 1, 2}, {0, 3, 2, 1}, {1, 0, 2, 3}, {1, 0, 3, 2},
    {1, 2, 0, 3}, {1, 2, 3, 0}, {1, 3, 0, 2}, {1, 3, 2, 0},
    {2, 0, 1, 3}, {2, 0, 3, 1}, {2, 1, 0, 3}, {2, 1, 3, 0},
    {2, 3, 0, 1}, {2, 3, 1, 0}, {3, 0, 1, 2}, {3, 0, 2, 1},
    {3, 1, 0, 2}, {3, 1, 2, 0}, {3, 2, 0, 1}, {3, 2, 1, 0}
  }};

  PermutationIndexer<4> indexer1;

  for (const array<unsigned, 4>& perm: permsOf4)
  {
    cout << "Permutation: ";
    for (unsigned i : perm)
      cout << i << ' ';
    cout << "Index: " << indexer1.rank(perm) << '\n';
  }

  cout << endl;

  // All partial permutations of 4 items picked 2 ways, in order.
  array<array<unsigned, 2>, 12> permsOf4Pick2 =
  {{
    {0, 1}, {0, 2}, {0, 3}, {1, 0}, {1, 2}, {1, 3},
    {2, 0}, {2, 1}, {2, 3}, {3, 0}, {3, 1}, {3, 2}
  }};

  PermutationIndexer<4, 2> indexer2;

  for (const array<unsigned, 2>& perm: permsOf4Pick2)
  {
    cout << "Permutation: ";
    for (unsigned i : perm)
      cout << i << ' ';
    cout << "Index: " << indexer2.rank(perm) << endl;
  }

  return 0;
}
*/