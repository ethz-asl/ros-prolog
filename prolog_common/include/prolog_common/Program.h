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

/** \file Program.h
  * \brief Header file providing the Program class interface
  */

#ifndef ROS_PROLOG_PROGRAM_H
#define ROS_PROLOG_PROGRAM_H

#include <list>

#include <prolog_common/Clause.h>

namespace prolog {
  /** \brief Prolog program
    */
  class Program {
  public:
    /** \brief Default constructor
      */
    Program(const std::list<Clause>& clauses = std::list<Clause>());
      
    /** \brief Copy constructor
      */
    Program(const Program& src);
    
    /** \brief Destructor
      */
    virtual ~Program();
    
    /** \brief Retrieve the number of clauses of this Prolog program
      */
    size_t getNumClauses() const;
    
    /** \brief Retrieve the clauses of this Prolog program
      */
    std::list<Clause> getClauses() const;
    
    /** \brief True, if this Prolog program is empty
      */
    bool isEmpty() const;
    
    /** \brief Retrieve the begin iterator of this Prolog program
      */ 
    std::list<Clause>::iterator begin();
    
    /** \brief Retrieve the begin const-iterator of this Prolog program
      */ 
    std::list<Clause>::const_iterator begin() const;

    /** \brief Retrieve the end iterator of this Prolog program
      */ 
    std::list<Clause>::iterator end();
    
    /** \brief Retrieve the end const-iterator of this Prolog program
      */ 
    std::list<Clause>::const_iterator end() const;
    
    /** \brief Append a clause to this Prolog program
      */
    void append(const Clause& clause);
    
    /** \brief Append the elements of another Prolog program to this
      *   Prolog program
      */
    void append(const Program& program);
    
    /** \brief Clear this Prolog program
      */
    void clear();
    
    /** \brief Unary operator for appending a clause to this Prolog program
      */
    Program& operator+=(const Clause& clause);
    
    /** \brief Unary operator for appending the clauses of another Prolog
      *   program to this Prolog program
      */
    Program& operator+=(const Program& program);
    
    /** \brief Binary operator for appending a clause to this Prolog program
      */
    Program operator+(const Clause& clause) const;
    
    /** \brief Binary operator for appending the clauses of another Prolog
      *   program to this Prolog program
      */
    Program operator+(const Program& program) const;
    
  protected:
    /** \brief Prolog program (implementation)
      */
    class Impl {
    public:
      Impl(const std::list<Clause>& clauses);
      virtual ~Impl();
      
      std::list<Clause> clauses_;
    };
    
    /** \brief The Prolog program's implementation
      */
    boost::shared_ptr<Impl> impl_;
  };
};

#endif
