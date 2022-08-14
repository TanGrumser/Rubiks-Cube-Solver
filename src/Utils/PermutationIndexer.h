#pragma once

#include <cstddef>
#include <array>
#include <bitset>
#include <iostream>
#include <algorithm>
#include <vector>

using std::size_t;
using std::array;
using std::bitset;
using std::cout;
using std::endl;
using std::vector;

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

  // TODO changes 12 to N
  static array<array<unsigned, 12>, (1 << N) - 1> nthZero;

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
      
      for (unsigned zeroToFind = 0; zeroToFind < N; ++zeroToFind) {
        unsigned foundZeros = 0;

        //Count until j-th zero is found.
        for (unsigned position = 0; position < N; ++position) {
          if (bits[position] == 0) {
            
            if (foundZeros == zeroToFind) {
              nthZero[i][zeroToFind] = position;
              break;
            }
            
            foundZeros++;
          }
        } 
      }
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

    for (unsigned i = 0; i < K; ++i){
      index += lehmer[i] * this->factorials[i];
    }

    return index;
  }

  array<unsigned, K> getPermutation(uint64_t rank) const {
    // This will hold the reconstructed permutation.
    array<unsigned, K> perm;

    // Set of "seen" digits in the permutation.
    bitset<N> seen;

    perm[0] = rank / this->factorials[0];
    seen[perm[0]] = 1;

    for (unsigned i = 1; i < K; ++i) {
      // Extract the i-th lehmer value of the factorial lehmer code.
      unsigned lehmer = (rank % this->factorials[i - 1]) / this->factorials[i];
      
      perm[i] = nthZero[seen.to_ulong()][lehmer];
      seen[perm[i]] = 1;
    }

    return perm;
  }

  void TestPermutationGenerator() {
    PermutationIndexer<6> indexer;

    for (int rep = 0; rep < 1000; rep++) {
        array<unsigned, 6> perm = {11, 11, 11, 11, 11, 11};

        for (int i = 0; i < 6; i++) {
            bool unset = true;
            unsigned trie = 0;
            
            while (unset) {
                unsigned random = rand() % 6u;

                if (perm[random] == 11) {
                    perm[random] = i;
                    unset = false;
                }

                trie++;
            }
        }

        array<unsigned ,6> recosntruction = indexer.getPermutation(indexer.rank(perm));
        
        for (int i = 0; i < 6; i++) {
            if (perm[i] != recosntruction[i]) {
                std::cout << "Fail";
                break;
            }
        }
    }

    std::cout << "Sucess" << std::endl;
  }

  void TestSparsePermutationReconstruction() {
    PermutationIndexer<12, 7> indexer;

    for (int rep = 0; rep < 10; rep++) {
        array<unsigned, 7> perm = {13, 13, 13, 13, 13, 13, 13};
        vector<unsigned> usedValues;

        for (int i = 0; i < 7; i++) {
            unsigned value = 13;

            do {
                value = std::rand() % 12u;
            } while (std::find(usedValues.begin(), usedValues.end(), value) != usedValues.end());

            usedValues.push_back(value);
        }

        for (int i = 0; i < 7; i++) {
            bool unset = true;
            unsigned trie = 0;
            
            while (unset) {
                unsigned random = rand() % 7u;

                if (perm[random] == 13) {
                    perm[random] = usedValues[random];
                    unset = false;
                }

                trie++;
            }
        }

        array<unsigned, 7> recosntruction = indexer.getPermutation(indexer.rank(perm));
        
        for (int i = 0; i < 6; i++) {
            if (perm[i] != recosntruction[i]) {
                std::cout << "Fail" << endl;
                break;
            }
        }

        for (int i = 0; i < 7; i++) {
            std::cout << perm[i] << " ";
        }

        std::cout << "| ";

        for (int i = 0; i < 7; i++) {
            std::cout << recosntruction[i] << " ";
        }

        std::cout << std::endl;
    }
  }
};

// Static member definitions.
template <size_t N, size_t K>
array<unsigned, (1 << N) - 1> PermutationIndexer<N, K>::onesCountLookup;

// This member stores the position of the n-th zero for all permutaions.
//nthZero[b][n] stores the positiion of the n-th zero for the binary number b.
template <size_t N, size_t K>
array<array<unsigned, 12>, (1 << N) - 1> PermutationIndexer<N, K>::nthZero;

template <size_t N, size_t K>
array<unsigned, K> PermutationIndexer<N, K>::factorials;