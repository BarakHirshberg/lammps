/* -*- c++ -*- ----------------------------------------------------------
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level LAMMPS directory.
------------------------------------------------------------------------- */

#ifdef FIX_CLASS

FixStyle(pimdb,FixPIMDB)

#else

#ifndef FIX_PIMDB_H
#define FIX_PIMDB_H

#include "fix.h"
#include <vector>

namespace LAMMPS_NS {

class FixPIMDB : public Fix {
 public:
  FixPIMDB(class LAMMPS *, int, char **);

  int setmask();

  void init();
  void setup(int);
  void post_force(int);
  void initial_integrate(int);
  void final_integrate();
  void end_of_step();

  double memory_usage();
  void grow_arrays(int);
  void copy_arrays(int,int,int);
  int pack_exchange(int,double*);
  int unpack_exchange(int,double*);
  int pack_restart(int,double*);
  void unpack_restart(int,int);
  int maxsize_restart();
  int size_restart(int);
  double compute_vector(int);

  int pack_forward_comm(int, int*, double *, int, int*);
  void unpack_forward_comm(int, int, double *);

  int method;
  int np;
  double inverse_np;

  /* ring-polymer model */

  double omega_np, fbond, spring_energy, sp;
  int x_last, x_next;

  void spring_force();
  double Evaluate_Ekn(const int n, const int k);
  std::vector<double> Evaluate_dEkn_on_atom(const int n, const int k, const int atomnum);
  std::vector<double> Evaluate_VBn(std::vector <double>& V, const int n);
  std::vector<std::vector<double>> Evaluate_dVBn(const std::vector <double>& V, const std::vector <double>& save_E_kn, const int n);
  void shuffle_atoms_list(std::vector<int>& list);

  /* fictious mass */

  double fmass, *mass;

  /* inter-partition communication */

  int max_nsend;
  tagint* tag_send;
  double *buf_send;

  int max_nlocal;
  double *buf_recv, **buf_beads;

  int size_plan;
  int *plan_send, *plan_recv;
  double **comm_ptr;

  void comm_init();
  void comm_exec(double **);

  /* normal-mode operations */

  double *lam, **M_x2xp, **M_xp2x, **M_f2fp, **M_fp2f;
  int *mode_index;

  void nmpimd_init();
  void nmpimd_fill(double**);
  void nmpimd_transform(double**, double**, double*);

  /* Nose-hoover chain integration */

  int nhc_offset_one_1, nhc_offset_one_2;
  int nhc_size_one_1, nhc_size_one_2;
  int nhc_nchain;
  bool nhc_ready;
  double nhc_temp, dtv, dtf, t_sys;

  double **nhc_eta;        /* coordinates of NH chains for ring-polymer beads */
  double **nhc_eta_dot;    /* velocities of NH chains                         */
  double **nhc_eta_dotdot; /* acceleration of NH chains                       */
  double **nhc_eta_mass;   /* mass of NH chains                               */

  void nhc_init();
  void nhc_update_v();
  void nhc_update_x();

  std::vector<double> E_kn;
  std::vector<double> V;
  std::vector<int> atoms_list;
  double virial;
  int nbosons; 
  int seed_shuffle;
  int freq_shuffle;

};


}

#endif
#endif
