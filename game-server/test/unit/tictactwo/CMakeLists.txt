add_library(tictactwo_tests OBJECT
  test_legal_move_grid_action.cc
  test_illegal_move_grid_action.cc
  test_place_piece_action.cc
  test_move_piece_action.cc
  test_board_is_full_rule.cc
  test_has_alignment_rule_fixed_grid.cc
  test_has_alignment_rule_moving_grid.cc
  test_tictactwo.cc
  )
