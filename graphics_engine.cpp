/*
 * Project: bid_estimator
 * File: graphics_engine.cpp
 * -------------------------------------------------------------------------------------------------
 * Author/Developer: Evan Sheehan
 *
 */

#include "graphics_engine.h"
#include "generics.h"
#include "gtable.h"
#include "gwindow.h"
#include "ginteractors.h"
#include "goptionpane.h"
#include "vector.h"
#include <string>
#include "set.h"

struct SOFT_DATA {

    std::string address;
    std::string plan;
    std::string lot_number;
    std::string number_of_baths;
    std::string garage_data;

    std::string land_cost;
    std::string finance;
    std::string home_warranty;
    std::string liability_insurance;
    std::string escrow;
    std::string commission;
    std::string sales_price;

};

// For Interactors
GWindow main_window;
GTextField* square_footage_field;
GButton* add_new_category_button;
GButton* run_analysis_button;
GButton* remove_category_button;
GButton* reset_button;
GTable* data_table;

// For Database Interactions
Vector<std::string> item_vector;
Vector<std::string> cost_vector;
Vector<std::string> square_footage_vector;
SOFT_DATA soft_data;
Set<int> square_footage_dependent_indeces;

// For Main Window
const int WINDOW_OFFSET = 90;

// For Data Table
const double TABLE_HEIGHT_FACTOR = 16.05;
const int INITIAL_ROW_HEIGHT = 22;
const double COLUMN_WIDTH = 200;

const int TABLE_ITEMS_START_ROW = 9;

const int NUM_ROWS = 80;
const int NUM_COLUMNS = 3;

std::string initial_square_footage = "2000";
std::string initial_value = "0";

// For Square Footage Dependencies
const int DRYWALL_INDEX = 8;
const int FOUNDATION_INDEX = 21;
const int FRAME_LABOR_INDEX = 23;
const int FRAME_LUMBER_INDEX = 24;
const int INSULATION_INDEX = 29;
const int ROOF_TRUSS_INDEX = 38;
const int ROOF_TILE_INDEX = 39;

/*
 * Active Functions For Action And Table Events
 * -------------------------------------------------------------------------------------------------
 *
 */

/*
 */
void add_string_labels() {

    data_table->set(ZERO, ZERO, "ADDRESS:");
    data_table->set(-SENTINEL, ZERO, "PLAN:");
    data_table->set(INDEX_TWO, ZERO, "LOT #:");
    data_table->set(INDEX_THREE, ZERO, "NUMBER OF BATHS:");
    data_table->set(INDEX_FOUR, ZERO, "TWO OR THREE CAR GARAGE:");
}

/*
 */
void add_soft_and_summary_labels() {

    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW - SENTINEL, ZERO,
            "CONSTRUCTION COST");
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_THREE, ZERO, "LOT COST");
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_FOUR, ZERO,
                    "CONSTRUCTION + LOST COST");
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_SIX, ZERO, "FINANCE");
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_SEVEN, ZERO,
                    "2-10 HOME WARRANTY");
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_EIGHT, ZERO,
                    "LIABILITY INSURANCE");
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_NINE, ZERO, "ESCROW");
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_TEN, ZERO,
                    "RE COMMISSION 5%");
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_ELEVEN, ZERO, "SOFT TOTAL");
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_THIRTEEN, ZERO,
                    "SALES PRICE");
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_FIFTEEN, ZERO, "PROFIT");
}

/*
 */
void add_labels_to_table() {

    add_string_labels();

    data_table->set(INDEX_SIX, ZERO, "LOT COST:");
    data_table->set(INDEX_EIGHT, -SENTINEL, "$ / SQ. FOOT (or Total Cost)");
    data_table->set(INDEX_EIGHT, INDEX_TWO, "SQUARE FOOTAGE (if used)");

    for (int i = ZERO; i <item_vector.size(); i++) {
        data_table->set(i + TABLE_ITEMS_START_ROW, ZERO, item_vector[i]);
    }

    add_soft_and_summary_labels();
}

/*
 */
void set_summary_data(int land_cost, int construction_cost, int soft_cost) {

    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW - SENTINEL, -SENTINEL,
            integerToString(construction_cost));
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_THREE, -SENTINEL,
            integerToString(land_cost));
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_FOUR, -SENTINEL,
            integerToString(land_cost + construction_cost));
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_ELEVEN, -SENTINEL,
            integerToString(soft_cost));
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_FIFTEEN, -SENTINEL,
            integerToString(stringToInteger(data_table->get(item_vector.size() +
            TABLE_ITEMS_START_ROW + INDEX_THIRTEEN, -SENTINEL)) - construction_cost - land_cost -
            soft_cost));
}

