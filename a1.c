#include "a1.h"

/** initialize_restaurant
 * utilizes load_menu
 *
 * @param name name of the restaurant
 * @return pointer to {@link Restaurant}
 */
Restaurant* initialize_restaurant(char* name) {
    //load menu
    Menu* menu = load_menu(MENU_FNAME);

    //load pending_orders
    Queue* pending = malloc(sizeof(Queue));
    pending->head = NULL;
    pending->tail = NULL;

    //Restaurant restaurant = {name, menu, 0, 0, &pending};
    Restaurant* restaurant = malloc(sizeof(Restaurant)); //changed your entire declaration of restaruant
    restaurant->name = name;
    restaurant->num_pending_orders = 0;
    restaurant->num_completed_orders = 0; 
    restaurant->pending_orders = pending;//not sure if this is right assignment
    restaurant->menu = menu;
    return restaurant;
}

/** load_menu
 *
 * @param fname file name containing menu items
 * @return pointer to {@link Menu}
 */
Menu* load_menu(char* fname) { //might need to rewrite this function
    int num_of_items = 0;

    char** item_code; 
    char** item_names;
    double* item_cost_per_unit;
    
    FILE* stream = fopen(fname, "r");


    char item[ITEM_CODE_LENGTH+MAX_ITEM_NAME_LENGTH+4+5+1];//not sure why you added those numbers

    while(fgets(item, ITEM_CODE_LENGTH+MAX_ITEM_NAME_LENGTH+4+5+1, stream) != NULL) {
        num_of_items++;
    }//Find how many items there are so 
    
    rewind(stream);

    item_code = (char**)malloc(sizeof(char*)*num_of_items);
    item_names = (char**)malloc(sizeof(char*)*num_of_items);
    item_cost_per_unit = (double*)malloc(sizeof(double)*num_of_items);

    for(int i = 0; i < num_of_items; i++){
        item_code[i] = (char*)malloc(sizeof(char)*ITEM_CODE_LENGTH);
        item_names[i] = (char*)malloc(sizeof(char)*MAX_ITEM_NAME_LENGTH);// idk why there's an error here
    }

    //stream = fopen(fname, "r");//reopen to start reading from the beginning again
    int j = 0;
    while(fgets(item, ITEM_CODE_LENGTH+MAX_ITEM_NAME_LENGTH+4+5+1, stream) != NULL) {
        strcpy(item_code[j], strtok(item, MENU_DELIM));
        strcpy(item_names[j], strtok(NULL, MENU_DELIM));
        item_cost_per_unit[j] = strtod(strtok(NULL, MENU_DELIM) + 1, NULL);
        //printf("%d", item_cost_per_unit[j]);
        //printf(item_names[j]);
        j++;
    }//actually make the menu

    fclose(stream);

    Menu* menu = malloc(sizeof(Menu));
    menu->item_codes = item_code; 
    menu->item_cost_per_unit = item_cost_per_unit;
    menu->item_names = item_names;
    menu->num_items = num_of_items;
    return menu;

}

/** build_order
 *
 * @param items item name
 * @param quantities item quantity
 * @return pointer to {@link Order}
 */
Order* build_order(char* items, char* quantities) {

    int j = 0; //used to iterate through string
    int num_items = 0;
    while(items[j] != '\0'){
        num_items++;
        j += 2;
    }
    j = 0;

    char** item_codes = malloc(sizeof(char*)*num_items);
    int* item_quantities = malloc(sizeof(int)*num_items); 

    for(int i = 0; i < num_items; i++){
        item_codes[i] = malloc(sizeof(char)*3);
    }

    char temp1[strlen(quantities) + 1];
    strcpy(temp1, quantities);
    
    for(int i = 0; i < num_items; i++){
        char temp[2];
        temp[0] = items[2*i];
        temp[1] = items[2*i+1];

        strcpy(item_codes[i], temp);

        if(i == 0){
            item_quantities[i] = strtod(strtok(temp1, MENU_DELIM), NULL);
        }else{
            item_quantities[i] = strtod(strtok(NULL, MENU_DELIM), NULL);
        }
    }//acquiring the orders from the inputs

    Order* order = malloc(sizeof(Order));
    order -> num_items = num_items; 
    order -> item_codes = item_codes; 
    order -> item_quantities = item_quantities; 
    return order;
}

/** enqueue_order
 * modify the {@var pending_orders} in {@var restaurant} to enqueue {@var order} by adding a node at the end
 *
 * @param order pointer to {@link Order}
 * @param restaurant pointer to {@link Restaurant}
 */
void enqueue_order(Order* order, Restaurant* restaurant) {
    Queue* queue = (*restaurant).pending_orders;
    QueueNode* node = malloc(sizeof(QueueNode));
    node->order = order; 
    node->next = NULL;

    if((*queue).head == NULL) {
        (*queue).head = node;
        (*queue).tail = node; 
    } else if ((*queue).head->next ==NULL){//added this edge case
        (*queue).head->next = node;
        (*queue).tail = node;

    }else {
        (*queue).tail->next = node;
        (*queue).tail = node; //added this one, only modification to this function
    }

    restaurant->num_pending_orders++;
}

/** dequeue_order
 * modify the {@var pending_orders} in {@var restaurant} to dequeue the order at the head of the queue
 *
 * free order and node
 * 
 * @param restaurant pointer to {@link Restaurant}
 * @return
 */
