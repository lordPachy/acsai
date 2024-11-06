#include <stdio.h>

struct Automobile{
        int price;
        char model[30];
        int hp;
        char colour[30];
    };

void insert_automobile_data(struct Automobile *car){
    printf("Insert the car model name (up to 30 chars):\n");
    scanf("%30[^\n]", car->model);
    printf("Insert the car hp:\n");
    scanf("%d", &(car->hp));
    printf("Insert the car colour (up to 30 chars):\n");
    scanf("%30[^\n]", car->colour);
    printf("Insert the car price:\n");
    scanf("%d", &(car->price));
}

void print_automobile_data(struct Automobile *car){
    printf("Car name is %s,\nit's got %d hp,\nit's %s-coloured\nand it costs %d",
            car->model, car->hp, car->colour, car->price);
}

int main(){
    struct Automobile car1, car2, car3;

    insert_automobile_data(&car1);
    insert_automobile_data(&car2);
    insert_automobile_data(&car3);

    print_automobile_data(&car1);
    print_automobile_data(&car2);
    print_automobile_data(&car3);

    return 0;
}