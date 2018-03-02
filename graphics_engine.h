/*
 * Project: bid_estimator
 * File: graphics_engine.h
 * -------------------------------------------------------------------------------------------------
 * Author/Developer: Evan Sheehan
 *
 */

#ifndef Graphics_Included
#define Graphics_Included

#include "gtable.h"
#include "gwindow.h"
#include "ginteractors.h"
#include "vector.h"
#include <string>

extern GWindow main_window;
extern GTextField* square_footage_field;
extern GButton* remove_category_button;
extern GButton* add_new_category_button;
extern GButton* run_analysis_button;
extern GButton* reset_button;
extern GTable* data_table;

extern Vector<std::string> item_vector;
extern Vector<std::string> cost_vector;
extern Vector<std::string> square_footage_vector;

extern const int TABLE_ITEMS_START_ROW;

/*
 */
void update_soft_data();

/*
 */
void set_summary_data(int land_cost, int construction_cost, int soft_cost);

/*
 */
void update_universal_square_footage();

/*
 */
void update_square_footage_vector(int row, std::string value);

/*
 */
void update_cost_vector(int row, std::string value);

/*
 */
void remove_category();

/*
 */
void add_new_category();

/*
 */
void init_window();

/*
 */
void init_interactor_data();

/*
 */
void init_interactors();

/*
 */
void add_interactors();

#endif
