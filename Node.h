/******************************************************
 * This file is part of the Dynamic-MPI distribution Copyright (c) 2017 Jimmy
 * Aguilar Mena.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************/

#ifndef NODE_HH
#define NODE_HH

#include "mpi.h"
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <thread>

// Messages types
enum msg_tag {
	TAG_EXIT = 0,               // Exit
	TAG_REDUCE = 1,             // Delete processes
	TAG_SPAWN = 2,              // Spawn add new mpi processes
};

// Simplest message struct
typedef struct msg_t {
	msg_tag type;           // Message Tag value
	size_t number;          // Number of processes
} msg_t;

// Base Node class for master and slave
class Node_t {

public:
	virtual ~Node_t();
	virtual void run() = 0;

protected:
	Node_t(int &argc, char** &argv, MPI_Comm _parent);

	MPI_Comm intra, parent;            // persistent communicators
	int wsize, wrank;                  // environment MPI vars
	int nargc;                         // command line arguments number
	char** nargv;                      // command line arguments vars
	virtual int spawn_merge(size_t n); // spawns n new mpi processes
	virtual int split_kill(size_t n);  // split the actual communicator and kills
	bool listening;                    // process will listen initialised as true
};

class Node_master : public Node_t {

public:
	Node_master(int &argc, char** &argv, MPI_Comm _parent);
	~Node_master();

	void run() override;

private:
	int send_to_remotes(msg_t msg);    // like a broadcast with unblocking messages
	void stopall();                    // kill all remotes before exiting
	int spawn_merge(size_t n) override;
	int split_kill(size_t n) override;
};

class Node_slave : public Node_t {

public:
	Node_slave(int &argc, char** &argv, MPI_Comm _parent);
	~Node_slave();
	void run() override;

private:
	void listen();                     // listen function, running while listening==true
};

class Manager {

public:
	Manager(int &argc, char** &argv);
	~Manager();

	Node_t *Node;
	MPI_Comm parent;
};

#endif
