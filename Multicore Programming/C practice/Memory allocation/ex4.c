#include <stdio.h>

// Let's define books

struct Book{
    char title[100];
    int title_length;
    int price;
};

// We can then define a book difference function

int book_difference(struct Book *book1, struct Book *book2){

    // Equality check on title length
    if (book1->title_length == book2->title_length){
        
        // Equality check on title
        for (int i = 0; i < book1->title_length; i++)
            if (book1->title[i] != book2->title[i]){
                return 1;
            }
    } else {
        return 1;
    }
    
    // Equality check on price
    if (book1->price != book2->price){
        return 1;
    }

    return 0;
}

void book_input(struct Book *book){
    printf("Please, insert the book title: \n");
    scanf(" %s", &book->title);
    printf("Please, insert the book title length: \n");
    scanf(" %d", &book->title_length);
    printf("Please, insert the book price: \n");
    scanf(" %d", &book->price);
}

int main(void){
    struct Book book1, book2;
    
    // We can fill book fields
    book_input(&book1);
    book_input(&book2);

    printf("Books are %d\n", book_difference(&book1, &book2));
    return 0;
}