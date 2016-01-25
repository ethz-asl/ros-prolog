# A C++ implementation for using Prolog in ROS

## Overview

**Author(s): Ralf Kaestner</br>
Affiliation: Autonomous Systems Lab, ETH Zurich**

## Content

This project provides a ROS server/client interface to Prolog.

## Installation

### Dependencies

- [swi-prolog](http://www.swi-prolog.org)

  ```
  sudo apt-get install swi-prolog
  ```
 
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)

  ```
  sudo apt-get install libjsoncpp-dev
  ```

### Building

Create a symlink in your catkin source folder, e.g.:

  ```
  ln -s ~/git/ros-prolog ~/catkin_ws/src
  ```

If you just need certain packages of this project:

  ```
  ln -s ~/git/ros-prolog/name_of_the_package ~/catkin_ws/src
  ```

## Usage

### Prolog Server

To launch the Prolog server, run

  ```
  roslaunch prolog_server mt_server_node.launch
  ```

### Interactive Prolog Client

To launch the interactive Prolog client, run

  ```
  roslaunch prolog_client interactive_client_node.launch
  ```

## Bugs & Feature Requests

Please report bugs and feature requests on the
[Issue Tracker](https://github.com/ethz-asl/ros-prolog).
