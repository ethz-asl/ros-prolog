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

/** \file InteractiveClient.h
  * \brief Header file providing the example InteractiveClient class interface
  */

#ifndef ROS_PROLOG_INTERACTIVE_CLIENT_H
#define ROS_PROLOG_INTERACTIVE_CLIENT_H

#include <string>

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <prolog_client/Client.h>

namespace prolog {
  namespace client {
    /** \brief Interactive Prolog client implementation
      */  
    class InteractiveClient :
      public Client {
    public:
      /** \brief Default constructor
        */
      InteractiveClient();
      
      /** \brief Destructor
        */
      virtual ~InteractiveClient();
    
    protected:
      /** \brief Initialize the interactive Prolog client
        */
      void init();
      
      /** \brief Cleanup the interactive Prolog client
        */
      void cleanup();
      
    private:
      /** \brief The Prolog service client of this interactive
        *   Prolog client
        */
      ServiceClient client_;
      
      /** \brief The I/O service of this interactive Prolog client
        */
      boost::asio::io_service io_;
      
      /** \brief The I/O service thread of this interactive Prolog client
        */
      boost::thread thread_;
      
      /** \brief The input stream of this interactive Prolog client
        */
      boost::asio::posix::stream_descriptor input_;
      
      /** \brief The input buffer of this interactive Prolog client
        */
      boost::asio::streambuf buffer_;
      
      /** \brief The query goal of this interactive Prolog client
        */
      std::string goal_;
      
      /** \brief Start console input
        */
      void startInput();
      
      /** \brief Handle console input
        */
      void handleInput(const boost::system::error_code& error,
        size_t length);
    };
  };
};

#endif
