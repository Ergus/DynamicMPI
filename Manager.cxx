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

#include "Node.h"

Manager::Manager(int &argc, char** &argv)
{
	int provided;

	MPI_Init_thread( &argc, &argv, MPI_THREAD_MULTIPLE, &provided);

	if (provided != MPI_THREAD_MULTIPLE) {
		fprintf(stderr,"Error in MPI initialisation\n");
		MPI_Abort(MPI_COMM_WORLD, -1);
	}

	MPI_Comm_get_parent(&parent);  // Parent to decide

	if(parent == MPI_COMM_NULL)
		Node = new Node_master(argc, argv, parent);
	else
		Node = new Node_slave(argc, argv, parent);

}

Manager::~Manager()
{
	delete Node;
	MPI_Finalize();
}
