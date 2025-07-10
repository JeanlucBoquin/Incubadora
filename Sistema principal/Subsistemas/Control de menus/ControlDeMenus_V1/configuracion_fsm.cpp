#include "configuracion_fsm.h"

// ############################ Configuracion de FSM ##########################

// State state(&func_on_enter_state, &func_in_state, &func_on_exit_state)
State state_a(&on_state, &a_in_state, nullptr);
State state_b(&on_state, &b_in_state, nullptr);
State state_c(&on_state, &c_in_state, nullptr);

Fsm fsm(&state_a);