Order* dequeue_order(Restaurant* restaurant) {
    Order* order = NULL;
    Queue* queue = (*restaurant).pending_orders;
    int lim = (*restaurant).num_completed_orders;
    QueueNode* curNode = queue->head;
    for(int i = 0; i < lim; i++) {
        curNode = curNode->next;
    }
    order = curNode->order;
//    if((*queue).head->next == NULL) {
//        order = (*queue).head->order;
//        (*queue).head = NULL;
//        (*queue).tail = NULL;
//    } else {
//        order = (*queue).head->order;
//        (*queue).head = (*queue).head->next;
//    }

    (*restaurant).num_pending_orders--;
    (*restaurant).num_completed_orders++;
    return order;
}

/** get_item_cost
 * utilizes load_menu
 *
 * @param item_code string pointer to the item code
 * @param menu pointer to {@link Menu}
 * @return cost of {@var item_code} listed in {@var menu}
 */
double get_item_cost(char* item_code, Menu* menu) {

    char** item_codes = (*menu).item_codes;
    double* item_prices = (*menu).item_cost_per_unit;

    for(int j = 0; j < (*menu).num_items; j++){//find item on the menu
            if(strcmp(item_code, item_codes[j]) == 0){
                return item_prices[j];
            }
    }//not writing for edge cases, assuming item code exists
}

/** get_order_subtotal
 * utilizes get_item_cost
 *
 * @param order pointer to {@link Order}
 * @param menu pointer to {@link Menu}
 * @return the subtotal of all entries in {@var order}
 */
double get_order_subtotal(Order* order, Menu* menu) {
    double subtotal = 0; 
    char** order_codes = order->item_codes;
    int* order_quantities = order->item_quantities;
    char** item_codes = menu->item_codes; 
    double* item_prices = menu->item_cost_per_unit;
    
    double price = 0; 
    for(int i = 0; i <order->num_items; i ++){//loop through each item in the order
        for(int j = 0; j < menu->num_items; j++){//find item on the menu
            if(strcmp( order_codes[i], item_codes[j]) == 0){
                price = item_prices[j];
            }
        }//item is now found
        subtotal += price*order_quantities[i]; 
        price = 0;
    }
    
    return subtotal;
}

/** get_order_total
 * utilizes get_order_subtotal
 *
 * @param order pointer to {@link Order}
 * @param menu pointer to {@link Menu}
 * @return the total of all entries in {@var order}
 */
double get_order_total(Order* order, Menu* menu) {
    double sub = get_order_subtotal(order, menu);
    double tax = sub * (TAX_RATE / 100.0);
    double total = sub + tax;
    return total;
}

/** get_num_completed_orders
 *
 * @param restaurant pointer to {@link Restaurant}
 * @return
 */
int get_num_completed_orders(Restaurant* restaurant) {
    return (*restaurant).num_completed_orders;
}

/** get_num_pending_orders
 *
 * @param restaurant pointer to {@link Restaurant}
 * @return
 */
int get_num_pending_orders(Restaurant* restaurant) {
    return (*restaurant).num_pending_orders;
}

/** clear_order
 *
 * @param order pointer to {@link Order}
 */
void clear_order(Order** order) {

    int* item_quantities = (*order)->item_quantities;
    char** item_codes = (*order)->item_codes;
    int num_items = (*order)->num_items; 

    free(item_quantities);
    for(int i = 0; i < num_items; i ++){
        free(item_codes[i]);
    }
    free(item_codes);
    free(*order);

    return;
}

/** clear_menu
 *
 * @param menu pointer to {@link Menu}
 */
void clear_menu(Menu** menu) {
    
    char** item_codes = (*menu)->item_codes;
    char** item_names = (*menu)->item_names;
    double* item_costs = (*menu)->item_cost_per_unit;
    int num_items = (*menu)->num_items; 

    free(item_costs);

    for(int i = 0; i < num_items; i++){
        free(item_codes[i]);
        free(item_names[i]);
    }

    free(item_codes);
    free(item_names);
    free(*menu);

    return;
}

/** close_restaurant
 *
 * @param restaurant pointer to {@link Restaurant}
 */
void close_restaurant(Restaurant** restaurant) {
    free((*restaurant)->pending_orders);
    clear_menu(&((*restaurant)->menu));
    free(*restaurant);
    return;
}


void print_menu(Menu* menu){
	fprintf(stdout, "--- Menu ---\n");
	for (int i = 0; i < menu->num_items; i++){
		fprintf(stdout, "(%s) %s: %.2f\n", 
			menu->item_codes[i], 
			menu->item_names[i], 
			menu->item_cost_per_unit[i]	
		);
	}
}


void print_order(Order* order){
	for (int i = 0; i < order->num_items; i++){
		fprintf(
			stdout, 
			"%d x (%s)\n", 
			order->item_quantities[i], 
			order->item_codes[i]
		);
	}
}


void print_receipt(Order* order, Menu* menu){
	for (int i = 0; i < order->num_items; i++){
		double item_cost = get_item_cost(order->item_codes[i], menu);
		fprintf(
			stdout, 
			"%d x (%s)\n @$%.2f ea \t %.2f\n", 
			order->item_quantities[i],
			order->item_codes[i], 
			item_cost,
			item_cost * order->item_quantities[i]
		);
	}
	double order_subtotal = get_order_subtotal(order, menu);
	double order_total = get_order_total(order, menu);
	
	fprintf(stdout, "Subtotal: \t %.2f\n", order_subtotal);
	fprintf(stdout, "               -------\n");
	fprintf(stdout, "Tax %.2f%%: \t$%.2f\n", TAX_RATE, order_total);
	fprintf(stdout, "              ========\n");
}