/*
 */
void update_soft_data() {

    soft_data.address =  data_table->get(ZERO, -SENTINEL);
    soft_data.plan = data_table->get(-SENTINEL, -SENTINEL);
    soft_data.lot_number = data_table->get(INDEX_TWO, -SENTINEL);
    soft_data.number_of_baths = data_table->get(INDEX_THREE, -SENTINEL);
    soft_data.garage_data = data_table->get(INDEX_FOUR, -SENTINEL);
    soft_data.land_cost = data_table->get(INDEX_SIX, -SENTINEL);

    soft_data.finance = data_table->get(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_SIX,
            -SENTINEL);
    soft_data.home_warranty = data_table->get(item_vector.size() + TABLE_ITEMS_START_ROW +
            INDEX_SEVEN, -SENTINEL);
    soft_data.liability_insurance = data_table->get(item_vector.size() + TABLE_ITEMS_START_ROW +
            INDEX_EIGHT, -SENTINEL);
    soft_data.escrow = data_table->get(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_NINE,
            -SENTINEL);
    soft_data.commission = data_table->get(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_TEN,
            -SENTINEL);
    soft_data.sales_price = data_table->get(item_vector.size() + TABLE_ITEMS_START_ROW +
            INDEX_THIRTEEN, -SENTINEL);
}

/*
 */
void set_table_cost_and_square_footage() {
    for (int i = ZERO; i < item_vector.size(); i++) {
        data_table->set(i + TABLE_ITEMS_START_ROW, -SENTINEL, cost_vector[i]);
        data_table->set(i + TABLE_ITEMS_START_ROW, INDEX_TWO, square_footage_vector[i]);
    }
}

/*
 */
void update_universal_square_footage() {
    for (int index: square_footage_dependent_indeces) {
        square_footage_vector[index] = square_footage_field->getText();
    }
    set_table_cost_and_square_footage();
}

/*
 */
void update_square_footage_vector(int row, std::string value) {
    if (value == "") {
        value = initial_value;
    }
    square_footage_vector[row - TABLE_ITEMS_START_ROW] = value;
}

/*
 */
void update_cost_vector(int row, std::string value) {
    cost_vector[row - TABLE_ITEMS_START_ROW] = value;
}

/*
 */
void set_table_soft_data() {

    data_table->set(ZERO, -SENTINEL, soft_data.address);
    data_table->set(-SENTINEL, -SENTINEL, soft_data.plan);
    data_table->set(INDEX_TWO, -SENTINEL, soft_data.lot_number);
    data_table->set(INDEX_THREE, -SENTINEL, soft_data.number_of_baths);
    data_table->set(INDEX_FOUR, -SENTINEL, soft_data.garage_data);
    data_table->set(INDEX_SIX, -SENTINEL, soft_data.land_cost);

    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_THREE, -SENTINEL,
            soft_data.land_cost);

    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_SIX, -SENTINEL,
            soft_data.finance);
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_SEVEN, -SENTINEL,
            soft_data.home_warranty);
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_EIGHT, -SENTINEL,
            soft_data.liability_insurance);
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_NINE, -SENTINEL,
            soft_data.escrow);
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_TEN, -SENTINEL,
            soft_data.commission);
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_THIRTEEN, -SENTINEL,
            soft_data.sales_price);
}

/*
 */
void remove_category() {

    std::string input = toLowerCase(GOptionPane::showInputDialog
            ("Which category should be removed?"));

    if (input == "") return;

    int index = SENTINEL;
    for (int i = ZERO; i < item_vector.size(); i++) {
        if (toLowerCase(item_vector[i]) == input) {
            index = i;
            break;
        }
    }
    if (index == SENTINEL) return;

    item_vector.remove(index);
    cost_vector.remove(index);
    square_footage_vector.remove(index);

    data_table->clear();
    add_labels_to_table();
    set_table_cost_and_square_footage();
    set_table_soft_data();
}

/*
 */
void add_new_category() {

    std::string input = GOptionPane::showInputDialog("What is the category called?");

    if (input == "") return;

    data_table->resize(data_table->numRows() - SENTINEL, data_table->numCols());
    item_vector += input;
    cost_vector += initial_value;
    square_footage_vector += "";

    data_table->clear();
    add_labels_to_table();
    set_table_cost_and_square_footage();
    set_table_soft_data();
}

