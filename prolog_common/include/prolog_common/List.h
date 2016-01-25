/******************************************************************************
 * Copyright (C) 2016 by Ralf Kaestner                                        *
 * ralf.kaestner@gmail.com                                                    *
 *                                                                            *
 * This program is free software; you can redistribute it and/or modify       *
 * it under the terms of the Lesser GNU General Public License as published by*
 * the Free Software Foundation; either version 3 of the License, or          *
 * (at your option) any later version.                                        *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the               *
 * Lesser GNU General Public License for more details.                        *
 *                                                                            *
 * You should have received a copy of the Lesser GNU General Public License   *
 * along with this program. If not, see <http://www.gnu.org/licenses/>.       *
 ******************************************************************************/

/** \file List.h
  * \brief Header file providing the List class interface
  */

#ifndef ROS_PROLOG_LIST_H
#define ROS_PROLOG_LIST_H

#include <list>

#include <prolog_common/Term.h>

namespace prolog {
  /** \brief Prolog list
    */
  class List :
    public Term {
  public:
    /** \brief Default constructor
      */
    List(const std::list<Term>& elements = std::list<Term>());
      
    /** \brief Copy constructor
      */
    List(const List& src);
    
    /** \brief Copy constructor (overloaded version taking a term)
      */
    List(const Term& src);
    
    /** \brief Destructor
      */
    virtual ~List();
    
    /** \brief Retrieve the number of elements of this Prolog list
      */
    size_t getNumElements() const;
    
    /** \brief Retrieve the elements of this Prolog list
      */
    std::list<Term> getElements() const;
    
    /** \brief True, if this Prolog list is empty
      */
    bool isEmpty() const;
    
    /** \brief Retrieve the begin iterator of this Prolog list
      */ 
    std::list<Term>::iterator begin();
    
    /** \brief Retrieve the begin const-iterator of this Prolog list
      */ 
    std::list<Term>::const_iterator begin() const;

    /** \brief Retrieve the end iterator of this Prolog list
      */ 
    std::list<Term>::iterator end();
    
    /** \brief Retrieve the end const-iterator of this Prolog list
      */ 
    std::list<Term>::const_iterator end() const;
    
    /** \brief Append an element to this Prolog list
      */
    void append(const Term& element);
    
    /** \brief Append the elements of another Prolog list to this
      *   Prolog list
      */
    void append(const List& list);
    
    /** \brief Clear this Prolog list
      */
    void clear();
    
    /** \brief Unary operator for appending an element to this Prolog list
      */
    List& operator+=(const Term& element);
    
    /** \brief Unary operator for appending the elements of another Prolog
      *   list to this Prolog list
      */
    List& operator+=(const List& list);
    
    /** \brief Binary operator for appending an element to this Prolog list
      */
    List operator+(const Term& element) const;
    
    /** \brief Binary operator for appending the elements of another Prolog
      *   list to this Prolog list
      */
    List operator+(const List& list) const;
    
  protected:
    friend class Term;
    
    /** \brief Prolog list (implementation)
      */
    class Impl :
      public Term::Impl {
    public:
      Impl(const std::list<Term>& elements);
      virtual ~Impl();
      
      std::list<Term> elements_;
    };
  };
};

#endif
