/*
 * File: bid_estimator.cpp
 * -------------------------------------------------------------------------------------------------
 * Author/Developer: Evan Sheehan
 *
 */

#include "generics.h"
#include "graphics_engine.h"
#include "math_engine.h"
#include "gevents.h"
#include "strlib.h"
#include "goptionpane.h"

#include "console.h"

const int STRING_CATEGORY_END_INDEX = 5;

/*
 */
void evaluate_table_event(GTableEvent e) {

    if (e.getRow() > STRING_CATEGORY_END_INDEX) {
        if (e.getColumn() == -SENTINEL || (e.getColumn() == INDEX_TWO && e.getValue() != "")) {
            if (!getIntegerAugment(e.getValue())) {
                GOptionPane::showMessageDialog("Invalid Numerical Entry");
                return;
            }
        }
    }

    if ((e.getRow() < (item_vector.size() + TABLE_ITEMS_START_ROW)) &&
            (e.getRow() >= TABLE_ITEMS_START_ROW)) {
        if (e.getColumn() == -SENTINEL) {
            update_cost_vector(e.getRow(), e.getValue());
        } else if (e.getColumn() == INDEX_TWO) {
            update_square_footage_vector(e.getRow(), e.getValue());
        }
    }

    update_totals();
}

/*
 */
void evaluate_action_event(GActionEvent e) {

    if (e.getSource() == square_footage_field) {

        if (!getIntegerAugment(square_footage_field->getText())) {
            GOptionPane::showMessageDialog("Invalid Numerical Entry");
            return;
        }

        update_universal_square_footage();
        update_totals();
    } else if (e.getSource() == add_new_category_button) {
        add_new_category();
        update_totals();
    } else if (e.getSource() == remove_category_button) {
        remove_category();
        update_totals();
    } else if (e.getSource() == run_analysis_button) {
        update_totals();
    } else if (e.getSource() == reset_button) {
        data_table->clear();
        init_interactor_data();
    }
}

int main() {

    init_window();
    init_interactors();
    init_interactor_data();
    add_interactors();

    while (true) {
        GEvent e = waitForEvent(TABLE_UPDATED | ACTION_EVENT);
        if (e.getEventType() == TABLE_UPDATED) {
            evaluate_table_event(e);
        } else {
            evaluate_action_event(e);
        }
    }

    return 0;
}
