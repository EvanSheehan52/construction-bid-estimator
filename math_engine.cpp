/*
 * Project: bid_estimator
 * File: math_engine.h
 * -------------------------------------------------------------------------------------------------
 * Author/Developer: Evan Sheehan
 *
 */

#include "math_engine.h"
#include "generics.h"
#include "graphics_engine.h"
#include "strlib.h"

/*
 */
int get_land_cost() {
    return stringToInteger(data_table->get(INDEX_SIX, -SENTINEL));
}

/*
 */
int sum_construction_costs() {

    int construction_total = ZERO;

    for (int i = ZERO; i < item_vector.size(); i++) {

        if (square_footage_vector[i] == "") {
            construction_total += stringToInteger(cost_vector[i]);
        } else {
            construction_total += stringToInteger(cost_vector[i]) *
                    stringToInteger(square_footage_vector[i]);
        }
    }

    return construction_total;
}

/*
 */
int sum_soft_costs() {

    int soft_cost = ZERO;

    for (int i = INDEX_SIX; i < INDEX_ELEVEN; i ++) {
        soft_cost += stringToInteger(data_table->get(item_vector.size() + TABLE_ITEMS_START_ROW + i,
                -SENTINEL));
    }

    return soft_cost;
}

/*
 */
void update_totals() {

    int land_cost = get_land_cost();
    int construction_cost = sum_construction_costs();
    int soft_cost = sum_soft_costs();

    set_summary_data(land_cost, construction_cost, soft_cost);
    update_soft_data();
}