/*
 * Passive Functions For Initialization And Reset
 * -------------------------------------------------------------------------------------------------
 *
 */

/*
 */
void init_cost_and_square_footage_vectors() {

    cost_vector.clear();
    square_footage_vector.clear();

    for (int i = ZERO; i < item_vector.size(); i++) {
        cost_vector += initial_value;
        if (square_footage_dependent_indeces.contains(i)) {
            square_footage_vector += initial_square_footage;
        } else {
            square_footage_vector += "";
        }
    }
}

/*
 */
void init_item_vector() {

    item_vector.clear();
    item_vector = {"Appliances", "Architect Fees(Reuse)", "Building Permit/School Fee", "Cabinets",
            "Carpet", "Counter Tops", "Doors/Millwork", "Driveway", "Drywall", "Dump Fees",
            "Duct Test", "Edison", "Electrical", "Exterior Decorative Stone", "Fence Wood",
            "Fence Wrought Iron", "Finish Grade", "Fire Insurance", "Fire Place", "Fire Sprinklers",
            "Flat Work", "Foundation", "Front Door", "Frame Labor", "Frame Lumber", "Garage Door",
            "Gate", "Hardwood Floor", "Impact/Road Fees", "Insulation", "Interior Hardware",
            "Landscape", "Lights", "Mechanical", "Mirrors/Shower Door", "Painting", "PG&E",
            "Plumbing", "Roof Trusses", "Roof Tile", "Rough Grade", "School Tax",
            "Side/Laundry Doors", "Stucco", "Termite Prep", "Tile Floor", "Tile Kitchen Backsplash",
            "Tile Shower/Bath $1500 per", "Tile Master Tub Surround", "Sewer/Water/Gas Trenches",
            "Water Main", "Windows"};
}

/*
 */
void init_table_summary_data() {

    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW - SENTINEL, -SENTINEL,
            initial_value);
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_FOUR, -SENTINEL,
            initial_value);
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_ELEVEN, -SENTINEL,
            initial_value);
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_THIRTEEN, -SENTINEL,
            initial_value);
    data_table->set(item_vector.size() + TABLE_ITEMS_START_ROW + INDEX_FIFTEEN, -SENTINEL,
            initial_value);
}

/*
 */
void init_table_data() {

    square_footage_dependent_indeces = {DRYWALL_INDEX, FOUNDATION_INDEX, FRAME_LABOR_INDEX,
            FRAME_LUMBER_INDEX, INSULATION_INDEX, ROOF_TRUSS_INDEX, ROOF_TILE_INDEX};

    data_table->resize(NUM_ROWS, NUM_COLUMNS);
    data_table->setSize(data_table->numCols() * COLUMN_WIDTH, main_window.getCanvasHeight());

    init_item_vector();
    init_cost_and_square_footage_vectors();
    soft_data = {"", "", "", "", "", initial_value, initial_value, initial_value, initial_value,
            initial_value, initial_value, initial_value};

    add_labels_to_table();
    set_table_cost_and_square_footage();
    set_table_soft_data();
    init_table_summary_data();
}

/*
 */
void init_interactor_data() {
    square_footage_field->setText(initial_square_footage);
    init_table_data();
}

/*
 */
void init_interactors() {

    add_new_category_button = new GButton("Add New Category");
    remove_category_button = new GButton("Remove Category");
    run_analysis_button = new GButton("Run Analysis");
    reset_button = new GButton("Reset");
    square_footage_field = new GTextField();

    data_table = new GTable(NUM_ROWS, NUM_COLUMNS, ZERO, ZERO);
    data_table->setRowColumnHeadersVisible(false);
}

/*
 */
void add_interactors() {

    main_window.addToRegion(new GLabel("Square Footage:"), "NORTH");
    main_window.addToRegion(square_footage_field, "NORTH");

    main_window.addToRegion(add_new_category_button, "WEST");
    main_window.addToRegion(remove_category_button, "WEST");
    main_window.addToRegion(run_analysis_button, "WEST");
    main_window.addToRegion(reset_button, "WEST");

    main_window.add(data_table);
}

/*
 */
void init_window() {

    main_window.setLocation(ZERO, ZERO);
    main_window.setSize(getScreenWidth(), getScreenHeight() - WINDOW_OFFSET);
    main_window.setTitle("Bid Estimator");
}
