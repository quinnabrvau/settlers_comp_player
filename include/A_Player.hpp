//
//  A_Player.hpp
//  GENETIC_TEST_LIB
//
//  Created by Quinn Abrahams-Vaughn on 1/10/19.
//

#ifndef A_Player_hpp
#define A_Player_hpp

#include "Matrix.hpp"
#include "Player.hpp"
#include <vector>

typedef matrix::Matrix<float> State;
typedef matrix::Matrix<float> pl_matrix;

class A_Player:public Player {
    std::vector<int> dims;
    std::vector<pl_matrix> array;
    
    pl_matrix calculate(State state);
    
public:
    A_Player(std::vector<pl_matrix> _array, int p = 0);
    
    // take a turn
    // given a set of moves (think start the game)
    Move move(State state);
    Move move(State state, std::vector<Move> moves);


    typedef typename std::vector<pl_matrix>::iterator top_lvl_iterator; //column iterator
    typedef typename pl_matrix::iterator bottom_lvl_iterator;              //row iterator
    typedef typename std::vector<pl_matrix>::const_iterator top_lvl_iterator_const; //column iterator const
    typedef typename pl_matrix::const_iterator bottom_lvl_iterator_const;              //row iterator const

    class iterator {
    public:
        top_lvl_iterator cit, end_;
        bottom_lvl_iterator rit;
        int index;

        typedef std::bidirectional_iterator_tag iterator_category;
        typedef size_t difference_type;
        
        iterator() {}

        iterator(top_lvl_iterator _c_it,bottom_lvl_iterator _r_it,top_lvl_iterator _end, int _index) :
            cit(_c_it), end_(_end), rit(_r_it), index(_index) {}
        
        bool operator==(iterator const * const it) const{
            if (cit == end_ && it->cit == end_)
                return true;
            if (this->cit != it->cit)
                return false;
            if (this->rit != it->rit)
                return false;
            return true;
        }
        bool operator!=(iterator const * const it) const{
            if (cit == end_ && it->cit == end_)
                return false;
            if (this->cit == it->cit && this->rit == it->rit)
                return false;
            return true;
        }
        
        bool operator==(iterator const &it) const{
            if (cit == end_ && it.cit == end_)
                return true;
            if (this->cit != it.cit)
                return false;
            if (this->rit != it.rit)
                return false;
            return true;
        }
        bool operator!=(iterator const &it) const{
            if (cit == end_ && it.cit == end_)
                return false;
            if (this->cit == it.cit && this->rit == it.rit)
                return false;
            return true;
        }
        void next(void) {
            if (cit == end_)
                return;
            rit++;
            index++;
            if (rit == (*cit).end()) {
                cit++;
                if (cit != end_) {
                    rit = (*cit).begin();
                    index = 0;
                }
            }
        }
        void prev(void) {
            if (rit == (*cit).begin() ||
                cit == end_ ) {
                cit--;
                rit = (*cit).end();
                rit--;
                index = (*cit).size();
            } else {
                rit--;
                index--;
            }
            if (index < 0) {
                index--;
            }
        }
        iterator operator++(void) {
            this->next(); return *this;
        }
        iterator operator++(int junk) {
            iterator out = *this;
            this->next();
            return out;
        }
        iterator operator--(void) {
            this->prev(); return *this;
        }
        iterator operator--(int junk) {
            iterator out = *this;
            this->prev();
            return out;
        }

        float& operator*(void) {
            return (*rit);
        }
        float* operator->(void) {
            return rit.operator->();
        }
        
        void operator*=(int junk) {
            cit++;
            rit = (*cit).begin();
            for (int i = 0; i < index; i++) {
                rit++;
            }
        }
        bottom_lvl_iterator begin(void) {
            return (*cit).begin();
        }
        bottom_lvl_iterator end(void) {
            return (*cit).end();
        }
    };
    
    class const_iterator:public iterator {
        top_lvl_iterator_const cit, end_;
        bottom_lvl_iterator_const rit;
        int index;
    public:
        // const_iterator(top_lvl_iterator _c_it, bottom_lvl_iterator _r_it, top_lvl_iterator _end, int _index) :
            // iterator(), cit(_c_it), end_(_end), rit(_r_it), index(_index) { }

        const_iterator(top_lvl_iterator_const _c_it, bottom_lvl_iterator_const _r_it, top_lvl_iterator_const _end, int _index) :
            iterator(), cit(_c_it), end_(_end), rit(_r_it), index(_index) { }
        
        const_iterator operator++(void) {
            this->next(); return *this;
        }
        const_iterator operator++(int junk) {
            const_iterator out = *this;
            this->next();
            return out;
        }
        const_iterator operator--(void) {
            this->prev(); return *this;
        }
        const_iterator operator--(int junk) {
            const_iterator out = *this;
            this->prev();
            return out;
        }
        const float& operator*(void) const {
            return (this->rit).operator*();
        }
        const float* operator->(void) {
            return (this->rit).operator->();
        }
        bottom_lvl_iterator_const begin(void) {
            return (*(this->cit)).cbegin();
        }
        bottom_lvl_iterator_const end(void) {
            return (*(this->cit)).cend();
        }
    };

    // Iterator Creator
    iterator begin(void) {
        return iterator(array.begin(),array[0].begin(),array.end(),0);
    }
    iterator end(void) {
        return iterator(array.end(),array.back().end(),array.end(),array.back().size());
    }
    const_iterator begin(void) const {
        return const_iterator(array.cbegin(),array[0].cbegin(),array.cend(),0);
    }
    const_iterator end(void) const {
        return const_iterator(array.cend(),array.back().cend(),array.cend(),array.back().size());
    }
    const_iterator cbegin(void) const {
        return const_iterator(array.cbegin(),array[0].cbegin(),array.cend(),0);
    }
    const_iterator cend(void) const {
        return const_iterator(array.cend(),array.back().cend(),array.cend(),array.back().size());
    }
    
// Iterator Init
    A_Player (iterator be, iterator end, int p = 0);


};

#endif /* A_Player_hpp */
