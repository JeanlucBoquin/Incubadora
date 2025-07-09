#ifndef CONFIGURACION_FSM_H
#define CONFIGURACION_FSM_H

  #include <Fsm.h>

  void on_state();
  void a_in_state();
  void b_in_state();
  void c_in_state();
  void check_button();

  extern State state_a;
  extern State state_b;
  extern State state_c;
  extern Fsm fsm;

#endif