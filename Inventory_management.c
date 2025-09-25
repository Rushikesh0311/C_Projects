#include <stdio.h>
struct inventory
{
   int product_id;
   char name[20];
   int quantity;
   float price;
};
void Add_item(struct inventory* arr,int* size)
{
    printf("Enter Product ID: ");
    scanf("%d",&arr->product_id);

    printf("Enter Product Name: ");
    scanf(" %[^\n]",arr->name);
    getchar();

    printf("Enter Quantity: ");
    scanf("%d",&arr->quantity);

    printf("Enter Price: ");
    scanf("%f",&arr->price);
}
int main(){
    struct inventory arr[50];
    int i = 1;
    int size = 0;
    int option;

    while(i)
    {
        //printf("");

        
        printf("1. Add item\n2. Display items\n3. Search item\n4. Update item\n5. Delete item");
        printf("\nSelect from Above menu: ");
        
        scanf("%d",&option);

        switch(option)
        {
            case 1:
                Add_item(arr,&size);
                break;
        }
         
        printf("(1-Yes/0-No)\n");
        printf("*Do you want to Continue: ");
        scanf("%d", &i);
    }
    

    

}
